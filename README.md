# SW7456
A software emulation of a MAX7456 OSD chip running on a ARM cortex M0+ MCU.

It is meant to be a drop in replacement for an AT7456 chip, using the same SPI interface as a real chip.
The project was built for an STM32G071 chip, but could run on an STM32G031 if the video generator is compiled out.

A schematic for the hardware, that is needed to run this software, is avalable at https://www.moddquad.com/p/fc.html
You may need to use a 8MHz crystal oscillator to drive the OSD chip. A MEMS oscillator may have too much phase noise to work properly. 
The software is built using STM32Cube IDE which is available for free at ST Micro's website www.st.com.
 
Most feature are implemented including;
Attribute bits (local background, blinking, invert)
Generates its own video signal when none is present. Automacically switches when camera is reconnecated.
horizontal and vertical adjustment and brightness setting via 7456 registers

Known issues:
1) No SPI read compatability. Only write compatability.
all SPI reads will return the value 0x00.

 
2) Line start detection quanitization
This causes lines to start at slightly different times (around 1/8 of a pixel width) which make vertical lines less crisp.
All software OSD will suffer from this issue to some degree, I did my best to minimize the artifact. 

Any MCU can only detect things at its own clock rate. 
Anything that occurs in between two clocks will be detected at the next clock.
This implementation uses a 64MHz clock and a two cycle M0+ pipline.
Using a higher clock will reduce this artifacting.

3) Some pixels get extended during SPI communication
SPI data is read using DMA, this is required because 80% of the time the MCU is drawing pixels.
The artifact occurs because the DMA inserts wait states into cpu execution.
This can be mitigated by choosing a MCU with RAM split into multiple banks that can accessed simultaneously.
