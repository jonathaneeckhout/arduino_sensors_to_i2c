# arduino_sensors_to_i2c
Arduino module that translates digital and analog inputs to the i2c protocol

## Getting Started

This project will let you read digital and analog inputs and translates them to the i2c protocol. This i2c signal can be used for example on a raspberry pi.

### Prerequisites

The following items are needed for this project

* [Arduino ide](https://www.arduino.cc/) - The programmer used to programm the arduino mega 2560
* arduino mega 2560 - the microcontroller. A different one can be used but then you need to modify the connections to the board and pin numbers in the code.
* 2 buttons + 2 resistors - the digital inputs.
* Joystick - the analog inputs (a joystick is just a combination of 2 potentiometers. One for the x axis and one for the y axis).

## Deployment

Connect the sensor as shown on the image

![Arduino pin connection](https://raw.githubusercontent.com/jonathaneeckhout/arduino_sensors_to_i2c/master/i2c_sensors_arduino_bb.jpg)


## Authors

* **Jonathan Eeckhout** - *Initial work* - [jonathaneeckhout](https://github.com/jonathaneeckhout)

## Acknowledgments

* Arduino for the example projects


