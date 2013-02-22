# RGB Ring code for Arduino v1.0

In this repository you will find example code for the Rainbow Ring Arduino board that has been migrated 
from the **0022 alpha** platform to **1.0.3**. 

The board is based on the **Amtel 8-bit atmega168** microcontroller 
and has a ring of 12 color RGB LED's that are independently addressible. More information about the Rainbow Ring can be found in the
[DFRobot wiki][1] and the board can be purchased from their [online store][2].

![alt text][board]

[1]: http://www.dfrobot.com/wiki/index.php?title=Rainbow_Ring_V3_(SKU:DFR0141
[2]: http://www.dfrobot.com/index.php?route=product/product&filter_name=rainbow&product_id=487#.USZMfKUVf9w
[board]: http://www.dfrobot.com/wiki/images/thumb/0/06/IMGP4358.jpg/350px-IMGP4358.jpg

## RGBRingDemo

This code cycles through various example functions to
produce a colored light show that can be controlled over a serial link. 
This code is a re-factored version of the original demo code obtained from the DFRobot wiki.

Changes made from the demo include
* Migration of Arduino platform from v0022 to v1.0.3.
* Correction of compiler errors and warnings.
* Splitting of code into separate cpp files which makes it easier to understand.
 
Code was moved from old to new files as follows:
```
 RGB_Ring_V3.pde => RGBRingDemo.ino
 RGB_Ring_V3.h => RGBRingUtil.h
                  RGBRingUtil.cpp
                  RGBRingUart.cpp
                  RGBRingTimer.h
                  RGBRingTimer.cpp
                  RGBRingLed.h
                  RGBRingLed.cpp
```

## Archive-0022

This directory contains the original code that works only on the Arduino 0022 platform.

## optiboot-RGBRing

**current status: Code is merged and compiles but flashing of bootloader is not complete.**

The RGB Ring uses a customized version of the optiboot loader. The customizations modify the pins used to flash 
the LED during the booloader stage. This directory contains the version of optiboot shipped with Arduino 1.0.3
with the customized changes. 

There are 4 methods you can use to flash a bootloader onto the atmega168:

1. **AVRISP serial:** Use an FTDI board that connects to a USB port and sends a serial signal to the
6-pin serial interface on the board.
2. **AVRISP mkII:** This is a programmer from Amtel that connects to the 6-pin ISP interface on the board.
3. **USBTiny:** A less expensive 3rd-party programmer based on the Amtel AVR processor.
4. **Arduino as ISP:** If you have an extra Arduino lying around you can wire it up as a programmer.

The serial interface requires that an existing bootloader of the correct protocol already exists on the chip. The 
methods that use the 6-pin ISP interface don't require a bootloader.

The program that does flashing is called **avrdude** and can be invoked on the command line or by the Arduino IDE. 
When it communicates with the atmega168 it can use a number of protocols. The state of development regarding the 
protocols is currently a mess and you can find quite a number of questions on the message boards.

1. **stk500v1:** This is a protocol originally used by an older Amtel board that is supported by Auduino and the 
optiboot loader. It is the correct protocol to use in Arduino 0022 (avrdude v5.4) but if you attempt
to use this in Arduino 1.0.3 (avrdude 5.11) you will get a timeout error because it does not reset the port.
Unfortunately Arduino 1.0.3 uses it by default.
2. **stk500v2:** This is a newer protocol required by the Amtel Studio 6.0 IDE. If the Amtel IDE detects a v1 protocol on 
the AVR chip it will prompt the user to upgrade firmware after which the v1 protocol will no longer work. 
3. **arduino**: This is additional protocol available in avrdude 5.11 to deal with the timeout issue in stk500v1 and 
with the exception of a port reset it is essentially the same. This is the correct protocol to use with Arduino 
1.0.3 (avrdude 5.11). 

To summarize, with Arduino 0022 you should use stk500v1 and with Arduino 1.0.3 you should use arduino. You can configure 
the Arduino IDE to use the arduino protocol but unfortunately it still does not work (at least on Windows) because
of the way it formats the port string. The only other option is to invoke avrdude manually or from a Makefile.

A bootloader can be difficult to deal with even without all these issues conspiring against us. One objective of this 
project is to produce a tested and clearly documented process for flashing the Ring board with its customized 
bootloader.





