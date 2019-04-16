//Header file for the optical encoder
//Functions to simply return a 0
//Only works for resistor combination 220Ohms and 10KOhms

#include "Arduino.h"


class OpticalEncoder {
  public:
    OpticalEncoder (int pin);
    int getValue();
	int getRawValue();

  private:
    int _pin;
};
