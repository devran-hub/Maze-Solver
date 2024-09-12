#include <NewPing.h>

// Define pins for the HC-SR04 sensors
#define TRIG_PIN_FRONT 7
#define ECHO_PIN_FRONT 6
#define TRIG_PIN_LEFT 5
#define ECHO_PIN_LEFT 4
#define TRIG_PIN_RIGHT 3
#define ECHO_PIN_RIGHT 2

// Motor control pins
#define MOTOR_LEFT_FORWARD 9
#define MOTOR_LEFT_BACKWARD 8
#define MOTOR_RIGHT_FORWARD 11
#define MOTOR_RIGHT_BACKWARD 10


// White circle detection sensor


// Define maximum distance in cm for walls
#define WALL_DISTANCE 1

NewPing sonarFront(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
NewPing sonarLeft(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
NewPing sonarRight(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);


void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
}

void loop() {
  if (sonarFront.ping_cm() > WALL_DISTANCE) {
    moveForward();
    if (sonarRight.ping_cm() < WALL_DISTANCE) turnLeft();
    else if (sonarLeft.ping_cm() < WALL_DISTANCE) turnRight();

  };
    else {
    turnRight();
  };
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  delay(500);  // Move for a fixed duration, adjust timing
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
}

// Turn the robot left
void turnLeft() {
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  delay(400);  // Turn for a fixed duration
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
}

// Turn the robot right
void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
  delay(400);  // Turn for a fixed duration
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}
