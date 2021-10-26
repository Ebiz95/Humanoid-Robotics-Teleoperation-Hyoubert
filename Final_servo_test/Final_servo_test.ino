// This code is mega stolen and edited. Needs to be edited into functions.
// for degrees 0-180
// send 15 character string like 010020030040050
// this string is usually sent from Unity

#include <XYZrobotServo.h>
// Create a SoftwareSerial object using pin 10 to receive (RX) and pin 11 to transmit (TX)
#ifdef SERIAL_PORT_HARDWARE_OPEN
#define servoSerial SERIAL_PORT_HARDWARE_OPEN
#else
#include <SoftwareSerial.h>
SoftwareSerial servoSerial(10, 11);
#endif

// Set up a servo object, specifying what serial port to use and what ID number to use
XYZrobotServo servo(servoSerial, 1);
const uint8_t playtime = 75; // How long the movement takes, in units of 10 ms

#include <Servo.h>
String readString, servo1, servo2, servo3, servo4, servo5;
float n1p, n2p, n3p, n4p, n5p;
int n1 = 90; // hand 180 - 50 (open - close)
int n2 = 140; // elbow range 140 - 20 (straight - bent)
int n3 = 95; // neck 0 - 180 (left - right)
int n4 = 80; // shoulder 80 - 260 (down - up)
int n5 = 90;
float n1s, n2s, n3s, n4s, n5s;
Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;

void setup() {
  Serial.begin(115200);
  myservo1.attach(8); // hand //the pin for the servo control
  myservo2.attach(9); // elbow
  myservo3.attach(7); // neck
  servoSerial.begin(115200); // shoulder (Turn on the serial port and set its baud rate)
  //myservo5.attach(9); // que 2 electric bogaloo
  Serial.println("servo-test:"); // so I can keep track of what is loaded
  n4 = map(n4, 0, 330, 0, 1023);
}

void loop() {
  read_serial();
}

void read_serial() {
  while (Serial.available()) {
    delay(2);
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }

  if (readString.length() > 0) {
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
    Serial.print("neck: ");
    Serial.println(servo3);
    Serial.print("TBD: ");
    Serial.println(servo4);
    Serial.print("TBD: ");
    Serial.println(servo5);

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
    n4 = map(n4, 0, 330, 0, 1023);

    char carray5[4];
    servo5.toCharArray(carray5, sizeof(carray5));
    n5 = atoi(carray5);
    
    readString = "";
    write_servo();
  }
}

void write_servo2() {
  float a = 0.9;
  n1s = (n1 * a) + (n1p * (1 - a));
  n2s = (n2 * a) + (n2p * (1 - a));
  n3s = (n3 * a) + (n3p * (1 - a));
  n4s = (n4 * a) + (n4p * (1 - a));
  n5s = (n5 * a) + (n5p * (1 - a));

  // bookmark previous values

  n1p = n1s;
  n2p = n2s;
  n3p = n3s;
  n4p = n4s;
  n5p = n5s;

  Serial.print(n1s);
  Serial.print(" , ");
  Serial.print(n2s);
  Serial.print(" , ");
  Serial.print(n3s);
  Serial.print(" , ");
  Serial.print(n4s);
  Serial.print(" , ");
  Serial.println(n5s);

  myservo1.write(n1s);
  myservo2.write(n2s);
  myservo3.write(n3s);
  servo.setPosition(n4s, 0);
  //myservo5.write(n5s);
}

void write_servo() {
  float a = 0.9;
  n1s = (n1 * a) + (n1p * (1 - a));
  n2s = (n2 * a) + (n2p * (1 - a));
  n3s = (n3 * a) + (n3p * (1 - a));
  n4s = (n4 * a) + (n4p * (1 - a));
  n5s = (n5 * a) + (n5p * (1 - a));

  // bookmark previous values

  n1p = n1s;
  n2p = n2s;
  n3p = n3s;
  n4p = n4s;
  n5p = n5s;

  Serial.print(n1s);
  Serial.print(" , ");
  Serial.print(n2s);
  Serial.print(" , ");
  Serial.print(n3s);
  Serial.print(" , ");
  Serial.print(n4s);
  Serial.print(" , ");
  Serial.println(n5s);

  myservo1.write(n1s);
  myservo2.write(n2s);
  myservo3.write(n3s);
  servo.setPosition(n4s, 0);
  //myservo5.write(n5s);
}
