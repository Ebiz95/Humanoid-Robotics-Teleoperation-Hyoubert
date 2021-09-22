#include <Servo.h> 
Servo elbow;

int maxI = 2250;
int minI = 850;
int midI = maxI - round((maxI-minI)/2);
int speedDelay = 10;
  
void setup() 
{ 
  Serial.print(maxI);
  Serial.print(minI);
  Serial.print(midI);
  elbow.attach(9);
  elbow.writeMicroseconds(midI);
  //elbow.write(0);  // set servo to zero
  delay(500);
} 

void loop() { 

  for (int x = midI; x <= maxI; x++)
  {
  elbow.writeMicroseconds(x);
  delay(speedDelay);
  }

  for (int i = maxI; i >= minI; i--)
  {
  elbow.writeMicroseconds(i);
  delay(speedDelay);
  }

  for (int j = minI; j <= midI; j++)
  {
  elbow.writeMicroseconds(j);
  delay(speedDelay);
  }

}
