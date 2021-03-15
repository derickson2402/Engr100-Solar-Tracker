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
    This program powers an Arduino to angle a solar panel towards a moving
    light source. The system uses 4 photoresistors and 2 servo motors, and can
    output diagnostic and periodic usage data over USB or Bluetooth serial.

    Axis Alignment
    Sensors and rotational axes are configured with a cardinal directioin
    system. North/South corresponds to up/down, which changes the angle between
    the ground and the panel. East/West corresponds to right/left, which
    rotates the whole apparatus about the z axis parallel to the ground. The
    cardinal directions are aligned as such:

                           N                (+)
                         W-+-E    -->    (-)-+-(+)
                           S                (-)

    Movement Algorithm
    The servo position is updated using a closed loop control structure. The
    raw inputs from both photoresistors on an axis are compared, and if the
    percent difference is higher than the threshold the servo motor will move
    one tick in the brighter direction.

    Bluetooth Serial Output
    By default the program will send debugging and serial outputs to the
    default port, which goes to pin 13 and to the USB port. When bluetooth is
    on, this data will also be sent to the specified bluetooth port. This code
    is tested to work on a HC-05 Serial Bluetooth Module v1.3.

##############################################################################*/


// Declare sensor and transmission pin numbers
const int sensorWestPin = 0;        // Pin number for west photoresistor
const int sensorEastPin = 1;        // Pin number for east photoresistor
const int sensorNorthPin = 2;       // Pin number for north photoresistor
const int sensorSouthPin = 3;       // Pin number for south photoresistor
const int servoNSPin = 10;          // Pin number for NS servo
const int servoEWPin = 11;          // Pin number for EW servo
const int btTXPin = 5;              // Pin number for bt transmit (TX)
const int btRXPin = 6;              // Pin number for bt recieve (RX)

// Declare thresholds and configurable settings
const double lightDiffThreshold = 1.0; // Min light difference
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
  int sensorNorthLight = analogRead(sensorNorthPin);
  int sensorSouthLight = analogRead(sensorSouthPin);
  int sensorEastLight = analogRead(sensorEastPin);
  int sensorWestLight = analogRead(sensorWestPin);
  double sensorEWDiff = ( (double)abs(sensorWestLight - sensorEastLight) / ( (double)(sensorWestLight + sensorEastLight) / 2.0)) * 100.0;
  double sensorNSDiff = ( (double)abs(sensorSouthLight - sensorNorthLight) / ( (double)(sensorSouthLight + sensorNorthLight) / 2.0)) * 100.0;

  // If debugging is enabled, print diagnostics
  if (configDebug) {
    Serial.print("NS\t"); Serial.print(sensorNSDiff);
    Serial.print("\t"); Serial.print(sensorNorthLight);
    Serial.print("\t"); Serial.print(sensorSouthLight);
    Serial.print("\tEW\t"); Serial.print(sensorEWDiff);
    Serial.print("\t"); Serial.print(sensorEastLight);
    Serial.print("\t"); Serial.print(sensorWestLight);

    if (configBT) {
      btSerial.print("NS\t"); btSerial.print(sensorNSDiff);
      btSerial.print("\t"); btSerial.print(sensorNorthLight);
      btSerial.print("\t"); btSerial.print(sensorSouthLight);
      btSerial.print("\tEW\t"); btSerial.print(sensorEWDiff);
      btSerial.print("\t"); btSerial.print(sensorEastLight);
      btSerial.print("\t"); btSerial.print(sensorWestLight);
    }
  }


  // Compare percent difference in East West direction
  if ( sensorEWDiff > lightDiffThreshold ) {

    if ( sensorWestLight > sensorEastLight ) {
      servoEW.write(servoEW.read() - servoMoveDist); // Twist clockwise to the West
      if (configDebug) {
        Serial.print("clockwise\t");
        if (configBT) {btSerial.print("clockwise\t");}
      }
    }

    else if ( sensorEastLight > sensorWestLight ) {
      servoEW.write(servoEW.read() + servoMoveDist); // Twist counterclockwise to the East
      if (configDebug) {
        Serial.print("counterclockwise\t");
        if (configBT) {btSerial.print("counterclockwise\t");}
      }
    }

  }


  // Compare percent difference in North South direction
  if ( sensorNSDiff > lightDiffThreshold ) {

    if ( sensorSouthLight > sensorNorthLight ) {
      servoNS.write(servoNS.read() - servoMoveDist); // Angle down to the south
      if (configDebug) {
        Serial.print("down\n");
        if (configBT) {btSerial.print("down\n");}
      }
    }
    
    else if ( sensorNorthLight > sensorSouthLight ) {
      servoNS.write(servoNS.read() + servoMoveDist); // Angle up to the north
      if (configDebug) {
        Serial.print("up\n");
        if (configBT) {btSerial.print("up\n");}
      }
    }

    else {Serial.print("\n"); if (configBT) {btSerial.print("\n");}}
  }


  // Wait to ensure that the servo reaches its position
  delay(100);

}
