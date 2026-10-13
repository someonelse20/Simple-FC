---
title: "Simple FC"
author: "Someonelse"
description: "An RP2350 based flight controller with an IMU and barometer"
created_at: "2026-10-7"
---

# October 7: Selected Parts

### About the project
For this board I chose to make a flight controller as it is something I'm sort of familiar with and have designed a couple in the past.  I chose to base it of the RP2350. I've been working with a RP2040 board I made recently and I kinda like it so I chose to go with the new chip in the series. There are some similarities so I copied over the usb c, power regulator, memory, and oscillator circuits of my previous design because if it ain't broke don't fix it.

### Memory
I did want to change up the memory to something cheaper, the memory IC on my other board is about $5 for 128MBits but I chose to stick with it because I know it works and I'm not very familiar with external memory in general.

![Memory Circuit](images/mem_circuit.png)

### Sensors
For the IMU I chose the LSM6DSM because I've used similar sensors before. For the magnetometer and barometer I chose the MMC5983MA and BMP384 respectively because I've used them before.

### Storage
I decided to add a micro sd card for data logging. At first I wasn't sure because I'm using SPI for all the senors and I didn't know how many SPI modules the RP3250 has and if it will interrupt polling the sensors. After looking into it some more the RP3250 has two SPI modules so I decided to add one. I found a socket on digikey that kicad already has a footprint for and went with it.

![First schematic draft](images/schematic-1.png)


**Total time spent: 2 hours**

# October 7: Routed Sensors and Micro SD Card

### Sensors
I looked through the datasheets of each of the sensors and routed them up accordingly. This was pretty routine but for the magnetic sensor the datasheet called for only one 1uF cap for both power pins but I decided to add one for each power pin.

![Sensors circuit](images/sensor_circuit-1.png)

### Micro SD Card
I wasn't sure how to wire up the micro sd card at first since the rp2350 can only communicate with it with spi. I found the pinout for it and added 10k pullup resistors for each pin. I didn't need a logic shifter as the mcu and the card both use 3.3v.

![Micro SD circuit](images/microsd_circuit-1.png)

**Total time spent: 1 hour**

# October 8: Switched to Buck-Boost Regulator

### Why the switch
I was thinking more about how this would work with other boards and I decided it needs a wider input voltage range and a higher output current so it can power other boards as well. So I went looking for a buck-boost regulator and found the TPS631000.

### Changes to the schematic
Kicad didn't have the symbol for the TPS631000 so I copied the symbol for the TPS63000 and made the changes myself.

![TPS63000 symbol](images/TPS63000_sym.png)

The example schematic already had the requirements I needed so I pretty much copied that and called it a day.

![TPS631000 circuti](images/TPS63000_circuit-1.png)

**Total time spent: 1 hour**

# October 8: Made MCU Power Circuit

I went to start routing up the RP2350 and started with the power circuit. After reading the datasheet I was a little confused but I started trying to recreate the example power circuit for the build in 1.1v regulator it needs.

![Power reference circuit](images/power_ref_circuit.png)

The default kicad symbol was making it confusing so I changed it to be closer to the datasheet.
![MCU power circuit](images/MCU_power_circuit-1.png)

I also changed the regulator circuit to a pull down resistor so it defaults to low power mode but can be toggled with a gpio.
![TPS63000 circuit](images/TPS63000_circuit-2.png)

**Total time spent: 1 hour**

# October 10: Routed MCU and added pin headers

### Routing MCU 

I spent a while looking at the pintout for the RP2350 so I can wire up the spi, i2c, uart, adc, and gpio correctly.

![MCU pin functions](images/MCU_pin_funcs.png)

After a few tries of reorganizing the layout I came up with this that has two SPIs, an I2c, two UARTs, four PWMs, one ADC, and a few GPIOs.

![MCU circuit](images/MCU_circuit-1.png)

I also added an ADC to read the vbus voltage and a resistor divider so it can read volages above 3.3v.

![USB circuit](images/USB_circuit-1.png)

### Pin headers

The flight controller needs pinouts so it can communicate with other boards. I added two UART headers in a special format so it can communicated with other boards I have made. I also added two I2C headers because the MCU only has two UARTs and with I2C it can communicate with many more boards. I added a serial wire debug header so it can be easily debugged and programmed. I also added one more header so it can communicate with an ESC. I then added two status LEDs, one that shows if the board is powered and another connected to a GPIO for software status.

