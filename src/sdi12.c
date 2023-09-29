#include "uart.h"
#include "rcc.h"
#include "gpio.h"
#include "utils.h"
#include "sdi12.h"

void sdi12_init(struct Uart *uart) {
    if (uart == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR2_BIT_USART1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_BIT_GPIOAEN;

        // gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_ALTERNATE_FUNCTION);
        // gpio_set_mode(GPIOA, GPIO_PIN_10, GPIO_MODE_ALTERNATE_FUNCTION);

        gpio_set_af(GPIOA, GPIO_PIN_9, GPIO_PA9_AF_USART1_TX);
        // gpio_set_af(GPIOA, GPIO_PIN_10, GPIO_PA10_AF_USART1_RX);

        uart->CR3 |= UART_CR3_BIT_HDSEL; // half-duplex mode
        // GPIOA->OTYPER |= 1 << GPIO_PIN_9; // TX pin open-drain
        // GPIOA->PUPDR |= 0b01 << (GPIO_PIN_9 * 2); // TX pull-up
        GPIOA->PUPDR |= 0b10 << (GPIO_PIN_9 * 2); // TX pull-down

        uart->CR2 |= UART_CR2_BIT_TXINV | UART_CR2_BIT_RXINV; // negative logic

        uart->BRR = UART1_KER_CK_PRES / 1200;
        
        uart->CR1 &= ~(UART_CR1_BIT_M1 | UART_CR1_BIT_M0);
        uart->CR1 |= UART_CR1_BIT_M1;   // set word length to 7

        uart->CR1 |= UART_CR1_BIT_PCE; // parity control enabled
        uart->CR1 &= ~(UART_CR1_BIT_PS); // even parity

        uart->CR2 &= ~(UART_CR2_BIT_STOP1 | UART_CR2_BIT_STOP0); // 1 stop bit

        uart->CR1 |= UART_CR1_BIT_UE | UART_CR1_BIT_TE | UART_CR1_BIT_RE;

        gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_ALTERNATE_FUNCTION);
        // gpio_write(GPIOA, GPIO_PIN_9, LOW);
    }
}

void sdi12_wake_up(struct Uart *uart) {
    if (uart == USART1) {
        gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT);
        gpio_write(GPIOA, GPIO_PIN_9, HIGH);
        delay(12);
        gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_ALTERNATE_FUNCTION);
        // gpio_write(GPIOA, GPIO_PIN_9, LOW);
        delay(9);
    }
}

uint8_t sdi12_send_command(struct Uart *uart, uint8_t addr, char *cmd) {
    if (addr > 9) {
        return SDI12_ERR_SEND_COMMAND_SENS_ADDR;
    }
    if (cmd[0] != 'a') {
        return SDI12_ERR_SEND_COMMAND_CMD_FORMAT;
    }

    cmd[0] = addr + 0x30;

    if (uart == USART1) {
        // gpio_set_mode(GPIOA, GPIO_PIN_9, GPIO_MODE_ALTERNATE_FUNCTION);
        uart_write_buf(uart, cmd);
    }

    return SDI12_SEND_COMMAND_OK;
}

uint8_t sdi12_get_sensor_response(struct Uart *uart, char *buf, uint8_t buf_len, uint8_t addr, bool check_short_res) {
    struct CpuTimer *cpu_timer_sensor_response_timeout = cpu_timer_new(15);

    uint8_t i;
    for (i = 0; i < buf_len; i++) {
        while (!uart_data_received(uart)) {
            if (cpu_timer_wait(cpu_timer_sensor_response_timeout) == 1) {
                cpu_timer_remove(cpu_timer_sensor_response_timeout);
                goto end_loop;
            }
        };

        buf[i] = (uint8_t)uart_read_data(uart);
        cpu_timer_init(cpu_timer_sensor_response_timeout, 2);
    }

    end_loop:
    cpu_timer_remove(cpu_timer_sensor_response_timeout);

    if (i == 0) {
        return SDI12_ERR_GET_SENSOR_RESPONSE_TIMEOUT;
    }
    if (check_short_res && i < buf_len) {
        return SDI12_ERR_GET_SENSOR_RESPONSE_SHORT_RES;
    }
    if (buf[0] != addr + 0x30) {
        return SDI12_ERR_GET_SENSOR_RESPONSE_ADDR_NO_MATCH;
    }
    if (buf[i - 2] != '\r' || buf[i - 1] != '\n') {
        return SDI12_ERR_GET_SENSOR_RESPONSE_TERMINATOR;
    }

    return SDI12_GET_SENSOR_RESPONSE_OK;
}

