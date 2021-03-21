#include <Servo.h>
#include <SoftwareSerial.h>

/*##############################################################################

Date
    March 15, 2021

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
const int sensorTempPin = 4;        // Pin number for temperature sensor
const int servoNSPin = 10;          // Pin number for NS servo
const int servoEWPin = 11;          // Pin number for EW servo
const int btTXPin = 5;              // Pin number for bt transmit (TX)
const int btRXPin = 6;              // Pin number for bt recieve (RX)

// Declare thresholds and configurable settings
const int lightErrorThreshold = 100; // Min lightlevel dif for movement to occur
const int configServoDist = 1;        // Servo position change in degrees
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


// Calculate the temperature in Celcius from a temperature sensor signal
int calcTemp(const int &signal) {

  // Calculate the temperature
  // TODO: Add math here for temperature conversion
  // The following line is a placeholder
  int temp = 1234;
  return(temp);

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
  // calculate the light level error
  int sensorNorthSignal = analogRead(sensorNorthPin);
  int sensorSouthSignal = analogRead(sensorSouthPin);
  int sensorEastSignal = analogRead(sensorEastPin);
  int sensorWestSignal = analogRead(sensorWestPin);
  int sensorTempSignal = analogRead(sensorTempPin);
  int lightErrorNS = sensorSouthSignal - sensorNorthSignal;
  int lightErrorEW = sensorWestSignal - sensorEastSignal;
  
  
  // If debugging is enabled, print diagnostics
  if (configDebug) {
    Serial.print(analogRead(sensorSouthPin)); Serial.print("\t");
    Serial.print(analogRead(sensorNorthPin) ); Serial.print("\t");
    Serial.print(lightErrorNS); Serial.print("\tNS\t");
    Serial.print(analogRead(sensorWestPin) ); Serial.print("\t");
    Serial.print(analogRead(sensorEastPin) ); Serial.print("\t");
    Serial.print(lightErrorEW); Serial.print("\tEW\t");

    if (configBT) {
      btSerial.print(analogRead(sensorSouthPin)); btSerial.print("\t");
      btSerial.print(analogRead(sensorNorthPin) ); btSerial.print("\t");
      btSerial.print(lightErrorNS); btSerial.print("\tNS\t");
      btSerial.print(analogRead(sensorWestPin) ); btSerial.print("\t");
      btSerial.print(analogRead(sensorEastPin) ); btSerial.print("\t");
      btSerial.print(lightErrorEW); btSerial.print("\tEW\t");
    }
  }


  // Compare error in East West direction
  if ( abs(lightErrorEW) > lightErrorThreshold ) {

    if ( lightErrorEW > 0 ) {
      servoEW.write(servoEW.read() - configServoDist); // Twist clockwise to the West
      if (configDebug) {
        Serial.print("clockwise\t");
        if (configBT) {btSerial.print("clockwise\t");}
      }
    }

    else if ( lightErrorEW < 0 ) {
      servoEW.write(servoEW.read() + configServoDist); // Twist counterclockwise to the East
      if (configDebug) {
        Serial.print("counterclockwise\t");
        if (configBT) {btSerial.print("counterclockwise\t");}
      }
    }

  }


  // Compare error in North South direction
  if ( abs(lightErrorNS) > lightErrorThreshold ) {

    if ( lightErrorNS > 0 ) {
      servoNS.write(servoNS.read() - configServoDist); // Angle down to the south
      if (configDebug) {
        Serial.print("down\n");
        if (configBT) {btSerial.print("down\n");}
      }
    }
    
    else if ( lightErrorNS < 0 ) {
      servoNS.write(servoNS.read() + configServoDist); // Angle up to the north
      if (configDebug) {
        Serial.print("up\n");
        if (configBT) {btSerial.print("up\n");}
      }
    }

    else {Serial.print("\n"); if (configBT) {btSerial.print("\n");}}
  }


  // Report data over bluetooth if current time is correct
  if ((millis() % (configBTInterval * 1000)) < configServoDelay) {
    btSerial.print("Current Time:             "); btSerial.println(millis() / 1000);
    btSerial.print("Ambient Temperature:      "); btSerial.println(calcTemp(sensorTempSignal));
    btSerial.print("Maximum Irradiance:       "); btSerial.println(calcIrrad(sensorNorthSignal, sensorSouthSignal, sensorEastSignal, sensorWestSignal));
    btSerial.print("Relative Sun Position:    "); btSerial.println("PLACEHOLDER");
  }


  // Wait to ensure that the servo reaches its position
  delay(configServoDelay);

}
