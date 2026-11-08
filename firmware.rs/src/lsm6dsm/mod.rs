//! # SPI DMA Example
//!
//! This application demonstrates how to use DMA for SPI transfers.
//!
//! The application expects the MISO and MOSI pins to be wired together so that it is able to check
//! whether the data was sent and received correctly.
//!
//! See the `Cargo.toml` file for Copyright and licence details.

use embedded_hal::spi::SpiDevice;
use embedded_hal_bus::spi::ExclusiveDevice;
// Ensure we halt the program on panic (if we don't mention this crate it won't
// be linked)
use panic_halt as _;

// Alias for our HAL crate
use rp235x_hal as hal;

// Some things we need
use hal::clocks::Clock;
use hal::fugit::RateExtU32;

/// External high-speed crystal on the Raspberry Pi Pico 2 board is 12 MHz.
/// Adjust if your board has a different frequency
const XTAL_FREQ_HZ: u32 = 12_000_000u32;

mod regs;

pub struct Lsm6dsm {
    // spi: ExclusiveDevice<hal::spi::Spi<_, _, _, 8>, hal::gpio::Pins, hal::Timer<_>>,

    pub gyro: [f32; 3],
    pub accel: [f32; 3],
    pub temp: i16,
}

impl Lsm6dsm {
    pub fn new() {
        // Grab our singleton objects
        let mut pac = hal::pac::Peripherals::take().unwrap();

        // Set up the watchdog driver - needed by the clock setup code
        let mut watchdog = hal::Watchdog::new(pac.WATCHDOG);

        // Configure the clocks
        let clocks = hal::clocks::init_clocks_and_plls(
            XTAL_FREQ_HZ,
            pac.XOSC,
            pac.CLOCKS,
            pac.PLL_SYS,
            pac.PLL_USB,
            &mut pac.RESETS,
            &mut watchdog,
        )
        .unwrap();

        let mut timer = hal::Timer::new_timer0(pac.TIMER0, &mut pac.RESETS, &clocks);

        // The single-cycle I/O block controls our GPIO pins
        let sio = hal::Sio::new(pac.SIO);

        // Set the pins to their default state
        let pins = hal::gpio::Pins::new(
            pac.IO_BANK0,
            pac.PADS_BANK0,
            sio.gpio_bank0,
            &mut pac.RESETS,
        );

        // These are implicitly used by the spi driver if they are in the correct mode
        let spi_mosi = pins.gpio7.into_function::<hal::gpio::FunctionSpi>();
        let spi_miso = pins.gpio4.into_function::<hal::gpio::FunctionSpi>();
        let spi_sclk = pins.gpio6.into_function::<hal::gpio::FunctionSpi>();
        let spi_bus = hal::spi::Spi::<_, _, _, 8>::new(pac.SPI0, (spi_mosi, spi_miso, spi_sclk));

        // Exchange the uninitialised SPI driver for an initialised one
        let spi_bus = spi_bus.init(
            &mut pac.RESETS,
            clocks.peripheral_clock.freq(),
            16.MHz(),
            embedded_hal::spi::MODE_0,
        );

        let spi_cs = pins.gpio8.into_function::<hal::gpio::FunctionSioOutput>();
        let mut spi = ExclusiveDevice::new(spi_bus, spi_cs, &mut timer).unwrap();
    }

    pub fn set_conf() {

    }

    pub fn check() -> bool {
        return true;
    }

    pub fn poll() -> [i32; 6] {
        let data_out: [i32; 6] = [0, 0, 0, 0, 0, 0];
        return data_out;
    }
}


// End of file
