#ifndef LSM6DSM_H
#define LSM6DSM_H

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <stdint.h>

// Hardware defs
#define LSM_PIN_INT1 18
#define LSM_PIN_INT2 23
#define LSM_PIN_CS 21

// FIFO Decimation
#define LSM_FIFO_DEC_0               0x01
#define LSM_FIFO_DEC_2               0x02
#define LSM_FIFO_DEC_3               0x03
#define LSM_FIFO_DEC_4               0x04
#define LSM_FIFO_DEC_8               0x05
#define LSM_FIFO_DEC_16              0x06
#define LSM_FIFO_DEC_32              0x07

// FIFO Modes
#define LSM_FIFO_MODE_BYPASS         0x01
#define LSM_FIFO_MODE_FIFO           0x02
#define LSM_FIFO_MODE_CONT           0x04
#define LSM_FIFO_MODE_CONT_FIFO      0x05
#define LSM_FIFO_MODE_BYPASS_CONT    0x07

// Output Data Rates
#define LSM_ODR_0001_6               0x0B
#define LSM_ODR_0012_5               0x01
#define LSM_ODR_0026                 0x02
#define LSM_ODR_0052                 0x03
#define LSM_ODR_0104                 0x04
#define LSM_ODR_0208                 0x05
#define LSM_ODR_0416                 0x06
#define LSM_ODR_0833                 0x07
#define LSM_ODR_1660                 0x08
#define LSM_ODR_3330                 0x09
#define LSM_ODR_6660                 0x0A

// Linear Accelerometer Full Scales
#define LSM_XL_FS_2                  0x00
#define LSM_XL_FS_4                  0x02
#define LSM_XL_FS_8                  0x03
#define LSM_XL_FS_16                 0x01

// Linear Accelerometer Sensivities
#define LSM_XL_SO_2                  0.061
#define LSM_XL_SO_4                  0.122
#define LSM_XL_SO_8                  0.244
#define LSM_XL_SO_16                 0.488

// Gyroscope Full Scales
#define LSM_G_FS_250                 0x00
#define LSM_G_FS_500                 0x01
#define LSM_G_FS_1000                0x02
#define LSM_G_FS_2000                0x03

// Gyroscope Sensivities
#define LSM_G_SO_125                 4.375
#define LSM_G_SO_250                 8.75
#define LSM_G_SO_500                 17.50
#define LSM_G_SO_1000                35
#define LSM_G_SO_2000                75

void LSM6DSM_init();
bool LSM6DSM_check_id();

void LSM6DSM_reset();
void LSM6DSM_clear_mem();

void LSM6DSM_set_XL_ODR(uint8_t ODR);
void LSM6DSM_set_XL_scale(uint8_t scale);
void LSM6DSM_set_G_ODR(uint8_t ODR);
void LSM6DSM_set_G_scale(uint8_t scale);

void LSM6DSM_int_handler(uint gpio, uint32_t events);

float *LSM6DSM_get_gyro();
float *LSM6DSM_get_accel();

#endif
