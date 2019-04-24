void setup(void) {
  Serial.begin(9600);
}

void loop(void) {

  int reading = analogRead(A5);

//  int calculated = (6762/(reading-9))-4;

  Serial.println(reading);

  delay(200);
}
