# Engr100-Solar-Tracker

Final solar tracker project for Engr 100.
This program powers an Arduino to angle a solar panel towards a moving light source. The system uses 4 photoresistors and 2 servo motors, and can output diagnostic and periodic usage data over USB or Bluetooth serial.

## Axis Alignment

Sensors and rotational axes are configured with a cardinal directioin system. North/South corresponds to up/down, which changes the angle between the ground and the panel. East/West corresponds to right/left, which rotates the whole apparatus about the z axis parallel to the ground. The cardinal directions are aligned as such:

```plaintext
  N                (+)
W-+-E    -->    (-)-+-(+)
  S                (-)
```

## Movement Algorithm

The servo position is updated using a simple error correction algorithm. The raw input from two opposite photoresistors is compared, and every cycle the servo motor on that axis will move one tick in the brighter direction.

## Bluetooth Serial Output

By default the program will send debugging and serial outputs to the default port, which goes to pin 13 and to the USB port. When bluetooth is on, this data will also be sent to the specified bluetooth port. This code is tested to work on a HC-05 Serial Bluetooth Module v1.3.
