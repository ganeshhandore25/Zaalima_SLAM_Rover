
#include <Servo.h>
#include <math.h>

#define ENA 5
#define IN1 8
#define IN2 7

#define ENB 6
#define IN3 4
#define IN4 3

#define TRIG 10
#define ECHO 11

#define GRID_SIZE 10
#define CELL_SIZE 5
#define MAX_DISTANCE 50
#define OBSTACLE_THRESHOLD 20

Servo scanner;

int grid[GRID_SIZE][GRID_SIZE] = {0};

float distance;
float leftDistance;
float rightDistance;

int motorSpeed = 180;

float getDistance()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);

  if (duration == 0)
    return -1;

  return duration * 0.0343 / 2;
}

void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void backward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void left()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void right()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void stopRobot()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void scanSides()
{
  scanner.write(150);
  delay(400);
  leftDistance = getDistance();

  scanner.write(30);
  delay(400);
  rightDistance = getDistance();

  scanner.write(90);
  delay(300);
}

void updateMap(int angle, float distance)
{
  if (distance <= 0 || distance > MAX_DISTANCE)
    return;

  float rad = angle * PI / 180.0;

  float x = distance * cos(rad);
  float y = distance * sin(rad);

  int col = round(x / CELL_SIZE) + 5;
  int row = round(y / CELL_SIZE);

  if (row >= 0 && row < GRID_SIZE &&
      col >= 0 && col < GRID_SIZE)
  {
    grid[row][col] = 1;
  }

  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(" Distance: ");
  Serial.print(distance);
  Serial.print(" X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.println(y);
}

void scanArea()
{
  for (int angle = 0; angle <= 180; angle += 15)
  {
    scanner.write(angle);
    delay(250);

    float d = getDistance();

    updateMap(angle, d);
  }

  scanner.write(90);
  delay(250);
}

void displayGrid()
{
  Serial.println("OCCUPANCY GRID");

  for (int row = GRID_SIZE - 1; row >= 0; row--)
  {
    for (int col = 0; col < GRID_SIZE; col++)
    {
      Serial.print(grid[row][col]);
      Serial.print(" ");
    }

    Serial.println();
  }
}

void avoidObstacle()
{
  stopRobot();
  delay(300);

  backward();
  delay(400);

  stopRobot();
  scanSides();

  Serial.print("Left Distance: ");
  Serial.println(leftDistance);

  Serial.print("Right Distance: ");
  Serial.println(rightDistance);

  if (leftDistance < 0)
    leftDistance = 0;

  if (rightDistance < 0)
    rightDistance = 0;

  if (leftDistance > rightDistance && leftDistance > OBSTACLE_THRESHOLD)
  {
    left();
    delay(600);
  }
  else if (rightDistance > OBSTACLE_THRESHOLD)
  {
    right();
    delay(600);
  }
  else
  {
    backward();
    delay(800);
  }

  stopRobot();
  scanner.write(90);
}

void setup()
{
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  scanner.attach(9);
  scanner.write(90);

  stopRobot();

  delay(1000);

  Serial.println("AUTONOMOUS SLAM ROVER STARTED");
}

void loop()
{
  distance = getDistance();

  Serial.print("Front Distance: ");
  Serial.println(distance);

  if (distance < 0 || distance > OBSTACLE_THRESHOLD)
  {
    forward();
  }
  else
  {
    avoidObstacle();
  }

  scanArea();
  displayGrid();

  delay(300);
}
