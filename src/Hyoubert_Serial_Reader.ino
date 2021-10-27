// Code for receiving 4 angles from unity and writing them to servos
// recieves 12 character strings like 010020030040

// Creates a serial connection to the bus-servo (XYZ-servo). In this project a hardware serial i opened at port 18 and 19.
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

#include <Servo.h>
String readString, servo1, servo2, servo3, servo4;
// Variables for holding the received angles
int hand = 90; // hand 180 - 50 (open - close)
int elbow = 140; // elbow range 140 - 20 (straight - bent)
int neck = 95; // neck 0 - 180 (left - right)
int shoulder = 80; // shoulder 80 - 260 (down - up)
// Variables for applying the EWMA algorithm
float hand_smoothed, elbow_smoothed, neck_smoothed, shoulder_smoothed;
float hand_prev, elbow_prev, neck_prev, shoulder_prev;
Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;

// Variables for the timing of the servo-writing
unsigned long time_now = 0;
int period = 5;

void setup() {
  Serial.begin(115200);
  myservo1.attach(8); // hand //the pin for the servo control
  myservo2.attach(9); // elbow
  myservo3.attach(7); // neck
  servoSerial.begin(115200); // shoulder (Turn on the serial port and set its baud rate)
  Serial.println("servo-test:"); // so I can keep track of what is loaded
  shoulder = map(shoulder, 0, 330, 0, 1023); // xyz-servos use a different mapping 
}

void loop() {
  // read the serial port every loop. 
  read_serial();

  // write to servos at an interval of period milliseconds
  // Needed for a reliable behaivor from EWMA but also for allowing the arduino to spend more time reading serial data
  // If the serial buffer gets full the Unity engine slows down and eventually crashes
  if(millis() >= time_now + period){
    time_now += period;
    write_servo();
  }
}

void read_serial() {
  // Read the serial buffer until empty. if there is one
  while (Serial.available()) {
    delay(2);
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }

  if (readString.length() > 0) {
    // Serial.println(readString); //see what was received

    // expect a string like 090120020030 containing the two servo positions (in deg)
    servo1 = readString.substring(0, 3); //get the first three characters
    servo2 = readString.substring(3, 6); //get the next three characters
    servo3 = readString.substring(6, 9); //get the next three characters
    servo4 = readString.substring(9, 12); //get the next three characters
    
    char carray1[4]; //magic needed to convert string to a number
    servo1.toCharArray(carray1, sizeof(carray1));
    hand = atoi(carray1);

    char carray2[4];
    servo2.toCharArray(carray2, sizeof(carray2));
    elbow = atoi(carray2);

    char carray3[4];
    servo3.toCharArray(carray3, sizeof(carray3));
    neck = atoi(carray3);

    char carray4[4];
    servo4.toCharArray(carray4, sizeof(carray4));
    shoulder = atoi(carray4);
    shoulder = map(shoulder, 0, 330, 0, 1023);
    
    readString = "";
  }
}

void write_servo() {
  // EWMA algorithm
  float alpha = 0.05;
  hand_smoothed = (hand * alpha) + (hand_prev * (1 - alpha));
  elbow_smoothed = (elbow * alpha) + (elbow_prev * (1 - alpha));
  neck_smoothed = (neck * alpha) + (neck_prev * (1 - alpha));
  shoulder_smoothed = (shoulder * alpha) + (shoulder_prev * (1 - alpha));

  // bookmark previous values

  hand_prev = hand_smoothed;
  elbow_prev = elbow_smoothed;
  neck_prev = neck_smoothed;
  shoulder_prev = shoulder_smoothed;

  // Constrain angles to legal range
  hand_smoothed = constrain(hand_smoothed, 50, 180);
  elbow_smoothed = constrain(elbow_smoothed, 20, 140);
  neck_smoothed = constrain(neck_smoothed, 0, 180);
  shoulder_smoothed = constrain(shoulder_smoothed, 248, 806);

  // Write constrained and smoothed angle to servos
  myservo1.write(hand_smoothed);
  myservo2.write(elbow_smoothed);
  myservo3.write(neck_smoothed);
  servo.setPosition(shoulder_smoothed, 0);
}
