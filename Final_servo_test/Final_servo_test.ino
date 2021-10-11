// This code is mega stolen and edited. Needs to be edited into functions.
// for degrees 0-180
// send 15 character string like 010020030040050
// this string is usually sent from Unity 

#include <Servo.h> 
String readString, servo1, servo2, servo3, servo4, servo5;
Servo myservo1;  // create servo object to control a servo 
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;

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
      servo1 = readString.substring(0, 3); //get the first three characters
      servo2 = readString.substring(3, 6); //get the next three characters 
      servo3 = readString.substring(6, 9); //get the next three characters 
      servo4 = readString.substring(9, 12); //get the next three characters 
      servo5 = readString.substring(12, 15); //get the next three characters 

      Serial.print("Hand: ");
      Serial.println(servo1);  //print ot serial monitor to see results
      Serial.print("Elbow: ");
      Serial.println(servo2);
      Serial.print("Shoulder (up/down): ");
      Serial.println(servo3);
      Serial.print("Shoulder sideways: ");
      Serial.println(servo4);
      Serial.print("Neck: ");
      Serial.println(servo5);
      
      int n1; //declare as number  
      int n2;
      int n3;
      int n4;
      int n5;
      
      char carray1[4]; //magic needed to convert string to a number 
      servo1.toCharArray(carray1, sizeof(carray1));
      n1 = atoi(carray1); 
      
      char carray2[4];
      servo2.toCharArray(carray2, sizeof(carray2));
      n2 = atoi(carray2); 
      
      char carray3[4];
      servo3.toCharArray(carray3, sizeof(carray3));
      n3 = atoi(carray3); 
      
      char carray4[4];
      servo4.toCharArray(carray4, sizeof(carray4));
      n4 = atoi(carray4); 
      
      char carray5[4];
      servo5.toCharArray(carray5, sizeof(carray5));
      n5 = atoi(carray5); 
      
      myservo1.write(n1); //set servo position 
      myservo2.write(n2);
      myservo3.write(n3);
      myservo4.write(n4);
      myservo5.write(n5);
    readString="";
  } 
}
