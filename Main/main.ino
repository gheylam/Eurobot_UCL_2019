#include <Wire.h>
#include <MD25IIC.h>

#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>    
#include <IRLib_P02_Sony.h>   
#include <IRLibCombo.h>   

// Physics
#define WD 100 
#define CLICKS_PER_REV 347
double circ = PI*WD; 


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

//MOTOR CONTROL FUNCTIONS =================================================
unsigned long dist_to_clicks(int dist_mm){
  double num_revs = dist_mm / circ; 
  Serial.print("num_revs: ");
  Serial.println(num_revs);
  unsigned long clicks = round(CLICKS_PER_REV * num_revs);
  return clicks;
}

void travel_dist(int speed_set, int dist_mm){
   MyBoard.resetEncoders();
   unsigned long clicks = dist_to_clicks(dist_mm); 
   Serial.print("clicks: "); 
   Serial.println(clicks);  
   set_speed_forward(speed_set, speed_set);
   while(abs(MyBoard.getMotor1Encoder()) < clicks){
    Serial.println(MyBoard.getMotor1Encoder()); 
   }
   Stop();
}

void turn_90_left(){
  set_speed_forward(100, -128); 
  delay(1350); 
  Stop();  
}

void turn_180_left(){ 
  set_speed_forward(100, -128); 
  delay(4000); 
  Stop();
}

void turn_360_left(){
  set_speed_forward(100, -128); 
  delay(6600);
  Stop();  
}

void pivot_180(){
	set_speed_forward(100, -100);
	delay(4000); 
	Stop();
}

void set_speed_forward(int speed_left, int speed_right){
  
  //Make left spin first 
  MyBoard.setMotor1Speed(-speed_left);
  MyBoard.setMotor2Speed(128); 
  delay(200); 
  MyBoard.setMotor1Speed(-speed_left);
  MyBoard.setMotor2Speed(-(speed_right+6));
}


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
  //sendSignal();
  //delay(500);
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
   turn_360_left();
    delay(1000);
    travel_dist(100, 250);
    delay(1000);
    //input fire IR signal
	sendSignal();
    pivot_180(); 
    delay(1000);
    travel_dist(100,250);
}
