#include <NewPing.h>

// Define pins for the HC-SR04 sensors
#define TRIG_PIN_FRONT 5
#define ECHO_PIN_FRONT 4
#define TRIG_PIN_LEFT 12
#define ECHO_PIN_LEFT 13
#define TRIG_PIN_RIGHT 2
#define ECHO_PIN_RIGHT 3

// Motor control pins
#define MOTOR_LEFT_FORWARD 9
#define MOTOR_LEFT_BACKWARD 10
#define MOTOR_RIGHT_FORWARD 7
#define MOTOR_RIGHT_BACKWARD 8
#define EnA 11
#define EnB 6


// Define maximum distance in cm for walls
#define WALL_DISTANCE 5

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
  analogWrite(ENR,255);
  analogWrite(ENL,255);

  while(sonarRight.ping_cm() < WALL_DISTANCE && sonarRight.ping_cm() > 0){
    if(sonarFront.ping_cm() > 2){
      moveForward();
    }else if(sonarLeft.ping_cm() > WALL_DISTANCE){
      stopRobot();
      turnLeft();
    }else {
      stopRobot();
      turn180();
    }
  }
  stopRobot();
  turnRight();
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  
  //distances has been rounded to obtain smoother movement.
  if((int)sonarRight.ping_cm()>(int)sonarLeft.ping_cm()) analogWrite(ENR,150); //We don't need to specifiy speed of the other motor because we already define it at at the beginning of the loop
  else if((int)sonarLeft.ping_cm()>(int)sonarRight.ping_cm())analogWrite(ENL,150);
  delay(200);
}

void stopRobot() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  delay(400);  // Turn for a fixed duration
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  delay(400);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
  delay(400);  // Turn for a fixed duration
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  delay(400);
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
}

void turn180() {
  while(sonarFront.ping_cm() < 17){
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  }
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  delay(400);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
}
