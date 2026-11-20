#ifndef SENSORS_H
#define SENSORS_H

#include <pico/stdlib.h>

#define SPI_BUS spi0
#define SPI_DELAY 10

#define READ_BIT              1
#define WRITE_BIT             0

void spi_write_reg(uint16_t cs_pin, uint8_t reg, uint8_t buf);
void spi_write_mask(uint16_t cs_pin, uint8_t reg, uint8_t value, uint8_t mask);
void spi_read_regs(uint16_t cs_pin, uint8_t reg, uint8_t *buf, uint16_t len);

#endif
