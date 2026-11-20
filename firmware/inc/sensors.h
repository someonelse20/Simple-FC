#ifndef SENSORS_H
#define SENSORS_H

#include <pico/stdlib.h>

#define SPI_BUS spi0
#define SPI_DELAY 10

#define READ_BIT              1
#define WRITE_BIT             0

void write_reg(uint8_t reg, uint8_t buf);
void write_mask(uint8_t reg, uint8_t value, uint8_t mask);
void read_regs(uint8_t reg, uint8_t *buf, uint16_t len);

#endif
