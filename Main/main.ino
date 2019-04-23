#include <Wire.h>
#include <MD25IIC.h>

#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>    
#include <IRLib_P02_Sony.h>   
#include <IRLibCombo.h>   

// Physics
#define WHEEL_DIAM 50

MD25IIC MyBoard;
IRsend mySender;

#define TURN_ON_PIN 30
#define TRANSMITTER_PIN 9
// Check one which side of the map
// Connect pin to ground if distributor on left of PT
#define MAP_SIDE_PIN 22 
#define LEFT 0
#define RIGHT 1


int distLocation;


// MAIN FUNCTIONS ==========================================================

void sendSignal(){
  mySender.send(SONY,0xa8bca, 20);
}

void setup() {
  pinMode(TURN_ON_PIN, INPUT_PULLUP);
  pinMode(MAP_SIDE_PIN, INPUT_PULLUP);

  while (digitalRead(TURN_ON_PIN) == LOW);

  Serial.begin(9600);

  // MOTOR DRIVER
  MyBoard.enableTimeOut(false);    // Stop motor if no command sent
  MyBoard.enableController(true); // Enable On-board speed controller
  MyBoard.resetEncoders();        // Reset (both) encoders to 0000 

  // Check for correct side
  int val = digitalRead(MAP_SIDE_PIN);
  if (val == 0){
    distLocation = LEFT;
  }else{
    distLocation = RIGHT;
  }

}

void loop() {
  sendSignal();
  delay(500);
  if (distLocation == LEFT){
    distOnLeft();
  
  
  }else if (distLocation == RIGHT){
    distOnRight();
  
  }
  
}


// Distributor on the left
void distOnLeft(){
}

// Distributor on the right
void distOnRight(){
}
