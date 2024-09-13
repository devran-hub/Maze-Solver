#include <NewPing.h>  // Library to control the HC-SR04 ultrasonic sensor

// Define pin connections for ultrasonic sensors
#define TRIG_PIN_LEFT 2
#define ECHO_PIN_LEFT 3
#define TRIG_PIN_FRONT 4
#define ECHO_PIN_FRONT 5
#define TRIG_PIN_RIGHT 2
#define ECHO_PIN_RIGHT 7
#define MAX_DISTANCE 200 // Maximum distance to measure (in cm)

// Motor control pins
#define MOTOR_LEFT_FORWARD 6
#define MOTOR_LEFT_BACKWARD 5
#define MOTOR_RIGHT_FORWARD 3
#define MOTOR_RIGHT_BACKWARD 4

NewPing sonarLeft(TRIG_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE);
NewPing sonarFront(TRIG_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE);
NewPing sonarRight(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE);

// Custom queue implementation
const int MAX_QUEUE_SIZE = 100;  // Max size of the queue
int pathQueue[MAX_QUEUE_SIZE];   // Array to store path
int front = -1, rear = -1;       // Pointers for the queue

// Variables to track the state of the maze solving
bool firstRun = true;
bool pathMemorized = false;

// Queue Functions
bool isQueueEmpty() {
  return (front == -1);
}

bool isQueueFull() {
  return (rear == MAX_QUEUE_SIZE - 1);
}

void enqueue(int value) {
  if (!isQueueFull()) {
    if (front == -1) {
      front = 0;
    }
    rear++;
    pathQueue[rear] = value;
  }
}

int dequeue() {
  if (!isQueueEmpty()) {
    int value = pathQueue[front];
    if (front == rear) {
      front = rear = -1;  // Reset the queue
    } else {
      front++;
    }
    return value;
  }
  return -1;  // Return -1 if the queue is empty
}

// Function to move robot forward
void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

// Function to turn robot left
void turnLeft() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

// Function to turn robot right
void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

// Function to stop the robot
void stopRobot() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

// Function to detect distance using ultrasonic sensor
int getDistance(NewPing sonar) {
  return sonar.ping_cm();  // Get distance in cm
}

// Function to check the surroundings and make decisions
int checkSurroundings() {
  int leftDist = getDistance(sonarLeft);
  int frontDist = getDistance(sonarFront);
  int rightDist = getDistance(sonarRight);

  if (frontDist > 20) {
    // If no obstacle in front, move forward
    moveForward();
    return 0;  // 0 represents moving forward
  } else if (leftDist > 20) {
    // If no obstacle on the left, turn left
    turnLeft();
    return 1;  // 1 represents turning left
  } else if (rightDist > 20) {
    // If no obstacle on the right, turn right
    turnRight();
    return 2;  // 2 represents turning right
  } else {
    // If surrounded by walls, backtrack
    stopRobot();
    return -1; // -1 represents dead end
  }
}

// Setup function for the Arduino
void setup() {
  // Set up motor control pins
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

// Loop function for the Arduino
void loop() {
  if (firstRun) {
    // First run - explore the maze and memorize the path
    int decision = checkSurroundings();
    if (decision != -1) {
      // Store valid decision in the queue
      enqueue(decision);
    } else {
      // Backtrack when a dead end is found
      while (!isQueueEmpty()) {
        int lastMove = dequeue();
        // Execute opposite movement to backtrack
        if (lastMove == 0) {
          // If last move was forward, stop and find new path
          stopRobot();
        } else if (lastMove == 1) {
          // If last move was left, turn right to backtrack
          turnRight();
        } else if (lastMove == 2) {
          // If last move was right, turn left to backtrack
          turnLeft();
        }
      }
      firstRun = false; // Complete first run
      pathMemorized = true; // Memorize the fastest path
    }
  } else if (pathMemorized) {
    // Second run - use the memorized path
    while (!isQueueEmpty()) {
      int nextMove = dequeue();
      if (nextMove == 0) {
        moveForward();
      } else if (nextMove == 1) {
        turnLeft();
      } else if (nextMove == 2) {
        turnRight();
      }
    }
    stopRobot(); // Stop once the maze is solved
    delay(5000); // Pause before restarting
  }
}
