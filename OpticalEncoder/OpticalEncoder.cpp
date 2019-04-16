//Library for optical encoders

#include "Arduino.h"
#include "OpticalEncoder.h"

OpticalEncoder :: OpticalEncoder(int pin){
  pinMode(pin, INPUT); 
  _pin = pin;
}

int OpticalEncoder :: getValue(){
  int value = analogRead(_pin);
  if(value > 1022){
    return 1; 
  }else{
    return 0;
  }
}

int OpticalEncoder :: getRawValue(){ 
	int value = analogRead(_pin); 
	return value;
}



