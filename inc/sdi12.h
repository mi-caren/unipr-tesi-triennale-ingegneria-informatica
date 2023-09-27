#ifndef SDI12_H
#define SDI12_H

#include <inttypes.h>

#define SDI12_START_MEASUREMENT_RESPONSE_LENGTH                 7

void sdi12_init(struct Uart *uart);
void sdi12_wake_up(struct Uart *uart);
uint8_t sdi12_start_measurement(struct Uart *uart, uint8_t sensor_address);
uint8_t sdi12_get_sensor_response(struct Uart *uart, char *buf, uint8_t buf_len);

#endif