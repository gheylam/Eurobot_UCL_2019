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

void setup()
{
    Serial.begin(115200);           // Set Serial Port to 115200 bps
    MyBoard.enableTimeOut(false);    // Stop motor if no command sent
    MyBoard.enableController(true); // Enable On-board speed controller
    MyBoard.resetEncoders();        // Reset (both) encoders to 0000 
}    

void loop() 
{ 
    MoveForward();
  
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

