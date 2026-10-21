# Stage 3: Servo and Ultrasonic Sensor Testing

## Objective
To connect and test a servo-mounted HC-SR04 ultrasonic sensor for distance measurement at different angles.

## Connections

### Servo Motor
- Signal: Arduino D3
- Power: Arduino 5V
- Ground: Arduino GND

### HC-SR04 Ultrasonic Sensor
- TRIG: Arduino D7
- ECHO: Arduino D4
- VCC: Arduino 5V
- GND: Arduino GND

## Testing Procedure
1. Connected the servo motor and ultrasonic sensor to the Arduino Uno.
2. Programmed the servo to move to 0°, 90°, and 180°.
3. Measured the distance at each angle using the ultrasonic sensor.
4. Observed the readings in the Serial Monitor.

## Result
The servo motor and ultrasonic distance sensor operated successfully in the Tinkercad simulation. Distance measurements were displayed in the Serial Monitor.

## Next Step
Implement scanning at 15° intervals and record the angle and distance measurements for occupancy mapping.
