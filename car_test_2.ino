
#include <Servo.h>

const int echo = 8;
const int trigger = 9;

const int motorleft_en = 5;
const int motorright_en = 6;
const int motorleft_in1 = 1;
const int motorleft_in2 = 2;
const int motorright_in1 = 3;
const int motorright_in2 = 4;

const int motorspeed = 110;      
const int servo_pin = A1;
const int ir_sensor = A4;

Servo myservo;

void setup() 
{
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ir_sensor, INPUT);

  pinMode(motorleft_in1, OUTPUT);
  pinMode(motorleft_in2, OUTPUT);
  pinMode(motorright_in1, OUTPUT);
  pinMode(motorright_in2, OUTPUT);
  pinMode(motorleft_en, OUTPUT);
  pinMode(motorright_en, OUTPUT);

  myservo.attach(servo_pin);
  myservo.write(90);
  delay(500);
  
  Serial.begin(9600);
}

void loop()
{

  // finish line reached

  if (digitalRead(ir_sensor) == HIGH) {
    stopmotors();
    //while (1); // forever loop
    delay(10000);
  }
 
  myservo.write(90);        // Always look straight ahead
  delay(500);                 
  int distance = getdistance();

 // while (distance > 20) {  moveforward(); int distance = getdistance();}
  
  stuck(distance); 

  if (distance > 30)
  {
    moveforward();
  }
  //if (distance < 20)
  else
  {                    
    stopmotors();
    delay(100);

    // Check left (180) and right (0)
    int distleft  = measure(160);
    delay(200);
    int distright = measure(35);
    delay(200);
    int distfront = measure(90);
    delay(200);
    myservo.write(90);                     // Look forward again
    delay(200);

    if (distfront < 9)
    {
      reverse();
    } // sometimes there is no object on the sides, but if there is a object in the from the car is unalbe to take a turn. this if statement is to counter that
    if (distleft >= distright && distleft > 25) {
      turnleft();                          // Keep turning left until front is clear
    }
    else if (distright > 25) {
      turnright();               // Keep turning right until front is clear
    }
    else {
      // Both sides blocked
      reverse();
    }
  } 
 // else 
  {                                   
    //moveforward();   
  }
}

// ──────────────────────────────  FUNCTIONS ──────────────────────────────

int getdistance()
 {
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
 // long duration = pulseIn(echo, HIGH, 30000); // 30000 - timeout value in microseconds ~50cm
  long duration = pulseIn(echo, HIGH);
  return (duration * 0.034 / 2);
}

int measure(int angle) 
{
  myservo.write(angle);
  delay(200);                     // Give servo time to reach position
  return getdistance();
}

void moveforward()
{
  digitalWrite(motorleft_in1, HIGH);
  digitalWrite(motorleft_in2, LOW);
  digitalWrite(motorright_in1, HIGH);
  digitalWrite(motorright_in2, LOW);
  analogWrite(motorleft_en, motorspeed);
  analogWrite(motorright_en, motorspeed);
}

void stopmotors()
 {
  analogWrite(motorleft_en, 0);
  analogWrite(motorright_en, 0);
}

// Keeps turning left  until the front is clear 
void turnleft() 
{
    myservo.write(90);
    delay(200); 
    while (true) 
    {
        digitalWrite(motorleft_in1, LOW);
        digitalWrite(motorleft_in2, HIGH);
        digitalWrite(motorright_in1, HIGH);
        digitalWrite(motorright_in2, LOW);
        analogWrite(motorleft_en, motorspeed - 20);   // Left wheel slower and reverse
        analogWrite(motorright_en, motorspeed);

    
    if (getdistance() > 30) 
    {
      delay(8);
      stopmotors();
      delay(200);
      break;  
    }
  }
}

// Keeps turning right until the front is clear
void turnright() 
{
  myservo.write(90);
  delay(200);  

  digitalWrite(motorleft_in1, HIGH);
  digitalWrite(motorleft_in2, LOW);
  digitalWrite(motorright_in1, LOW);
  digitalWrite(motorright_in2, HIGH);
  analogWrite(motorleft_en, motorspeed);
  analogWrite(motorright_en, motorspeed - 20);

  while (true) 
  {
    if (getdistance() > 30) {
    delay(5);    // the sensor is at a height and does not cover the entire bumper... this delay turns the car a little more to account for bumper's width
    stopmotors();
    delay(200);
    break;
  }
  
  }
  
}

void reverse() 
{
  digitalWrite(motorleft_in1, LOW);
  digitalWrite(motorleft_in2, HIGH);
  digitalWrite(motorright_in1, LOW);
  digitalWrite(motorright_in2, HIGH);
  analogWrite(motorleft_en, motorspeed );
  analogWrite(motorright_en, motorspeed );
  delay(850);
  stopmotors();
}

unsigned long timer = 0;
int lastdistance = 0;

void stuck(int currentdistance)
{
  // If distance hasn't changed much
  if (abs(currentdistance - lastdistance) <= 3)
  {
    if (timer == 0)
    {
      timer = millis();  // start timer
    }
    else if (millis() - timer >= 5000) // 5000 miliseconds ~ 5 seconds
    {
      // car is stuck
      stopmotors();
      delay(200);
      reverse();   // reverse 
      delay(200);
      timer = 0;
    }
  }
  else
  {
    // not stuck
    timer = 0;
  }
  lastdistance = currentdistance;
}