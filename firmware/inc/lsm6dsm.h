#ifndef LSM6DSM_H
#define LSM6DSM_H

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <stdint.h>


#define LSM_ODR_0001_6 0x0B
#define LSM_ODR_0012_5 0x01
#define LSM_ODR_0026   0x02
#define LSM_ODR_0052   0x03
#define LSM_ODR_0104   0x04
#define LSM_ODR_0208   0x05
#define LSM_ODR_0416   0x06
#define LSM_ODR_0833   0x07
#define LSM_ODR_1660   0x08
#define LSM_ODR_3330   0x09
#define LSM_ODR_6660   0x0A

struct LSM6DSM_h {
	uint8_t CTRL1_XL_reg;
	uint8_t CTRL2_G_reg;
	uint8_t CTRL3_C_reg;
};

void LSM6DSM_init(struct LSM6DSM_h *lsm6dsm);
bool LSM6DSM_check_id();
void LSM6DSM_reset(struct LSM6DSM_h *lsm6dsm);
void LSM6DSM_clear_mem(struct LSM6DSM_h *lsm6dsm);

#endif
