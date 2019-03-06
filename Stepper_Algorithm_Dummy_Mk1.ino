#include <Stepper.h>

#define STEPS_PER_REV 200
#define DIAMETER 74



Stepper stepper_left(STEPS_PER_REV, 6, 7, 8, 9);
Stepper stepper_right(STEPS_PER_REV, 10, 11, 12, 13);

void setup() {
  stepper_left.setSpeed(60);
  stepper_right.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
  Move_Forward_X(300);
}

void loop() {
 
}

//function to travel x amount of distane in mm
void Move_Forward_X(int distanceMM) {
  //calculate the circumference
  float circumference = DIAMETER * PI;
  float number_of_revolutions = distanceMM / circumference;
  int number_of_steps = int(number_of_revolutions * STEPS_PER_REV);
  Simultaneous_Move(number_of_steps, number_of_steps);
}

//function to make both steppers travel simultaneously 
void Simultaneous_Move(int left_steps, int right_steps){ 
  //Maximum number of steps required is just one of the wheels 
  int max_step = left_steps; 
  //Save the status of whether the wheel is turning clockwise or counter clockwise.
  int left_direction = 1; 
  int right_direction = 1;
  //If the number of steps is less than 0, then reverse the direction 
  if(left_steps < 0){
    left_direction = -1;  
  }
  if(right_steps < 0){
    right_direction = -1;
  }
  //incrementally move each of the stepper motors
  for(int steps = 0; steps < max_step; steps++){
     stepper_left.step(left_direction); 
     stepper_right.step(right_direction);
  }
}



