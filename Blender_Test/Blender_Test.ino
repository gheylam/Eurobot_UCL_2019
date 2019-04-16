#include "OpticalEncoder.h"

int encoder_pin_1 = A0;
int encoder_pin_2 = A1;

OpticalEncoder left_Encoder = OpticalEncoder(encoder_pin_1);
OpticalEncoder right_Encoder = OpticalEncoder(encoder_pin_2);

#define lmotor_pin1 11
#define lmotor_pin2 10
#define rmotor_pin1 9
#define rmotor_pin2 6

unsigned long left_prev_time = 0;
unsigned long right_prev_time = 0;

#define INTERVAL_PADDING 10
#define LEFT_SPEED 255
#define RIGHT_SPEED 255

void setup() {
  // put your setup code here, to run once:
  pinMode(lmotor_pin1, OUTPUT);
  pinMode(lmotor_pin2, OUTPUT);

  pinMode(rmotor_pin1, OUTPUT);
  pinMode(rmotor_pin2, OUTPUT); 

  analogWrite(lmotor_pin1, LEFT_SPEED);
  analogWrite(lmotor_pin2, 0);
  
  analogWrite(rmotor_pin1, RIGHT_SPEED);
  analogWrite(rmotor_pin2, 0);

}

void loop() {
  //check if the interval for the two motors are roughly the sam e

  //get the first interval
  unsigned long first_interval = motor_interval(left_Encoder, left_prev_time);
  unsigned long second_interval = motor_interval(right_Encoder, right_prev_time);
  if (!first_interval) {
    left_prev_time = first_interval;
  }
  if (!second_interval) {
    right_prev_time = second_interval;
  }
  motor_sync(first_interval, second_interval);

}

unsigned long motor_interval(OpticalEncoder motor_encoder, unsigned long last_intercept) {
  unsigned long newest_intercept;
  //this function handles the logic on how and find the interval between each blade
  if (motor_encoder.getValue()) {
    while (motor_encoder.getValue()) {
      //when we are still getting a 1 signal, constantly update the time
    }
    newest_intercept = millis();
    //when we leave the signal, we will have the most recent update of the last time we got a 1
    //Then we calculate the interval between the last positive signal and this one
    return (newest_intercept - last_intercept);
  }
  return 0;
}


void motor_sync(unsigned long left_interval, unsigned long right_interval) {
  if (left_interval > right_interval) {
    //since left motor is faster, we need to pause the left one for the amount of time difference
    unsigned long finish_time = (left_interval - right_interval) + millis();
    //shut down the left motor
    shutdown_blender(lmotor_pin1, lmotor_pin2);
    while (millis() < finish_time) {
    }
    //restart the blender
    start_left_blender();
  } else {
    //else the right motor is faster, we need to pause the right one for the amount of time difference
    unsigned long finish_time = (right_interval - left_interval) + millis();
    //shut down the right motor
    shutdown_blender(rmotor_pin1, rmotor_pin2);
    while (millis() < finish_time) {
    }
    start_right_blender();
  }
}

void shutdown_blender(int motor_pin1, int motor_pin2) {
  digitalWrite(motor_pin1, LOW);
  digitalWrite(motor_pin2, LOW);
}

void start_left_blender(void) {
  analogWrite(lmotor_pin1, LEFT_SPEED);
  analogWrite(lmotor_pin2, 0);
}

void start_right_blender(void) {
  analogWrite(rmotor_pin1, RIGHT_SPEED);
  analogWrite(rmotor_pin2, 0);
}
