/*##############################################################################

Date
    February 23, 2021

Written By
    Daniel Erickson   (danerick)
    Juliana Rodich    (jrodich)
    John Keane        (jjkeane)
    Caroline Collins  (carcoll)

Course
    Engr 100-435

Libraries
    None (this section is a placeholder)
    
Description
    This program drives two servo motors connected to an Arduino, in order to
    angle a solar panel towards a moving light source.



##############################################################################*/

// Establish sensor pins, servo pins, and threshold values
const int sensorWest = 0;           // The pin number for west photoresistor
const int sensorEast = 1;           // The pin number for east photoresistor
const int sensorNorth = 2;          // The pin number for north photoresistor
const int sensorSouth = 3;          // The pin number for south photoresistor
const int servoAnglePin = 4;        // The pin number for angle servo
const int servoRotatePin = 5;       // The pin number for rotation servo
const int lightErrorThreshold = 10; // The threshold for the light level error,
//                                     below which the servo position will not
//                                     change

/* TODO: Currently the program uses raw light level error to determine
actions, but it might be useful to have a conversion function so
that actual irradiance values can be used */
int Convert the raw input to a 10 bit scale ( raw) {
  
}

void setup() {
  Serial.begin(9600);               // Open a serial port for remote instrument reading
  pinMode(servoAnglePin, OUTPUT);   // Open angle servo output
  pinMode(servoRotatePin, OUTPUT);  // Open rotation servo output
  )

}
void loop() {

  // collect raw input from sensors

  // run conversion function to get standardized light level ???

  // logic to compare Left and Right sensors
  if (right side is brighter by more than threshold) {
    move to the right
  } else if (left side is brighter by more than threshold) {
    move to the left
  }

  // logic to compare rotation
  
}

















void setup()
{
  pinMode(ledPin, OUTPUT);    // Set up the LED pin to be an output.
  Serial.begin(9600);
}

void loop()
{
  lightLevel = analogRead(sensorPin);  // reads the voltage on the sensorPin
  Serial.print(lightLevel);
  //autoRange();  // autoRanges the min / max values you see in your room.

  calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);  // scale the lightLevel from 0 - 1023 range to 0 - 255 range.
                                                  // the map() function applies a linear scale / offset.
                                                  // map(inputValue, fromMin, fromMax, toMin, toMax);
  Serial.print("\t"); 		  // tab character
  Serial.print(calibratedlightLevel);   // println prints an CRLF at the end (creates a new line after)

  analogWrite(ledPin, calibratedlightLevel);    // set the led level based on the input lightLevel.
}
/******************************************************************
 * void autoRange()
 * 
 * This function sets a minThreshold and maxThreshold value for the
 * light levels in your setting. Move your hand / light source / etc
 * so that your light sensor sees a full range of values. This will
 * "autoCalibrate" to your range of input values.
/*****************************************************************/

void autoRange()
{
  if (lightLevel < minThreshold)  // minThreshold was initialized to 1023 -- so, if it's less, reset the threshold level.
    minThreshold = lightLevel;

  if (lightLevel > maxThreshold)  // maxThreshold was initialized to 0 -- so, if it's bigger, reset the threshold level.
    maxThreshold = lightLevel;

  // Once we have the highest and lowest values, we can stick them
  // directly into the map() function.
  // 
  // This function must run a few times to get a good range of bright and dark values in order to work.

  lightLevel = map(lightLevel, minThreshold, maxThreshold, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
}
