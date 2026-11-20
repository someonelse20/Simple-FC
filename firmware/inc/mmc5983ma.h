#include <pico/stdlib.h>

// Hardware defs
#ifndef MMC5983MA_H
#define MMC5983MA_H

#define MMC_PIN_INT 15
#define MMC_PIN_CS 17

// Output Data Rates
#define MMC_ODR_100               0x00
#define MMC_ODR_200               0x01
#define MMC_ODR_400               0x02
#define MMC_ODR_800               0x03

void MMC5983MA_init();
bool MMC5983MA_check_id();

void MMC5983MA_reset();

void MMC5983MA_set_ODR(uint8_t ODR);
void MMC5983MA_set_scale(uint8_t scale);

void MMC5983MA_int_handler(uint gpio, uint32_t events);

void MMC5983MA_poll();

#endif
