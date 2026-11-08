//! # SPI DMA Example
//!
//! This application demonstrates how to use DMA for SPI transfers.
//!
//! The application expects the MISO and MOSI pins to be wired together so that it is able to check
//! whether the data was sent and received correctly.
//!
//! See the `Cargo.toml` file for Copyright and licence details.

use embedded_hal::spi::SpiBus;
use rp235x_hal::{self as hal, gpio};

use hal::singleton;

use embedded_hal::delay::DelayNs;
use embedded_hal::digital::OutputPin;
use hal::clocks::Clock;
use hal::dma::{bidirectional, DMAExt};
use hal::fugit::RateExtU32;

// Ensure we halt the program on panic (if we don't mention this crate it won't
// be linked)
use panic_halt as _;

mod spi;
mod regs;

pub struct Lsm6dsm {
    cs_pin: gpio::Pins,

    gyro: [f32; 3],
    accel: [f32; 3],
    temp: i16,
}

impl Lsm6dsm {
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
