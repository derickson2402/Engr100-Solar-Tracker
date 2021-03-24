#include <Servo.h>
#include <SoftwareSerial.h>

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
    the ground and the panel. East/West corresponds to right/left, which
    rotates the whole apparatus about the z axis parallel to the ground. The
    cardinal directions are aligned as such:

                           N                (+)
                         W-+-E    -->    (-)-+-(+)
                           S                (-)

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


// Declare sensor and transmission pin numbers
const int sensorWestPin = 0;        // Pin number for west photoresistor
const int sensorEastPin = 1;        // Pin number for east photoresistor
const int sensorNorthPin = 2;       // Pin number for north photoresistor
const int sensorSouthPin = 3;       // Pin number for south photoresistor
const int sensorTempPin = 4;        // Pin number for temperature sensor
const int servoNSPin = 10;          // Pin number for NS servo
const int servoEWPin = 11;          // Pin number for EW servo
const int btTXPin = 5;              // Pin number for bt transmit (TX)
const int btRXPin = 6;              // Pin number for bt recieve (RX)

// Declare thresholds and configurable settings
const double configLightThreshold = 1.0; // Min light % difference before moving
const int servoMoveDist = 1;        // Servo position change in degrees
const int configServoDist = 1;      // Servo position change in degrees
const int configServoDelay = 10;    // Servo time between pos updates in ms
const bool configDebug = false;     // Turn debugging on or off
const bool configBT = false;        // Turn bluetooth on or off
const bool configBTInterval = 60;   // Set bluetooth update interval in seconds

// Open servo and serial objects
Servo servoNS;                      // Servo object for North South servo
Servo servoEW;                      // Servo object for East West servo
SoftwareSerial btSerial(btRXPin,btTXPin); // Serial port to bluetooth device


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
  }

  // Compare percent difference in East West direction
  if ( sensorEWDiff > configLightThreshold ) {

    if (sensorWestSignal > sensorEastSignal) {
      servoEW.write(servoEW.read() - configServoDist); // Twist clockwise to the West
      if (configDebug) {Serial.print("clockwise\t");}
    }

    else if (sensorEastSignal > sensorWestSignal) {
      servoEW.write(servoEW.read() + configServoDist); // Twist counterclockwise to the East
      if (configDebug) {Serial.print("counterclockwise\t");}
    }

  }

  // Compare percent difference in North South direction
  if ( sensorNSDiff > configLightThreshold ) {

    if (sensorSouthSignal > sensorNorthSignal) {
      servoNS.write(servoNS.read() - configServoDist); // Angle down to the south
      if (configDebug) {Serial.print("down\n");}
    }

    else if (sensorNorthSignal > sensorSouthSignal) {
      servoNS.write(servoNS.read() + configServoDist); // Angle up to the north
      if (configDebug) {Serial.print("up\n");}
    }

    else {Serial.print("\n");}

  }

  // Report data over bluetooth if current time is correct
  if (configBT) {
    if ((millis() % (configBTInterval * 1000)) < configServoDelay) {
      btSerial.print("Current Time:             "); btSerial.println(millis() / 1000);
      btSerial.print("Ambient Temperature:      "); btSerial.println(calcTemp(sensorTempSignal));
      btSerial.print("Maximum Irradiance:       "); btSerial.println(calcIrrad(sensorNorthSignal, sensorSouthSignal, sensorEastSignal, sensorWestSignal));
      btSerial.print("Relative Sun Position:    "); btSerial.println("PLACEHOLDER");
    }
  }

  // Wait to ensure that the servo reaches its position
  delay(configServoDelay);

}
