#include <Servo.h>

/*##############################################################################

Date
    March 24, 2021

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
    output diagnostic and ambient environment data over USB or Bluetooth serial.

    Axis Alignment
    Sensors and rotational axes are configured with a cardinal directioin
    system. North/South corresponds to up/down, which changes the angle between
    the ground and the panel. East/West corresponds to right/left and
    clockwise/counterclockwise, which rotates the whole apparatus about the
    z axis parallel to the ground. The cardinal directions are aligned as such:

                         N                 (180)
                       W-+-E    -->    ( 0 )-+-(180)
                         S                 ( 0 )

    Movement Algorithm
    The servo position is updated using a simple error correction structure. The
    raw inputs from both photoresistors on an axis are compared, and if the
    percent difference is higher than the configured threshold then the servo motor will move one step in the brighter direction.

    Bluetooth Serial Output
    When switched on, the program can send ambient environment data over
    Bluetooth serial. The data includes ambient temperature, incident solar
    irradiance, and relative sun position. This code is tested to work with a
    HC-05 Serial Bluetooth Module v1.3, but uses a standart 9600 baud serial
    connection and should work with any serial connection.

##############################################################################*/


// Declare user settings. These should be modified before running the program
const int sensorWestPin = 0;        // Pin number for West photoresistor
const int sensorEastPin = 1;        // Pin number for East photoresistor
const int sensorNorthPin = 2;       // Pin number for North photoresistor
const int sensorSouthPin = 3;       // Pin number for South photoresistor
const int sensorTempPin = 4;        // Pin number for temperature sensor
const int servoNSPin = 10;          // Pin number for NS servo
const int servoEWPin = 11;          // Pin number for EW servo
const double configLightThreshold = 10.0; // Min light % difference for movement
const int configServoDist = 1;      // Servo position change in degrees
const int configServoDelay = 10;    // !0 # of ms between servo pos updates
const bool configDebug = false;     // Turn debugging on or off


// Declare global scope variables. These should not be modified before running
Servo servoNS;                      // Servo object for North South servo
Servo servoEW;                      // Servo object for East West servo
bool servoEWInvert = false;


void setup() {

  // Open USB serial port
  Serial.begin(9600);
  Serial.println("Hello! I am Sunflower!\n\nYou are connected over USB\n");
  if (configDebug){Serial.println("*** Debugging Enabled ***\n");}

  // Connect servo objects to their signal pins
  servoNS.attach(servoNSPin);
  servoEW.attach(servoEWPin);
  servoNS.write(90);
  servoEW.write(90);

}


