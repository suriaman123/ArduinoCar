
int motorleft_in1 = 1;
int motorleft_in2 = 2;
int motorright_in1 = 3;
int motorright_in2 = 4;

int motorleft_en = 5; 
int motorright_en = 6;

void setup() {

  pinMode(motorleft_in1, OUTPUT);
  pinMode(motorleft_in2, OUTPUT);
  pinMode(motorright_in1, OUTPUT);
  pinMode(motorright_in2, OUTPUT);
}

void loop() {

  digitalWrite(motorleft_in1, HIGH);
  digitalWrite(motorleft_in2, LOW);

  digitalWrite(motorright_in1, LOW);
  digitalWrite(motorright_in2, HIGH);

  digitalWrite(motorleft_en, 50);
  digitalWrite(motorright_en, 50);

  delay(10000);  

  stopCar();
  delay(500); 
}

stopCar() 
{
  digitalWrite(motorleft_in1, LOW);
  digitalWrite(motorleft_in2, LOW);
  digitalWrite(motorright_in1, LOW);
  digitalWrite(motorright_in2, LOW);
}