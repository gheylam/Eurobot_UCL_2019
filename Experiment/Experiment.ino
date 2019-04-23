#include <IRremote.h>

#define CONTACT_END 4
#define MOTOR_PIN 6


const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup(){
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(CONTACT_END, INPUT_PULLUP);
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  digitalWrite(MOTOR_PIN, LOW);

  
  while(!irrecv.decode(&results));
  Serial.println(results.value, HEX);
  irrecv.resume();
  digitalWrite(MOTOR_PIN, HIGH);
}

void loop(){
  // NOTHING
  if(digitalRead(CONTACT_END)==0){
    digitalWrite(MOTOR_PIN, LOW);
  }
}