void loop() {

  // Measure light level signal, irradiance signal, and temperature signal, then
  // calculate the light level percent error
  int sensorNorthSignal = analogRead(sensorNorthPin);
  int sensorSouthSignal = analogRead(sensorSouthPin);
  int sensorEastSignal = analogRead(sensorEastPin);
  int sensorWestSignal = analogRead(sensorWestPin);
  int sensorTempSignal = analogRead(sensorTempPin);
  double sensorEWDiff = ((double)abs(sensorWestSignal - sensorEastSignal) / ((double)(sensorWestSignal + sensorEastSignal) / 2.0)) * 100.0;
  double sensorNSDiff = ((double)abs(sensorSouthSignal - sensorNorthSignal) / ((double)(sensorSouthSignal + sensorNorthSignal) / 2.0)) * 100.0;

  // If debugging is enabled, print diagnostics
  if (configDebug) {
    Serial.print(sensorNorthSignal); Serial.print("\t");
    Serial.print(sensorSouthSignal); Serial.print("\t");
    Serial.print(sensorNSDiff); Serial.print("\tNS\t");
    Serial.print(sensorEastSignal); Serial.print("\t");
    Serial.print(sensorWestSignal); Serial.print("\t");
    Serial.print(sensorEWDiff); Serial.print("\tEW\t");
    Serial.print(servoEWInvert); Serial.print("\tinvert\t");
  }

  // Compare percent difference in East West direction
  if (sensorEWDiff > configLightThreshold) {

    // Inverted axis
    if (servoEWInvert) {
      // If servo needs to turn to the West
      if (sensorWestSignal > sensorEastSignal) {
        // If servo at boundary, flip it
        if (servoEW.read() >= 180) {servoInvert(servoEWInvert);}
        // Not at boundary, so twist counter-clockwise to the West
        else {
          servoEW.write(servoEW.read() + configServoDist);
          if (configDebug) {Serial.print("West counter-clockwise\t");}
        }
      }
      // If servo needs to turn to the East
      else if (sensorEastSignal > sensorWestSignal) {
        // If servo at boundary, flip it
        if (servoEW.read() <= 0) {servoInvert(servoEWInvert);}
        // Not at boundary, so twist clockwise to the East
        else {
          servoEW.write(servoEW.read() - configServoDist);
          if (configDebug) {Serial.print("East clockwise\t");}
        }
      }
    }

    // Normal axis, not inverted
    else if (!servoEWInvert) {
      // If servo needs to turn to the West
      if (sensorWestSignal > sensorEastSignal) {
        // If servo at boundary, flip it
        if (servoEW.read() <= 0) {servoInvert(servoEWInvert);}
        // Not at boundary, so twist clockwise to the West
        else {
          servoEW.write(servoEW.read() - configServoDist);
          if (configDebug) {Serial.print("West clockwise\t");}
        }
      }
      // If servo needs to turn to the East
      else if (sensorEastSignal > sensorWestSignal) {
        // If servo at boundary, flip it
        if (servoEW.read() >= 180) {servoInvert(servoEWInvert);}
        // Not at boundary, so twist counter-clockwise to the East
        else {
          servoEW.write(servoEW.read() + configServoDist);
          if (configDebug) {Serial.print("East counter-clockwise\t");}
        }
      }
    }

  }

  // Compare percent difference in North South direction
  if ( sensorNSDiff > configLightThreshold ) {

    if (sensorSouthSignal > sensorNorthSignal) {
      // Toggle EW inversion off when crossing over 90 degrees
      if (servoNS.read() == 90) {servoEWInvert = false;}
      servoNS.write(servoNS.read() - configServoDist); // Angle to the South
      if (configDebug) {Serial.println("South");}
    }

    else if (sensorNorthSignal > sensorSouthSignal) {
      // Toggle EW inversion on when crossing over 90 degrees
      if (servoNS.read() == 90) {servoEWInvert = true;}
      servoNS.write(servoNS.read() + configServoDist); // Angle to the North
      if (configDebug) {Serial.println("North");}
    }

    else {Serial.print("\n");}

  }


  // Wait to ensure that the servo reaches its position
  delay(configServoDelay);

}


// Calculate the irradiance (W*m^-2) from 4 photoresistor voltage signals
int calcIrrad(const int &signalA, const int &signalB, const int &signalC, const int &signalD) {

  // Average the input signal
  double signalAvg = (double)(signalA + signalB + signalC + signalD) / 4.0;

  // Calculate the irradiance
  double irrad = (655 * pow(signalAvg, 2.0)) - (4735 * signalAvg) + 8670;
  return((int)(irrad + 0.5));

}


// Calculate the temperature (deg Celsius) from a temperature sensor signal
int calcTemp(const int &signal) {

  // Calculate the temperature
  double temp = (((double)signal * 0.004882813) - 0.5) * 100;
  return((int)(temp + 0.5));

}


// Flip the servos and invert EW axis when boundary is hit
void servoInvert(bool& toggleInvert) {

  // Record current NS position and set to 90 degrees (neutral position)
  int servoNSOriginal = servoNS.read();
  servoNS.write(90);
  delay(configServoDelay);

  // Flip the EW axis and set EW inversion
  servoEW.write(180 - servoEW.read());
  toggleInvert = !toggleInvert;
  delay(configServoDelay);

  // Flip the NS axis according to original position
  servoNS.write(180 - servoNSOriginal);
  delay(configServoDelay);

}
