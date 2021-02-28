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

Libraries
    [This section is a placeholder and should be updated]
    
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
const int servoNSPin = 4;           // The pin number for angle servo
const int servoEWPin = 5;           // The pin number for rotation servo
const int lightErrorThreshold = 10; // The threshold for the light level error,
Servo servo
//                                     below which the servo position will not
//                                     change


/* TODO: Currently the program uses raw light level error to determine
actions, but it might be useful to have a conversion function so
that actual irradiance values can be used
int Convert the raw input to a 10 bit scale ( raw) {
  
}
*/


void setup() {

  Serial.begin(9600);               // Open a serial port for remote instrument reading
  pinMode(servoNWPin, OUTPUT);   // Open North West servo output
  pinMode(servoEWPin, OUTPUT);  // Open East West servo output

}

void loop() {

  // Calculate the error on each axis, which is the difference between the
  // light levels on one axis
  int lightErrorNS = analogRead(sensorSouthPin) - analogRead(sensorNorthPin);
  int lightErrorEW = analogRead(sensorWestPin) - analogRead(sensorEastPin);


  // Possible TODO: run conversion function to get standardized light level ???


  // Compare error in East West direction
  if ( abs(lightErrorEW) > lightErrorThreshold ) {

    if ( lightErrorEW > 0 ) {
      // Move the servo to the East (positive direction)
    }
    else if ( lightErrorEW < 0 ) {
      // Move the servo to the West (negative direction)
    }

  }


  // Compare error in North South direction
  if ( abs(lightErrorNS) > lightErrorThreshold ) {

    if ( lightErrorNS > 0 ) {
      // Move the servo to the North (positive direction)
    }
    else if ( lightErrorNS < 0 ) {
      // Move the servo to the South (negative direction)
    }

  }
 
}