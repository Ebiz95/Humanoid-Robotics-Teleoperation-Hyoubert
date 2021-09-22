#include <Servo.h> 
Servo grip;

int maxI = 130;
int minI = 0;
int speedDelay = 10;

void setup() {
  // put your setup code here, to run once:
  grip.attach(3);
  grip.write(minI); //Open gripper

}

void loop() {
  //Close gripper
  for (int x = minI; x <= maxI; x++)
  {
  grip.write(x);
  delay(speedDelay);
  }

  //Open gripper
  for (int x = maxI; x >= minI; x--)
  {
  grip.write(x);
  delay(speedDelay);
  }
}
