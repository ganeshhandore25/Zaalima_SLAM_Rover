
#include <Servo.h>

Servo scanner;

const int servoPin = 3;
const int trigPin = 7;
const int echoPin = 4;
const int scanStep = 15;

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration =
      pulseIn(echoPin, HIGH, 30000UL);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

void setup() {
  Serial.begin(9600);
  scanner.attach(servoPin);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Angle_deg,Distance_cm");
}

void loop() {
  for (int angle = 0; angle <= 180; angle += scanStep) {
    scanner.write(angle);
    delay(300);

    float distance = measureDistance();

    Serial.print(angle);
    Serial.print(",");

    if (distance < 0) {
      Serial.println("No echo");
    } else {
      Serial.println(distance, 2);
    }
  }

  for (int angle = 165; angle >= 15; angle -= scanStep) {
    scanner.write(angle);
    delay(300);

    float distance = measureDistance();

    Serial.print(angle);
    Serial.print(",");

    if (distance < 0) {
      Serial.println("No echo");
    } else {
      Serial.println(distance, 2);
    }
  }
}
