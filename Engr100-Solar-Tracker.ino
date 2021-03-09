#include <Servo.h>
#include <SoftwareSerial.h>

/*##############################################################################

Date
    March 09, 2021

Written By
    Daniel Erickson   (danerick)
    Juliana Rodich    (jrodich)
    John Keane        (jjkeane)
    Caroline Collins  (carcoll)

Course
    Engr 100-435

Description
    This program powers an Arduino to angle a solar panel towards a moving light source. The system uses 4 photoresistors and 2 servo motors, and can output diagnostic and periodic usage data over USB or Bluetooth serial.

    Axis Alignment
    Sensors and rotational axes are configured with a cardinal directioin system. North/South corresponds to up/down, which changes the angle between the ground and the panel. East/West corresponds to right/left, which rotates the whole apparatus about the z axis parallel to the ground. The cardinal directions are aligned as such:

                           N                (+)
                         W-+-E    -->    (-)-+-(+)
                           S                (-)

    Movement Algorithm
    The servo position is updated using a simple error correction algorithm. The raw input from two opposite photoresistors is compared, and every cycle the servo motor on that axis will move one tick in the brighter direction.

##############################################################################*/


// Declare sensor and transmission pin numbers
const int sensorWestPin = 0;        // Pin number for west photoresistor
const int sensorEastPin = 1;        // Pin number for east photoresistor
const int sensorNorthPin = 2;       // Pin number for north photoresistor
const int sensorSouthPin = 3;       // Pin number for south photoresistor
const int servoNSPin = 5;           // Pin number for NS servo
const int servoEWPin = 6;           // Pin number for EW servo
const int btTXPin = 10;             // Pin number for bt transmit (TX)
const int btRXPin = 11;             // Pin number for bt recieve (RX)

// Declare thresholds and configurable settings
const int lightErrorThreshold = 100; // Min lightlevel dif for movement to occur
const int servoMoveDist = 1;        // Servo position change in degrees
const bool configDebug = false;     // Turn debugging on or off
const bool configBT = false;        // Turn bluetooth on or off

// Open servo and serial objects
Servo servoNS;                      // Servo object for North South servo
Servo servoEW;                      // Servo object for East West servo
SoftwareSerial btSerial(btRXPin,btTXPin); // Serial port to bluetooth device


void setup() {

  // Open USB serial port
  Serial.begin(9600);
  Serial.println("\t\tHello! I am Sunflower!\n\n\t\tYou are connected over USB\n");
  if (configDebug){Serial.println("*** Debugging Enabled ***\n");}


  // Connect servo objects to their signal pins
  servoNS.attach(servoNSPin);
  servoEW.attach(servoEWPin);
  servoNS.write(90);
  servoEW.write(90);

  // Set bluetooth transmission pins and open serial port
  if (configBT) {
    pinMode(btRXPin, INPUT);
    pinMode(btTXPin, OUTPUT);
    btSerial.begin(9600);
    btSerial.println("\t\tHello! I am Sunflower!\n\n\t\tYou are connected over Bluetooth\n");
    if (configDebug){btSerial.println("*** Debugging Enabled ***\n");}
  }
}


void loop() {

  // Calculate the error on each axis, or the difference between the sensor
  // light levels
  int lightErrorNS = analogRead(sensorSouthPin) - analogRead(sensorNorthPin);
  int lightErrorEW = analogRead(sensorWestPin) - analogRead(sensorEastPin);
  
  
  // If debugging is enabled, print diagnostics
  if (configDebug) {
    Serial.print(analogRead(sensorSouthPin)); Serial.print("\t");
    Serial.print(analogRead(sensorNorthPin) ); Serial.print("\t");
    Serial.print(lightErrorNS); Serial.print("\tNS");
    Serial.print(analogRead(sensorWestPin) ); Serial.print("\t");
    Serial.print(analogRead(sensorEastPin) ); Serial.print("\t");
    Serial.print(lightErrorEW); Serial.print("\tEW");

    if (configBT) {
      btSerial.print(analogRead(sensorSouthPin)); btSerial.print("\t");
      btSerial.print(analogRead(sensorNorthPin) ); btSerial.print("\t");
      btSerial.print(lightErrorNS); btSerial.print("\tNS");
      btSerial.print(analogRead(sensorWestPin) ); btSerial.print("\t");
      btSerial.print(analogRead(sensorEastPin) ); btSerial.print("\t");
      btSerial.print(lightErrorEW); btSerial.print("\tEW");
    }
  }
  

  // Compare error in East West direction
  if ( abs(lightErrorEW) > lightErrorThreshold ) {

    if ( lightErrorEW > 0 ) {
      servoEW.write(servoEW.read() - servoMoveDist); // Twist clockwise to the West
      if (configDebug) {
        Serial.print("clockwise\t");
        if (configBT) {btSerial.print("clockwise\t");}
      }
    }

    else if ( lightErrorEW < 0 ) {
      servoEW.write(servoEW.read() + servoMoveDist); // Twist counterclockwise to the East
      if (configDebug) {
        Serial.print("counterclockwise\t");
        if (configBT) {btSerial.print("counterclockwise\t");}
      }
    }

  }


  // Compare error in North South direction
  if ( abs(lightErrorNS) > lightErrorThreshold ) {

    if ( lightErrorNS > 0 ) {
      servoNS.write(servoNS.read() - servoMoveDist); // Angle down to the south
      if (configDebug) {
        Serial.print("down\n");
        if (configBT) {btSerial.print("down\n");}
      }
    }
    
    else if ( lightErrorNS < 0 ) {
      servoNS.write(servoNS.read() + servoMoveDist); // Angle up to the north
      if (configDebug) {
        Serial.print("up\n");
        if (configBT) {btSerial.print("up\n");}
      }    }

  }


  // Wait to ensure that the servo reaches its position
  delay(100);

}
