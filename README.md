# Engr100-Solar-Tracker

This program powers an Arduino to angle a solar panel towards a moving light source. The system uses 4 photoresistors and 2 servo motors, and can output diagnostic and ambient environment data over USB or Bluetooth serial.

## Contents

- [About The Project](#about-the-project)
- [Features](#features)
  - [Axis Alignment](#axis-alignment)
  - [Movement Algorithm](#movement-algorithm)
  - [Environmental Data Reporting](#environmental-data-reporting)
- [Set Up VS Code for Arduino](#set-up-vs-code-for-arduino)

## About The Project

This Solar Tracker project was built by Dan Erickson, Caroline Collins, John Keane, and Juliana Rodich for the Winter 2021 Engr 100-420 course at the University of Michigan. The device angles a solar cell towards the sun in order to optimize power generation, according to the specifications in ```Archive - Solar Tracker Rubric Winter 2021.pdf```.

There are three other files in this repo called ```Archive - <description>``` which showcase the functionality of the solar tracker and this program. The final report and the project rubric are the best places to look if you want to learn more about the project.

## Features

Technical details about the functions and algorithms of the program are detailed below.

### Axis Alignment

Sensors and rotational axes are configured with a cardinal directioin system. North/South corresponds to up/down, which changes the angle between the ground and the panel. East/West corresponds to right/left and clockwise/counterclockwise, which rotates the whole apparatus about the z axis parallel to the ground. The cardinal directions are aligned as such:

```plaintext
  N                 (180)
W-+-E    -->    ( 0 )-+-(180)
  S                 ( 0 )
```

### Movement Algorithm

The servo position is updated using a simple error correction structure. The raw inputs from both photoresistors on an axis are compared, and if the percent difference is higher than the configured threshold then the servo motor will move one step in the brighter direction.

### Environmental Data Reporting

When switched on, the program can report ambient environment data over the serial port. The data includes ambient temperature, incident solar irradiance, and relative sun position. This data is sent as a serial data stream at 115200 baud and will send over both USB serial and bluetooth serial.

## Set Up VS Code for Arduino

The Arduino IDE is extremely limited in functionality, so it is useful to use VS Code for example. To develop Arduino programs on VS Code, install the following extensions:

- Arduino
- C/C++

Then run the following commands (hit ```CMD + Shift + P``` or ```Ctrl + Shift + P```):

```plaintext
>Arduino: Initialize
>Arduino: Rebuild IntelliSense Configuration
```

Choose ```Arduino Uno``` for the board type, and ```AVR ISP``` for the programmer. If you get error messages about ```Serial``` class, then add the following to ```.vscode/c_cpp_properties.json``` in ```defines```:

```plaintext
...
"cppStandard": "c++11",
"defines": [
  "USBCON",
  ...
]
```
