#include <Servo.h>

/*##############################################################################

Date
    February 28, 2021

Written By
    Daniel Erickson   (danerick)
    Juliana Rodich    (jrodich)
    John Keane        (jjkeane)
    Caroline Collins  (carcoll)

Course
    Engr 100-435

Description
    This program drives two servo motors connected to an Arduino, in order to
    angle a solar panel towards a moving light source.

    Sensors and rotational axes are aligned according to a cardinal directional system, with North and East being positive as such:

                           N                (+)
                         W-+-E    -->    (-)-+-(+)
                           S                (-)

##############################################################################*/


// Establish sensor pins, servo pins, and threshold values
const int sensorWestPin = 0;        // The pin number for west photoresistor
const int sensorEastPin = 1;        // The pin number for east photoresistor
const int sensorNorthPin = 2;       // The pin number for north photoresistor
const int sensorSouthPin = 3;       // The pin number for south photoresistor
const int lightErrorThreshold = 10; // Threshold for the light level error, below which the servo position will not change
const int servoNSPin = 4;           // The pin number for NS servo
const int servoEWPin = 5;           // The pin number for EW servo
const int servoMoveDist = 1;            // Distance in degrees a servo will move at a time
Servo servoNS;                      // Servo object for North South servo
Servo servoEW;                      // Servo object for East West servo


/* TODO: Currently the program uses raw light level error to determine
actions, but it might be useful to have a conversion function so
that actual irradiance values can be used
int Convert the raw input to a 10 bit scale ( raw) {
  
}
*/


void setup() {

  // Open a serial port for remote instrument reading
  Serial.begin(9600);

  // Connect servo objects to their signal pins
  servoNS.attach(servoNSPin);
  servoEW.attach(servoEWPin);

}

void loop() {

  // Calculate the error on each axis, or the difference between the sensor
  // light levels
  int lightErrorNS = analogRead(sensorSouthPin) - analogRead(sensorNorthPin);
  int lightErrorEW = analogRead(sensorWestPin) - analogRead(sensorEastPin);


  // Possible TODO: run conversion function to get standardized light level ???


  // Compare error in East West direction
  if ( abs(lightErrorEW) > lightErrorThreshold ) {

    if ( lightErrorEW > 0 ) {
      servoEW.write(servoEW.read() - servoMoveDist);
    }
    else if ( lightErrorEW < 0 ) {
      servoEW.write(servoEW.read() + servoMoveDist);
    }

  }


  // Compare error in North South direction
  if ( abs(lightErrorNS) > lightErrorThreshold ) {

    if ( lightErrorNS > 0 ) {
      servoNS.write(servoNS.read() - servoMoveDist);
    }
    else if ( lightErrorNS < 0 ) {
      servoNS.write(servoNS.read() + servoMoveDist);
    }

  }


  // Wait to ensure that the servo reaches its position
  delay(20);

}