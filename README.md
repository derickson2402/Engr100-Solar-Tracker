# Engr100-Solar-Tracker

Final solar tracker project for Engr 100.
This program powers an Arduino to angle a solar panel towards a moving light source. The system uses 4 photoresistors and 2 servo motors, and can output diagnostic and ambient environment data over USB or Bluetooth serial.

## Axis Alignment

Sensors and rotational axes are configured with a cardinal directioin system. North/South corresponds to up/down, which changes the angle between the ground and the panel. East/West corresponds to right/left, which rotates the whole apparatus about the z axis parallel to the ground. The cardinal directions are aligned as such:

```plaintext
  N                (+)
W-+-E    -->    (-)-+-(+)
  S                (-)
```

## Movement Algorithm

The servo position is updated using a simple error correction structure. The raw inputs from both photoresistors on an axis are compared, and if the percent difference is higher than the configured threshold then the servo motor will move one step in the brighter direction.

## Bluetooth Serial Output

When switched on, the program can send ambient environment data over Bluetooth serial. The data includes ambient temperature, incident solar irradiance, and relative sun position. This code is tested to work with a HC-05 Serial Bluetooth Module v1.3, but uses a standart 9600 baud serial connection and should work with any serial connection.
