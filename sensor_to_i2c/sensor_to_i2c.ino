#define ANALOG_INPUTS 0x2 //Number of analog inputs
#define DIGITAL_INPUTS 0x2 //Number of ditital inputs
#define PAYLOAD_SIZE 0x2 + ANALOG_INPUTS + DIGITAL_INPUTS //first 2 bytes define the length of the other arrays
/*
 * Payload structure:
 * =======================
 * Byte 0: ANALOG_INPUTS -> defines how many analog fields there are
 * Byte 1: DIGITAL_INPUTS -> defines how many digital fields there are
 * Byte (2 --> 2 + ANALOG_INPUTS): analogOutValue
 * Byte ((2 + ANALOG_INPUTS) --> (2 + ANALOG_INPUTS + DIGITAL_INPUTS): digitalOutValue
 */

#include <Wire.h>

//Bufffer with all the analog input pins
const uint16_t analogIn[ANALOG_INPUTS] = {
  A0,
  A1
};

//Buffer with all the digital input pins
const uint16_t digitalIn[DIGITAL_INPUTS] = {
  0x34, //pin 52
  0x35 //pin 53
};


uint16_t analogReadValue[ANALOG_INPUTS] = {
  0x00,
  0x00
};

uint8_t analogOutValue[ANALOG_INPUTS] = {
  0x0,
  0x0
};

uint8_t digitalReadValue[DIGITAL_INPUTS] = {
  0x0,
  0x0
};

uint8_t digitalOutValue[DIGITAL_INPUTS] = {
  0x0,
  0x0
};

uint8_t lastDigitalOutValue[DIGITAL_INPUTS] = {
  HIGH,
  HIGH
};

unsigned long lastDebounceTimeButtons[DIGITAL_INPUTS] {
  0,
  0
};

//Debounce time in ms
const unsigned long debounceDelay = 50;

void initDigitalPins() {
  int i = 0;
  for (i = 0; i < DIGITAL_INPUTS; i++) {
    pinMode(digitalIn[i], INPUT);
  }
}

void setup() {
  initDigitalPins();

  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void readAllInputs() {
  int i = 0;

  // read all the analog inputs and convert them to a 8 bit value
  for (i = 0; i < ANALOG_INPUTS; i++) {
    analogReadValue[i] = analogRead(analogIn[i]);
    analogOutValue[i] = map(analogReadValue[i], 0, 1023, 0, 255);
  }

  // read all the digital inputs and debounce them, once set the output is only reset after a i2c read
  for (i=0; i < DIGITAL_INPUTS; i++) {
    digitalReadValue[i] = digitalRead(digitalIn[i]);
    if(digitalReadValue[i] != lastDigitalOutValue[i]) {
      lastDebounceTimeButtons[i] = millis();
    }
    if ((millis() - lastDebounceTimeButtons[i]) > debounceDelay) {
      // Low is high
      if (digitalReadValue[i] == LOW) {
        digitalOutValue[i] = 0x1;
      }
    }
    lastDigitalOutValue[i] = digitalReadValue[i];
  }
}

void loop() {

  readAllInputs();  

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}

void requestEvent() {
  char buff[PAYLOAD_SIZE];
  int i = 0;
  for (i=0;i<PAYLOAD_SIZE;i++) {
    buff[i] = 0x0;
  }
  buff[0] = ANALOG_INPUTS;
  buff[1] = DIGITAL_INPUTS;

  for (i=0;i<ANALOG_INPUTS;i++) {
    // put the current analog value into the buffer
    buff[i+2] = analogOutValue[i];
  }

  for (i=0;i<DIGITAL_INPUTS;i++) {
    // put the current digital value into the buffer
    buff[i+2+ANALOG_INPUTS] =digitalOutValue[i];
    // reset the value after it has been read
    digitalOutValue[i] = 0x0;
  }
  
  Wire.write(buff, PAYLOAD_SIZE);
}
