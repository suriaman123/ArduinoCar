// Car test 1

#include <Servo.h>

// ----- Pins -----

const int echo = 8;
const int trigger = 9;

const int motorleft_en = 5;
const int motorright_en = 6;
const int motorleft_in1 = 1;
const int motorleft_in2 = 2;
const int motorright_in1 = 3;
const int motorright_in2 = 4;
const int motorspeed = 115;

Servo servomotor;

// ----- Variables -----
long duration;
int distance;

// Function to measure distance with ultrasonic sensor
int getdistance() {

  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  int d = duration * 0.034 / 2;        // convert to cm
  return d;
}

// ----- Motor functions -----
void moveforward() {
  analogWrite(motorleft_en, motorspeed);    
  analogWrite(motorright_en, motorspeed); 

  digitalWrite(motorleft_in1, HIGH);
  digitalWrite(motorleft_in2, LOW);
  digitalWrite(motorright_in1, HIGH);
  digitalWrite(motorright_in2, LOW);
}

void movebackward() {
  analogWrite(motorleft_en, motorspeed);    
  analogWrite(motorright_en, motorspeed);

  digitalWrite(motorleft_in1, LOW);
  digitalWrite(motorleft_in2, HIGH);
  digitalWrite(motorright_in1, LOW);
  digitalWrite(motorright_in2, HIGH);
}

void stopcar() {
  digitalWrite(motorleft_en, 0);    
  digitalWrite(motorright_en, 0);

  digitalWrite(motorleft_in1, LOW);
  digitalWrite(motorleft_in2, LOW);
  digitalWrite(motorright_in1, LOW);
  digitalWrite(motorright_in2, LOW);
}

void turnleft() {
  analogWrite(motorleft_en, motorspeed);    
  analogWrite(motorright_en, motorspeed - 20);

  digitalWrite(motorleft_in1, LOW);
  digitalWrite(motorleft_in2, HIGH);   // left wheel moves backward
  digitalWrite(motorright_in1, HIGH);   // right wheel moves forward
  digitalWrite(motorright_in2, LOW);

  //delay(1000); 
 // stopcar();
}

void turnright() {
  analogWrite(motorleft_en, motorspeed);    
  analogWrite(motorright_en, motorspeed - 20);

  digitalWrite(motorleft_in1, HIGH);    // left wheel forward
  digitalWrite(motorleft_in2, LOW);
  digitalWrite(motorright_in1, LOW);
  digitalWrite(motorright_in2, HIGH);  // right wheel backward

//  delay(1000); 
 // stopCar();
}


// ---------------SETUP-----------------
void setup() 
{
  Serial.begin(9600);

  pinMode(A4, INPUT); // IR sensor

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
  

  pinMode(motorleft_in1, OUTPUT);
  pinMode(motorleft_in2, OUTPUT);
  pinMode(motorright_in1, OUTPUT);
  pinMode(motorright_in2, OUTPUT);
  pinMode(motorleft_en, OUTPUT);  
  pinMode(motorright_en, OUTPUT);  

  servomotor.attach(A1);  // servo on pin 1
  servomotor.write(90);  // face forward
  delay(1000);           
}

// --------------LOOP------------------

void loop() 
{
  servomotor.write(90); // Look forward
 // analogWrite(motorleft_en, motorspeed);
 // analogWrite(motorright_en, motorspeed);
  delay(500);


  distance = getdistance();
  Serial.print("Front distance: ");
  Serial.println(distance);

  if (distance > 30) {
    // Clear path 
    moveforward();
  } else {
    // Obstacle detected
    stopcar();
    delay(100);

    // Look left
    servomotor.write(150);
    delay(100);                   // 20 delay too low
    int leftdist = getdistance();
    delay(100);
    // Look right
    servomotor.write(30);
    delay(100);
    int rightdist = getdistance();
  //  int rightdist = 50;
   
    servomotor.write(90);
    delay(100);

    // Decide direction
    if (leftdist >= rightdist)
    {
      Serial.println("Turning left");
      turnleft();
      delay(50);
    } 
    else if ( rightdist > leftdist )
    {
      Serial.println("Turning right");
      turnright();
      delay(50);
    }
    else
    {
     // stopcar();
      movebackward();
      delay(200);
    }

  }

  delay(100);
}
