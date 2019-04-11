//Declaration of pins used
const int contactSwitch1 = 2;
const int contactSwitch2 = 3;
const int leftMotor1 = 10;
const int leftMotor2 = 11;
const int rightMotor1 = 6;
const int rightMotor2 = 9;

int switchState1; // 0 closed switch 1 open switch
int switchState2; // 0 closed switch 1 open switch

unsigned long previousMillis = 0;
unsigned long currentMillis = 1;

int motor_analog1 = analogRead(A0); //left motor controller
int motor_analog2 = analogRead(A1); //right motor controller
int motor_sp1 = map(motor_analog1, 0, 1023, 0, 255);
int motor_sp2 = map(motor_analog2, 0, 1023, 0, 255);

//Different states of the lift
int initialState = 0;
int fillFirstTray = 1;
int emptyFirstTray = 2;
int emptySecondTray = 3;

int currentState = fillFirstTray;

//Interval ms lengths for each function
int emptyFirstTrayInterval = 1300;
int initialStateInterval = 1300;
int fillFirstTrayInterval = 1300;
int emptySecondTrayInterval = 1300;
//Next interval holds the amount of time required for the state. the time is unique to the state
int nextInterval = 0;

//Declaration of functions used
void move_up();
void move_down();
void brake();
void setState();
void checkState();
void fillFirstTrayFunction();
void emptyFirstTrayFunction();
void emptySecondTrayFunction();
void initialStateFunction();

void setup()
{
  pinMode(contactSwitch1, INPUT_PULLUP);
  pinMode(contactSwitch2, INPUT_PULLUP);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  switchState1 = digitalRead(contactSwitch1);
  switchState2 = digitalRead(contactSwitch2);
  Serial.println(currentState);
  Serial.println(currentMillis);
  currentMillis = millis();
  if ((currentMillis - previousMillis) > nextInterval) {
    Serial.println("test: ");
    Serial.println(currentMillis);
    checkState();
    //previousMillis = currentMillis;
  }
  delay(2000);
}

//move up motor function
void move_up(int motor_speed1, int motor_speed2) {
  analogWrite(leftMotor1, motor_speed1);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  analogWrite(rightMotor2, motor_speed2);
}

//move down motor function
void move_down(int motor_speed1, int motor_speed2) {
  digitalWrite(leftMotor1, LOW);
  analogWrite(leftMotor2, motor_speed1);
  analogWrite(rightMotor1, motor_speed2);
  digitalWrite(rightMotor2, LOW);
}

void brake() {
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
}

/*
  void setState(){
  //Initial State of the lift is to fill the second tray
  //First line of the if statements is psuedo-code as the state for a full first tray is unknown
  if (secondTray == full){
    currentState = fillFirstTray;
    }

  if (atWeighingScale == True){
    currentState = emptyFirstTray;
    }

  if (firstTray == empty){
    currentState == emptySecondTray;
    }

  if (secondTray == empty){
    currentState == initialState;
    }
  }
*/

void checkState() {
  if (currentState == fillFirstTray) {
    Serial.println("Before function");
    fillFirstTrayFunction();
    Serial.println("After function");
    currentState = emptyFirstTray;
    Serial.println("Current State after function: ");
    Serial.println(currentState);
    nextInterval = emptyFirstTrayInterval;
  }

  if (currentState == emptyFirstTray) {
    emptyFirstTrayFunction();
    currentState = emptySecondTray;
    nextInterval = emptySecondTrayInterval;
  }

  if (currentState == emptySecondTray) {
    emptySecondTrayFunction();
    currentState = initialState;
    nextInterval = initialStateInterval;
  }

  if (currentState == initialState) {
    initialStateFunction();
    currentState = fillFirstTray;
    nextInterval = fillFirstTrayInterval;
  }
  
}

//fillFirstTrayFunction moves the lift down from initialState
//Does not use a time interval. Stops the motors when bottom contact switch is hit
void fillFirstTrayFunction() {
  Serial.println("Within function");
  if (switchState1 == LOW) {
    previousMillis = currentMillis;
    brake();
  }
  else {
    Serial.println("Within function else statement");
    move_up(motor_sp1, motor_sp2);
  }
}

//emptyFirstTrayFunction move the lift up from fillFirstTray State
void emptyFirstTrayFunction() {
  if (currentMillis - previousMillis >= emptyFirstTrayInterval) {
    previousMillis = currentMillis;
    brake();
  }
  else {
    move_up(motor_sp1, motor_sp2);
  }
}

//emptySecondTrayFunction moves the lift up from emptyFirstTray State
//This does not use a time interval but stops the motors when the top contact switch is hit
void emptySecondTrayFunction() {
  if (switchState2 == LOW) {
    previousMillis = currentMillis;
    brake();
  }
  else {
    move_up(motor_sp1, motor_sp2);
  }
}

//initialStateFunction moves the lift down from emptySecondTray State
void initialStateFunction() {
  if (currentMillis - previousMillis >= initialStateInterval) {
    previousMillis = currentMillis;
    brake();
  }
  else {
    move_down(motor_sp1, motor_sp2);
  }
}
