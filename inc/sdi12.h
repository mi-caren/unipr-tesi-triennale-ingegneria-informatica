#ifndef SDI12_H
#define SDI12_H

#include <inttypes.h>
#include <stdbool.h>

// Lengths of command responses
#define SDI12_START_MEASUREMENT_RES_LEN                         7
#define SDI12_SERVICE_REQUEST_LEN                               3
#define SDI12_SEND_DATA_RES_LEN                                 ( 35 + 3 )

#define SDI12_CMD_START_MEASUREMENT                             ( "aM!" )
#define SDI12_CMD_SEND_DATA                                     ( "aD0!" )

#define SDI12_MAX_DATA_BLOCKS                                   9
#define SDI12_MAX_DIGITS                                        7
#define SDI12_MAX_MEASUREMENTS                                  9

struct Sdi12Value {
    int8_t polarity_sign;
    uint8_t non_decimal_digits[SDI12_MAX_DIGITS];
    uint8_t non_decimal_digits_count;
    uint8_t decimal_digits[SDI12_MAX_DIGITS];
    uint8_t decimal_digits_count;
    uint8_t error_code;
};

enum Sdi12Errors {
    // sdi12_send_command
    SDI12_SEND_COMMAND_OK                           = 0,

    SDI12_ERR_SEND_COMMAND_SENS_ADDR                = 1,
    SDI12_ERR_SEND_COMMAND_CMD_FORMAT               = 2,

    // sdi12_get_sensor_response
    SDI12_GET_SENSOR_RESPONSE_OK                    = 0,
    SDI12_ERR_GET_SENSOR_RESPONSE_ADDR_NO_MATCH     = 1,
    SDI12_ERR_GET_SENSOR_RESPONSE_TIMEOUT           = 2,
    SDI12_ERR_GET_SENSOR_RESPONSE_SHORT_RES         = 3,
    SDI12_ERR_GET_SENSOR_RESPONSE_TERMINATOR        = 4,

    // sdi12_get_measurement
    SDI12_GET_MEASUREMENT_OK                        = 0,
    SDI12_ERR_GET_MEASUREMENT_SEND_START_MEAS_CMD   = 1,
    SDI12_ERR_GET_MEASUREMENT_GET_RES_START_MEAS    = 2,
    SDI12_ERR_GET_MEASUREMENT_SEND_SEND_DATA_CMD    = 3,
    SDI12_ERR_GET_MEASUREMENT_GET_RES_SEND_DATA     = 4,

    // sdi12_parse_next_value
    SD12_PARSE_NEXT_VALUE_OK                        = 0,
    SDI12_ERR_PARSE_NEXT_VALUE_POLARITY_SIGN        = 1,
};

void sdi12_init(struct Uart *uart);
void sdi12_wake_up(struct Uart *uart);
uint8_t sdi12_send_command(struct Uart *uart, uint8_t addr, char *cmd);
uint8_t sdi12_get_sensor_response(struct Uart *uart, char *buf, uint8_t buf_len, uint8_t addr, bool check_short_res);
// uint8_t sdi12_get_measurement(struct Uart *uart, uint8_t addr, double *measurements, uint8_t *received_measurement_count);
uint8_t sdi12_get_measurement(struct Uart *uart, uint8_t addr, struct Float *measurements, uint8_t *received_measurement_count);
struct Sdi12Value sdi12_parse_next_value(char *buf);
bool sdi12_received_service_request(struct Uart *uart, uint8_t addr);

#endif