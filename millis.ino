unsigned long myTime;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time since running: ");
  myTime = millis() / 1000;  // convert milliseconds to seconds
  Serial.print(myTime);
  Serial.println(" seconds"); 
  delay(1000);          
}