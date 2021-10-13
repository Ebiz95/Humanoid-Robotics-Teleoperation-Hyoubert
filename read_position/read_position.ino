// This code is mega stolen and edited. Needs to be edited into functions.
// for position in cm
// send 15 character string like 001000200030
// this means x = 10mm, y = 20mm and z = 30mm
// this string is usually sent from Unity 

#include <Servo.h> 
String readString, servo1, servo2, servo3;
Servo myservo1;  // create servo object to control a servo 
Servo myservo2;
Servo myservo3;

void setup() {
  Serial.begin(9600);
  myservo1.attach(8); // hand //the pin for the servo control 
  myservo2.attach(9); // elbow
  myservo3.attach(7); // neck
  //myservo4.attach(9); // que
  //myservo5.attach(9); // que 2 electric bogaloo
  Serial.println("servo-test:"); // so I can keep track of what is loaded
}

void loop() {

  while (Serial.available()) {
    delay(10);  
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }

  if (readString.length() >0) {
      Serial.println(readString); //see what was received
      
      // expect a string like 090120 containing the two servo positions (in deg)     
      servo1 = readString.substring(0, 5); //get the first three characters
      servo2 = readString.substring(5, 10); //get the next three characters 
      servo3 = readString.substring(10, 15); //get the next three characters 

      Serial.print("Hand: ");
      Serial.println(servo1);  //print ot serial monitor to see results
      Serial.print("Elbow: ");
      Serial.println(servo2);
      Serial.print("Shoulder (up/down): ");
      Serial.println(servo3);
      Serial.print("Shoulder sideways: ");
      
      int n1; //declare as number  
      int n2;
      int n3;
      
      char carray1[4]; //magic needed to convert string to a number 
      servo1.toCharArray(carray1, sizeof(carray1));
      n1 = atoi(carray1); 
      
      char carray2[4];
      servo2.toCharArray(carray2, sizeof(carray2));
      n2 = atoi(carray2); 
      
      char carray3[4];
      servo3.toCharArray(carray3, sizeof(carray3));
      n3 = atoi(carray3); 

      // Don't do this without converting to angles
      //myservo1.write(n1); //set servo position 
      //myservo2.write(n2);
      //myservo3.write(n3);
    readString="";
  } 