uint8_t sdi12_get_measurement(struct Uart *uart, uint8_t addr, struct Float *measurements, uint8_t *received_measurement_count) {
    uint8_t err;

    sdi12_wake_up(uart);

    err = sdi12_send_command(uart, addr, SDI12_CMD_START_MEASUREMENT);
    if (err != SDI12_SEND_COMMAND_OK) {
        return SDI12_ERR_GET_MEASUREMENT_SEND_START_MEAS_CMD;
    }

    char start_measurement_res_buf[SDI12_START_MEASUREMENT_RES_LEN];
    err = sdi12_get_sensor_response(uart, start_measurement_res_buf, SDI12_START_MEASUREMENT_RES_LEN, addr, true);
    if (err != SDI12_GET_SENSOR_RESPONSE_OK) {
        return SDI12_ERR_GET_MEASUREMENT_GET_RES_START_MEAS;
    }

    uint8_t wait_time = (start_measurement_res_buf[1] - 0x30)*100 + (start_measurement_res_buf[2] - 0x30)*10 + (start_measurement_res_buf[3] - 0x30);
    struct CpuTimer *cpu_timer_wait_measurement = cpu_timer_new(wait_time*1000); // wait_time is expressed in seconds

    bool wake_up_sensor = false;
    while (!sdi12_received_service_request(uart, addr)) {
        if (cpu_timer_wait(cpu_timer_wait_measurement) == 1) {
            wake_up_sensor = true;
            break;
        }
    }
    cpu_timer_remove(cpu_timer_wait_measurement);

    if (wake_up_sensor) sdi12_wake_up(uart);
    char cmd_send_data[] = SDI12_CMD_SEND_DATA;
    uint8_t measurement_number = start_measurement_res_buf[4] - 0x30;
    *received_measurement_count = 0;

    for (uint8_t data_block = 0; data_block < SDI12_MAX_DATA_BLOCKS; data_block++) {
        cmd_send_data[0] = 'a';
        cmd_send_data[2] = data_block + 0x30;
        err = sdi12_send_command(uart, addr, cmd_send_data);
        if (err != SDI12_SEND_COMMAND_OK) {
            return SDI12_ERR_GET_MEASUREMENT_SEND_SEND_DATA_CMD;
        }

        char send_data_res_buf[SDI12_SEND_DATA_RES_LEN];
        err = sdi12_get_sensor_response(uart, send_data_res_buf, SDI12_SEND_DATA_RES_LEN, addr, false);
        if (err != SDI12_GET_SENSOR_RESPONSE_OK) {
            return SDI12_ERR_GET_MEASUREMENT_GET_RES_SEND_DATA;
        }

        char *buf_ptr = send_data_res_buf + 1;
        while (*buf_ptr != '\r') {
            struct Sdi12Value value = sdi12_parse_next_value(&buf_ptr);
            // measurements[*received_measurement_count] = 0;
            // for (uint8_t digits_idx = 0; digits_idx < value.non_decimal_digits_count; digits_idx++) {
            //     measurements[*received_measurement_count] += value.non_decimal_digits[digits_idx]*power(10, value.non_decimal_digits_count - 1 - digits_idx);
            // }
            // for (uint8_t digits_idx = 0; digits_idx < value.decimal_digits_count; digits_idx++) {
            //     measurements[*received_measurement_count] += value.decimal_digits[digits_idx]*power(10, -(digits_idx + 1));
            // }
            // measurements[*received_measurement_count] *= value.polarity_sign;
            measurements[*received_measurement_count].value = 0;
            measurements[*received_measurement_count].decimal_count = 0;
            uint8_t digits_idx = 0;
            uint8_t total_digitis_count = value.non_decimal_digits_count + value.decimal_digits_count;
            while (digits_idx < value.non_decimal_digits_count) {
                measurements[*received_measurement_count].value +=
                    value.non_decimal_digits[digits_idx]*power(10, total_digitis_count - 1 - digits_idx);
                digits_idx++;
            }
            while (digits_idx < total_digitis_count) {
                measurements[*received_measurement_count].value +=
                    value.decimal_digits[digits_idx - value.non_decimal_digits_count]*power(10, total_digitis_count - 1 - digits_idx);
                digits_idx++;
                measurements[*received_measurement_count].decimal_count++;
            }
            measurements[*received_measurement_count].value *= value.polarity_sign;
            (*received_measurement_count)++;
        }

        if (*received_measurement_count >= measurement_number) break;
    }

    return SDI12_GET_MEASUREMENT_OK;
}

struct Sdi12Value sdi12_parse_next_value(char **buf) {
    struct Sdi12Value sdi12_value = {
        .non_decimal_digits_count = 0,
        .decimal_digits_count = 0
    };

    if (**buf != '+' && **buf != '-') {
        sdi12_value.error_code = SDI12_ERR_PARSE_NEXT_VALUE_POLARITY_SIGN;
        return sdi12_value;
    }

    sdi12_value.polarity_sign = **buf == '+' ? 1 : -1;
    (*buf)++;

    bool decimal_part = false;
    while (**buf != '+' && **buf != '-' && **buf != '\r') {
        if (**buf == '.') {
            decimal_part = true;
        } else {
            if (decimal_part) {
                sdi12_value.decimal_digits[sdi12_value.decimal_digits_count] = **buf - 0x30;
                sdi12_value.decimal_digits_count++;
            } else {
                sdi12_value.non_decimal_digits[sdi12_value.non_decimal_digits_count] = **buf - 0x30;
                sdi12_value.non_decimal_digits_count++;
            }
        }
        (*buf)++;
    }
    return sdi12_value;
}

bool sdi12_received_service_request(struct Uart *uart, uint8_t addr) {
    if (!uart_data_received(uart)) {
        return false;
    }

    char res_buf[SDI12_SERVICE_REQUEST_LEN];
    uint8_t err = sdi12_get_sensor_response(uart, res_buf, SDI12_SERVICE_REQUEST_LEN, addr, true);
    if (err != SDI12_GET_SENSOR_RESPONSE_OK) {
        return false;
    }

    return true;
}