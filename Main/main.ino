#include <Wire.h>
#include <MD25IIC.h>
#include <Servo.h>

// JESUS
#define SERVO_PIN 3
#define SERVO_DEPLOY 20
#define SERVO_UNDEPLOY 50

// Physics
#define WHEEL_DIAM 50

/*
// Motor Driver
#define MOTOR_PIN_1 11
#define MOTOR_PIN_2 12
*/
MD25IIC MyBoard;
Servo myservo;

/*
// Contact Switches
#define CONTACT_FL_PIN 2  
#define CONTACT_FR_PIN 3
#define CONTACT_BL_PIN 6
#define CONTACT_BR_PIN 7
#define CONTACT_B_PIN 8
*/

// Check one which side of the map
// Connect pin to ground if distributor on left of PT
#define MAP_SIDE_PIN 20 
#define LEFT 0
#define RIGHT 1

#define TURN_ON_PIN 30

enum state_enum {PT_CHAOS, CHAOS_COLLECT, CHAOS_PT, CHAOS_RELEASE, PT_DIST, DIST_JESUS, DIST_COLLECT, DIST_PT, DIST_RELEASE, FINISH};

state_enum PC; //Program Counter (instruction index)


int distLocation;


// MOTOR FUNCTIONS =========================================

//**********************************************************//

int FindRev(int wd, int dist) 
{
    int rev = round((dist/(3.14*wd))*360+1);
    return rev;  
}

//*********************************************************//

void Travel(int wd, int dist)
{ 
    int x = FindRev(wd, dist);
    while(1)
    {
      int Val = MyBoard.getMotor1Encoder();
      if(Val > -x){
          MoveForward();         
      }
      else{
          Stop();
          MyBoard.resetEncoders();
          break; 
      }
      Serial.println(MyBoard.getMotor1Encoder()); // Print data
    }
}

void Reverse(int wd, int dist)
{
    int x = FindRev(wd, dist);
    while(1)
    {
      int Val = MyBoard.getMotor1Encoder();
      if(Val < x){
          MoveBackward();         
      }
      else{
          Stop();
          MyBoard.resetEncoders();
          break; 
      }
      Serial.println(MyBoard.getMotor1Encoder()); // Print data
    }
}

//*********************************************************//

void AngleCW(int wd, int r_len)
{
    unsigned long cir = (0.25)*3.14*r_len;
    int x = round((cir/(3.14*wd))*360+1);
    while(1)
    {
      int Val = MyBoard.getMotor1Encoder();
      if(Val < x)
      {
          RotateCW();
      }
      else
      {
          Stop();
          MyBoard.resetEncoders();
          break;
      }
      Serial.println(MyBoard.getMotor1Encoder());
    }   
}

void AngleCCW(int wd, int r_len)
{
    unsigned long cir = (0.25)*3.14*r_len;
    int x = round((cir/(3.14*wd))*360+1);
    while(1)
    {
      int Val = MyBoard.getMotor1Encoder();
      if(Val > -x)
      {
          RotateCCW();
      }
      else
      {
          Stop();
          MyBoard.resetEncoders();
          break;
      }
      Serial.println(MyBoard.getMotor1Encoder());
    }   
}

//*********************************************************//

void Stop() {
    MyBoard.setMotor1Speed(128);
    MyBoard.setMotor2Speed(128);
}

void MoveForward() {
    MyBoard.setMotor1Speed(120);
    MyBoard.setMotor2Speed(120);
}

void MoveBackward() {
     MyBoard.setMotor1Speed(136);
     MyBoard.setMotor2Speed(136);
}

void RotateCCW() {
     MyBoard.setMotor1Speed(120);
     MyBoard.setMotor2Speed(136);    
}

void RotateCW() {
     MyBoard.setMotor1Speed(136);
     MyBoard.setMotor2Speed(120);    
}

//*********************************************************//



// MOTOR FUNCTION WRAPPER
void RotateLeft() {
  AngleCCW(WHEEL_DIAM, 100);
}

void RotateRight() {
  AngleCW(WHEEL_DIAM, 100);
}



// MAIN FUNCTIONS ==========================================================


void setup() {
  pinMode(TURN_ON_PIN, INPUT_PULLUP);
  pinMode(MAP_SIDE_PIN, INPUT_PULLUP);

  while (digitalRead(TURN_ON_PIN) == LOW){
    delay(50);
  }

  Serial.begin(9600);
  PC = PT_CHAOS;

  // MOTOR DRIVER
  MyBoard.enableTimeOut(false);    // Stop motor if no command sent
  MyBoard.enableController(true); // Enable On-board speed controller
  MyBoard.resetEncoders();        // Reset (both) encoders to 0000 

  //SERVO DEPLOY JESUS
  myservo.attach(SERVO_PIN);
  myservo.write(SERVO_DEPLOY);

  // Check for correct side
  int val = digitalRead(MAP_SIDE_PIN);
  if (val == 0){
    distLocation = LEFT;
  }else{
    distLocation = RIGHT;
  }

}

