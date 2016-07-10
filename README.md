# Raspberry PI Servo Motor Control

Raspberry PI servo motor control for a pan-tilt system using an AVR atmega328 for accurate PWM generation.  The raspi communications with the AVR using the SPI bus/protocol using a simple delimited message scheme.  The raspberry pi control segment uses the SPI-Py python bindings for communication on the spidev device.

The timing critical PWM is generated on the atmega328.  A SPI slave with buffering and newline termination is fed into a processing block.  

![raspi-servos](https://raw.githubusercontent.com/shraken/raspi-servos/shraken_image/raspi-servos.jpg)

## Building

cd/SPI-Py
sudo python ./setup.py build install

## Flashing

Use Arduino IDE or avrdude to flash arduino firmware.

## Using It

A graphical interface is provided using python/tk:
python ./tk_slider_spi.py
