#include <IRremote.h>

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup(){
  pinMode(6, OUTPUT);

  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  digitalWrite(6, LOW);

  
  while(!irrecv.decode(&results));
  Serial.println(results.value, HEX);
  irrecv.resume();
  digitalWrite(6, HIGH);
}

void loop(){
  // NOTHING
}