void loop() {
  Reverse(100,100);
  /*
  if (distLocation == LEFT){
    distOnLeft();
  }else if (distLocation == RIGHT){
    distOnRight();
  }
  */
}

/*
// Distributor on the left
void distOnLeft(){
  switch(PC){
    // 1-2
    case PT_CHAOS:
      // Starting position to CHAOS AREA
      Travel(WHEEL_DIAM, 100); // Towards CHAOS
      RotateRight(90); // Face Pucks
      PC = CHAOS_COLLECT;
      break;
    
    // 2-3
    case CHAOS_COLLECT:
      Travel(WHEEL_DIAM, 100); // Eat Pucks
      RotateRight(90); // Face PT
      PC = CHAOS_PT;
      break;
    
    // 3-4
    case CHAOS_PT:
      Travel(WHEEL_DIAM, 100); // Return Pucks Home
      RotateRight(90); 
      Travel(WHEEL_DIAM, 100);
      PC = CHAOS_RELEASE;
      break;

    // 4-5
    case CHAOS_RELEASE:
      Reverse(WHEEL_DIAM, 100);; //Spit out Pucks    
      PC = PT_DIST;
      break;

    // 5-6
    case PT_DIST:
      RotateRight(180); 
      Travel(WHEEL_DIAM, 100);
      RotateLeft(90);
      Travel(WHEEL_DIAM, 100);
      RotateLeft(90);
      PC = DIST_JESUS;
      break;

    // 6-7
    case DIST_JESUS:
      Travel(WHEEL_DIAM, 100);
      RotateRight(180);
      PC = DIST_COLLECT;
      break;

    // 7-8
    case DIST_COLLECT:
      Travel(WHEEL_DIAM, 100);
      RotateRight(90);
      PC = DIST_PT;
      break;

    // 8-9
    case DIST_PT:
      Travel(WHEEL_DIAM, 100);
      RotateRight(90);
      PC = DIST_RELEASE;
      break;

    // 9-10
    case DIST_RELEASE:
      Reverse(WHEEL_DIAM, 100);;
      PC = FINISH;
      break;

    case FINISH:
      delay(100);
      break;

    default: 
      Serial.println("WE DONE F*CKED UP");    
      break;
  }
}

// Distributor on the right
void distOnRight(){
  switch(PC){
    // 1-2
    case PT_CHAOS:
      // Starting position to CHAOS AREA
      Travel(WHEEL_DIAM, 100); // Towards CHAOS
      RotateLeft(90); // Face Pucks
      PC = CHAOS_COLLECT;
      break;
    
    // 2-3
    case CHAOS_COLLECT:
      Travel(WHEEL_DIAM, 100); // Eat Pucks
      RotateLeft(90); // Face PT
      PC = CHAOS_PT;
      break;
    
    // 3-4
    case CHAOS_PT:
      Travel(WHEEL_DIAM, 100); // Return Pucks Home
      RotateLeft(90); 
      Travel(WHEEL_DIAM, 100);
      PC = CHAOS_RELEASE;
      break;

    // 4-5
    case CHAOS_RELEASE:
      Reverse(WHEEL_DIAM, 100);; //Spit out Pucks    
      PC = PT_DIST;
      break;

    // 5-6
    case PT_DIST:
      RotateLeft(180); 
      Travel(WHEEL_DIAM, 100);
      RotateRight(90);
      Travel(WHEEL_DIAM, 100);
      RotateRight(90);
      PC = DIST_JESUS;
      break;

    // 6-7
    case DIST_JESUS:
      Travel(WHEEL_DIAM, 100);
      RotateLeft(180);
      PC = DIST_COLLECT;
      break;

    // 7-8
    case DIST_COLLECT:
      Travel(WHEEL_DIAM, 100);
      RotateLeft(90);
      PC = DIST_PT;
      break;

    // 8-9
    case DIST_PT:
      Travel(WHEEL_DIAM, 100);
      RotateLeft(90);
      PC = DIST_RELEASE;
      break;

    // 9-10
    case DIST_RELEASE:
      Reverse(WHEEL_DIAM, 100);;
      PC = FINISH;
      break;

    case FINISH:
      delay(100);
      break;

    default: 
      Serial.println("WE DONE F*CKED UP");    
      break;
  }
}
*/