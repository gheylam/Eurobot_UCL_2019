/***************************************************************
// MOTOR CONTROL WITH MD-25 DRIVER BOARD
//**************************************************************/
#include <Wire.h>
#include <MD25IIC.h>
// MD25IIC METHODS:
//    -----------------------   ------------------------
//   |       MOTOR          |  | BOARD                 |
// --------------------------  |-----------------------|
// 1 | setMotor1Speed(byte) |  | getAddress()          |
//   | getMotor1Encoder()   |  | setMode(byte)         |
//   | getMotor1Current()   |  | getBattery()          |
// -------------------------|  | enableTimeOut(T/F)    |
// 2 | setMotor2Speed(byte) |  | enableController(T/F) |
//   | getMotor2Encoder()   |  | resetEncoders()       |
//   | getMotor2Current()   |  | setAcceleration(byte) |
// -------------------------|  |-----------------------|

MD25IIC MyBoard;
/***************************************************************
// Setup function
//**************************************************************/

#define WD 100 
#define CLICKS_PER_REV 347
double circ = PI*WD; 

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
  delay(3300); 
  Stop();
}

void turn_360_left(){
  set_speed_forward(100, -128); 
  delay(6600);
  Stop();  
}


void pivot_180(){
  set_speed_forward(100, -100); 
  delay(1800); 
  Stop();
}



void accelerate(int speed_left, int speed_right){
  for(int increment = 0; increment < 11; increment++){
    int speed_left_val = map(increment, 0, 10, 0, speed_left); 
    int speed_right_val = map(increment, 0, 10, 0, speed_right); 
    set_speed_forward(speed_left_val, speed_right_val);
    delay(50);
  }
}

void set_speed_forward(int speed_left, int speed_right){
  
  //Make left spin first 
  MyBoard.setMotor1Speed(-speed_left);
  MyBoard.setMotor2Speed(128); 
  delay(200); 
  MyBoard.setMotor1Speed(-speed_left);
  MyBoard.setMotor2Speed(-(speed_right+6));
}


void setup()
{
    Serial.begin(115200);           // Set Serial Port to 115200 bps
    MyBoard.enableTimeOut(false);    // Stop motor if no command sent
    MyBoard.enableController(true); // Enable On-board speed controller
    MyBoard.resetEncoders();        // Reset (both) encoders to 0000 
    //travel_dist(50, 1000);
    //left motor 
    //set_speed_forward(60, 60);
    
}

void loop() 
{ 
//  MoveForward();
    pivot_180(); 
    delay(1000);
}




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

void Stop() 
{
    MyBoard.setMotor1Speed(128);
    MyBoard.setMotor2Speed(128);
}

void MoveForward() 
{
    MyBoard.setMotor1Speed(120);
    MyBoard.setMotor2Speed(120);
}

void MoveBackward() 
{
     MyBoard.setMotor1Speed(136);
     MyBoard.setMotor2Speed(136);
}

void RotateCCW() 
{
     MyBoard.setMotor1Speed(120);
     MyBoard.setMotor2Speed(136);    
}

void RotateCW() 
{
     MyBoard.setMotor1Speed(136);
     MyBoard.setMotor2Speed(120);    
}

//*********************************************************//
