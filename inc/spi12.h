#ifndef SPI12_H
#define SPI12_H

void spi12_init(struct Uart *uart);
void spi12_wake_up(struct Uart *uart);
uint8_t spi12_start_measurement(struct Uart *uart, uint8_t sensor_address);

#endif