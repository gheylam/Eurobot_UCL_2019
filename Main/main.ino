#include <Wire.h>
#include <MD25IIC.h>

#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLib_P02_Sony.h>
#include <IRLibCombo.h>

// Physics
#define WD 100
#define CLICKS_PER_REV 347
double circ = PI * WD;


MD25IIC MyBoard;
IRsend mySender;

#define TURN_ON_PIN 30
#define TRANSMITTER_PIN 9
// Check one which side of the map
// Connect pin to ground if distributor on left of PT
#define MAP_SIDE_PIN 22
#define LEFT 0
#define RIGHT 1

//BLUE LED Pins
#define LED_1 24
#define LED_2 26
#define LED_3 28
#define LED_4 29
#define LED_5 32
#define LED_6 34
#define LED_7 36
#define LED_8 38
#define LED_9 40
#define LED_10 42



int distLocation;

//MOTOR CONTROL FUNCTIONS =================================================
unsigned long dist_to_clicks(int dist_mm) {
  double num_revs = dist_mm / circ;
  Serial.print("num_revs: ");
  Serial.println(num_revs);
  unsigned long clicks = round(CLICKS_PER_REV * num_revs);
  return clicks;
}

void travel_dist(int speed_set, int dist_mm) {
  MyBoard.resetEncoders();
  unsigned long clicks = dist_to_clicks(dist_mm);
  Serial.print("clicks: ");
  Serial.println(clicks);
  set_speed_forward(speed_set+8, speed_set);
  while (abs(MyBoard.getMotor1Encoder()) < clicks) {
    Serial.println(MyBoard.getMotor1Encoder());
  }
  Stop();
}

void turn_90_left() {
  set_speed_forward(100, -128);
  delay(1350);
  Stop();
}

void turn_180_left() {
  set_speed_forward(100, -128);
  delay(3200);
  Stop();
}

void turn_360_left() {
  set_speed_forward(100, -128);
  delay(6900);
  Stop();
}

void turn_180_right(){
  set_speed_forward(-128, 100);
  delay(5000);
  Stop();
}

void turn_360_right() {
  set_speed_forward(-128, 100);
  delay(10000);
  Stop();
}

void pivot_180() {
  set_speed_forward(100, -100);
  delay(1800);
  Stop();
}

void Stop() {
  MyBoard.setMotor1Speed(128);
  MyBoard.setMotor2Speed(128);
}
void set_speed_forward(int speed_left, int speed_right) {

  //Make left spin first
  MyBoard.setMotor1Speed(-speed_left);
  MyBoard.setMotor2Speed(128);
  delay(200);
  MyBoard.setMotor1Speed(-speed_left);
  MyBoard.setMotor2Speed(-(speed_right + 6));
}


// MAIN FUNCTIONS ==========================================================

void sendSignal() {
  //Rev up the LEDs!
  digitalWrite(LED_10, HIGH);
  delay(200);
  digitalWrite(LED_9, HIGH);
  delay(200);
  digitalWrite(LED_8, HIGH);
  delay(200);
  digitalWrite(LED_7, HIGH);
  delay(200);
  digitalWrite(LED_6, HIGH);
  delay(200);
  digitalWrite(LED_5, HIGH);
  delay(200);
  digitalWrite(LED_4, HIGH);
  delay(200);
  digitalWrite(LED_3, HIGH);
  delay(200);
  digitalWrite(LED_2, HIGH);
  delay(200);
  digitalWrite(LED_1, HIGH);
  delay(200);
  mySender.send(SONY, 0xa8bca, 20);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_4, LOW);
  digitalWrite(LED_5, LOW);
  digitalWrite(LED_6, LOW);
  digitalWrite(LED_7, LOW);
  digitalWrite(LED_8, LOW);
  digitalWrite(LED_9, LOW);
  digitalWrite(LED_10, LOW);

}

void setup() {
  pinMode(TURN_ON_PIN, INPUT_PULLUP);
  pinMode(MAP_SIDE_PIN, INPUT_PULLUP);

  //Set up the LED pins
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  pinMode(LED_7, OUTPUT);
  pinMode(LED_8, OUTPUT);
  pinMode(LED_9, OUTPUT);
  pinMode(LED_10, OUTPUT);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_4, LOW);
  digitalWrite(LED_5, LOW);
  digitalWrite(LED_6, LOW);
  digitalWrite(LED_7, LOW);
  digitalWrite(LED_8, LOW);
  digitalWrite(LED_9, LOW);
  digitalWrite(LED_10, LOW);
  // sendSignal();

  while (digitalRead(TURN_ON_PIN) == LOW);

  Serial.begin(9600);

  // MOTOR DRIVER
  MyBoard.enableTimeOut(false);    // Stop motor if no command sent
  MyBoard.enableController(true); // Enable On-board speed controller
  MyBoard.resetEncoders();        // Reset (both) encoders to 0000

  // Check for correct side
  int val = digitalRead(MAP_SIDE_PIN);
  if (val == 0) {
    distLocation = LEFT;
  } else {
    distLocation = RIGHT;
  }

}

char should_i_stop = 'n';

void loop() {
  sendSignal();
  if (should_i_stop != 'y') {
    if (distLocation == LEFT) {
      distOnLeft();
      should_i_stop = 'y';
    } else if (distLocation == RIGHT) {
      distOnRight();
      should_i_stop = 'y';
    }
  }

}


// Distributor on the left
void distOnLeft() {
  turn_180_right();
  travel_dist(100, 600);
  turn_180_right();
  travel_dist(100, 600);

  // turn_360_right();
  // delay(1000);
  //input fire IR signal
  // sendSignal();
  // delay(1000);
  //input fire IR signal
  // sendSignal();
  // pivot_180();
  // delay(1000);
  // //input fire IR signal
  // sendSignal();
  // travel_dist(100, 400);
}

// Distributor on the right
void distOnRight() {
  turn_180_left();
  travel_dist(100, 600);
  turn_180_left();
  travel_dist(100, 600);

  // turn_360_left();
  // delay(1000);
  // //input fire IR signal
  // sendSignal();
  // travel_dist(100, 250);
  // delay(1000);
  // //input fire IR signal
  // sendSignal();
  // pivot_180();
  // delay(1000);
  // //input fire IR signal
  // sendSignal();
  // travel_dist(100, 400);
}