![Pin headers](images/header_circuit-1.png)

**Total time spent: 2 hours**

# October 10: Drafted PCB layout

I added footprints to everything and opened up the pcb editor. I first tried making the headers jst but it wouldn't fit on the board well and I would have to put things on both sides.

![PCB layout 1](images/pcb-1.png)

I switched over to simple pin headers and was able to get everything to fit better.

![PCB layout 2](images/pcb-2.png)

I then started adding the rest of the chips and to make things fit I also had to move the mounting holes closer to the corners. I tried to place the magnetic sensor (the one in the corner near the micro sd) away from the power circuit to reduce noise.

![PCB layout 3](images/pcb-3.png)

**Total time spent: 1.5 hours**

# October 11: Routed power and MCU for PCB

### Routing power
Per the RP2350 datasheet I started with routing the mcu's regulator. The example layout had a power trace running between two capacitors which I couldn't do without changing the spacing tolerances in kicad. I found a way to route it but ran into problems when I then tried routing the usb and qspi traces since they are all on that same side of the mcu. I kept tweaking things until they fit although I'm a little uncomfortable with how close everything is.

![MCU power layout](images/mcu_power_pcb-1.png)

I then went and routed the power regulator. Last journal I forgot to mention I added copper pads for an additial power input other than usb. I was unable to find a place to put it closer to the regulator but I think I can get away with a longer trace for it. The regulator layout was fairly simple and wasn't to complicated.

![Power layout](images/power_pcb-1.png)

Finally I placed all the decoupling capacitors and the oscillator. The only part that gave me trouble was the oscillator. I couldn't fit it as close as I would have liked because of the micro sd card slot. After looking at the pico 2 for reference I decided it will be ok to place it a little further.

![MCU capacitors / oscillator](images/mcu_caps_pcb-1.png)

**Total time spent: 3 hours**

# October 11: Routed other ICs

I went to route together all the other circuits. First I noticed that I forgot to add the decoupling capacitors to the regulator so I did that and tried to compact it a little more.

![New power PCB](images/power_pcb-2.png)

I also forgot to add the snubber circuit to the anaglog voltage reference so I did that too. Then I added the decoupling capacitors to each of the sensors and added the pull up resistors to the micro sd card.

![Sensors PCB](images/sensors_pcb-1.png)

![Magnetometer PCB](images/magnetometer_pcb-1.png)

**Total time spent: 2 hours**

# October 12: Finished routing PCB

After routing each of the separate circuits I starting routing them all together. Once I started I noticed I forgot to wire up the interrupt pins on the schematic which I quickly fixed. At first I tried to make it a two layer board but I ran out of space. I changed it to four layers and even now it's a bit crowded around the MCU. The pico 2 gets away with easier routing because it uses 0201 isntead of 0402 capacitors which I can't do. I did make the vias smaller which helped a lot but it still took a few tries to get everything to fit. Also around the magnetometer I added a zone with no copper fill for less magnetic noise. I tried to keep most of the signal traces to one layer so I could have ground and power layers. Below is the final result with and without the copper fills.

![PCB no fills](images/pcb-4.png)

![PCB with fills](images/pcb-5.png)

**Total time spent: 2.5 hours**

# October 12: Added 3D models and silkscreen

### 3D models
To pretty up the board I went around and found the 3d files for all the components that didn't have one. While doing this I noticed that the micro sd slot was backwards. So I fixed that and reajusted the pull up resistors. 

![Micro SD PCB](images/microsd_pcb-1.png)

I then added nice silkscreen labels to all the pin headers. 

![3D back](images/3d-1.png)

I then wanted to make a label to show which direction the sensor axes are. While double checking the datasheets I noticed that the magnetometer axes aren't aligned with the IMU's so I rotated the magnetometer until they did. While the x and y axes align the magnetometer has the z axis going down while the IMU's z axis goes up. Still it's much better and makes software easier.

![Rotated magnetometer PCB](images/magnetometer_pcb-2.png)

Here's the full 3D render:

![Full 3D](images/3d-2.png)

**Total time spent: 2 hours**

# October 13: Got project ready to ship

To get this ready to ship I started making the BOM. For this I went to the symbol fields spreadsheet in kicad and digikey and copied the part name into the spreadsheet. This was pretty tedious but I got it all done. I then wrote up a readme with what this project is and how it works and grabbed some screenshots for images.

![readme file](images/readme.png)

Now it's ready to ship!

**Total time spent: 2 hours**

