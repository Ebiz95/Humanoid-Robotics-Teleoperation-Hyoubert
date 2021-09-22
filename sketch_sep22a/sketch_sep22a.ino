#include <Servo.h>

Servo myservo;
int input;
void setup()
{
  myservo.attach(10);
}

void loop() {
  myservo.write(1);
  delay(1000);
  
  myservo.write(10);
  delay(1000);
  
  myservo.write(20);
  delay(1000);
  
  myservo.write(30);
  delay(1000);

  myservo.write(20);
  delay(1000);
  
  myservo.write(10);
  delay(1000);
}
