#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/binary_info.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>

#include "main.h"
#include "lsm6dsm.h"

int main() {
	stdio_init_all();

	spi_init(spi0, 16000);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCLK, GPIO_FUNC_SPI);
	bi_decl(bi_3pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCLK, GPIO_FUNC_SPI));

	gpio_init(PIN_LED);
	gpio_set_dir(PIN_LED, GPIO_OUT);
	gpio_put(PIN_LED, 0);
	bi_decl(bi_1pin_with_name(PIN_LED, "LED"));

	sleep_ms(1000);

	if (LSM6DSM_check_id() == true) {
		gpio_put(PIN_LED, 1);
	} else {
		while (true) {
			gpio_put(PIN_LED, 1);
			sleep_ms(100);
			gpio_put(PIN_LED, 0);
			sleep_ms(100);
			gpio_put(PIN_LED, 1);
			sleep_ms(100);
			gpio_put(PIN_LED, 0);

			sleep_ms(1000);
		}
	}
}
