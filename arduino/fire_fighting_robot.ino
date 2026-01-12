/*
 * Fire Fighting Robot Car - Arduino Version
 * Supports both autonomous fire detection and remote control
 * Author: Nar7407
 * Date: 2026-01-12
 */

#include <Servo.h>
#include "config.h"

// Motor control pins
#define MOTOR_LEFT_FWD 5
#define MOTOR_LEFT_BWD 6
#define MOTOR_RIGHT_FWD 9
#define MOTOR_RIGHT_BWD 10
#define MOTOR_LEFT_EN 3
#define MOTOR_RIGHT_EN 11

// Flame sensor pins
#define FLAME_LEFT A0
#define FLAME_CENTER A1
#define FLAME_RIGHT A2

// Ultrasonic sensor pins
#define TRIG_PIN 12
#define ECHO_PIN 13

// Other components
#define PUMP_PIN 7
#define SERVO_PIN 4
#define BUZZER_PIN 8

// Servo object
Servo scanServo;

// Variables
int flameLeft, flameCenter, flameRight;
long distance;
bool autonomousMode = true;
char command;

// Threshold for flame detection (lower value = flame detected)
const int FLAME_THRESHOLD = 500;
const int OBSTACLE_DISTANCE = 20; // cm
const int FIRE_NEAR_DISTANCE = 15; // cm

void setup() {
  Serial.begin(9600); // Bluetooth communication
  
  // Motor pins
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);
  pinMode(MOTOR_LEFT_EN, OUTPUT);
  pinMode(MOTOR_RIGHT_EN, OUTPUT);
  
  // Sensor pins
  pinMode(FLAME_LEFT, INPUT);
  pinMode(FLAME_CENTER, INPUT);
  pinMode(FLAME_RIGHT, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Other components
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  // Servo setup
  scanServo.attach(SERVO_PIN);
  scanServo.write(90); // Center position
  
  // Startup sequence
  beep(2);
  delay(1000);
}

void loop() {
  // Check for Bluetooth commands
  if (Serial.available() > 0) {
    command = Serial.read();
    processCommand(command);
  }
  
  if (autonomousMode) {
    autonomousOperation();
  }
  
  delay(50);
}

void autonomousOperation() {
  // Read flame sensors
  flameLeft = analogRead(FLAME_LEFT);
  flameCenter = analogRead(FLAME_CENTER);
  flameRight = analogRead(FLAME_RIGHT);
  
  // Check for obstacles
  distance = getDistance();
  
  // Fire detection logic
  if (flameCenter < FLAME_THRESHOLD || flameLeft < FLAME_THRESHOLD || flameRight < FLAME_THRESHOLD) {
    beep(1);
    
    if (distance < FIRE_NEAR_DISTANCE) {
      // Fire is very close - stop and extinguish
      stopMotors();
      extinguishFire();
    } else if (distance < OBSTACLE_DISTANCE) {
      // Obstacle detected - avoid it
      avoidObstacle();
    } else {
      // Navigate towards fire
      navigateToFire();
    }
  } else {
    // No fire detected - patrol
    patrol();
  }
}

void navigateToFire() {
  if (flameCenter < FLAME_THRESHOLD) {
    // Fire straight ahead
    moveForward(200);
  } else if (flameLeft < FLAME_THRESHOLD) {
    // Fire on left
    turnLeft(150);
  } else if (flameRight < FLAME_THRESHOLD) {
    // Fire on right
    turnRight(150);
  }
}

void extinguishFire() {
  Serial.println("Extinguishing fire!");
  digitalWrite(PUMP_PIN, HIGH);
  beep(3);
  delay(3000); // Pump on for 3 seconds
  digitalWrite(PUMP_PIN, LOW);
  
  // Move back
  moveBackward(200);
  delay(1000);
  stopMotors();
}

void patrol() {
  // Check for obstacles while patrolling
  if (distance < OBSTACLE_DISTANCE) {
    avoidObstacle();
  } else {
    moveForward(150);
  }
}

void avoidObstacle() {
  stopMotors();
  delay(300);
  moveBackward(200);
  delay(500);
  
  // Scan left and right
  int leftDist = scanDirection(45);
  int rightDist = scanDirection(135);
  
  scanServo.write(90); // Return to center
  
  if (leftDist > rightDist) {
    turnLeft(200);
    delay(500);
  } else {
    turnRight(200);
    delay(500);
  }
}

int scanDirection(int angle) {
  scanServo.write(angle);
  delay(300);
  return getDistance();
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  long dist = duration * 0.034 / 2; // Convert to cm
  
  return dist;
}

void processCommand(char cmd) {
  Serial.print("Command: ");
  Serial.println(cmd);
  
  switch(cmd) {
    case 'F': // Forward
      autonomousMode = false;
      moveForward(200);
      break;
    case 'B': // Backward
      autonomousMode = false;
      moveBackward(200);
      break;
    case 'L': // Left
      autonomousMode = false;
      turnLeft(200);
      break;
    case 'R': // Right
      autonomousMode = false;
      turnRight(200);
      break;
    case 'S': // Stop
      autonomousMode = false;
      stopMotors();
      break;
    case 'W': // Water pump
      digitalWrite(PUMP_PIN, HIGH);
      delay(2000);
      digitalWrite(PUMP_PIN, LOW);
      break;
    case 'A': // Autonomous mode
      autonomousMode = true;
      Serial.println("Autonomous mode ON");
      break;
    case 'M': // Manual mode
      autonomousMode = false;
      stopMotors();
      Serial.println("Manual mode ON");
      break;
    default:
      break;
  }
}

void moveForward(int speed) {
  analogWrite(MOTOR_LEFT_EN, speed);
  analogWrite(MOTOR_RIGHT_EN, speed);
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void moveBackward(int speed) {
  analogWrite(MOTOR_LEFT_EN, speed);
  analogWrite(MOTOR_RIGHT_EN, speed);
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, HIGH);
}

void turnLeft(int speed) {
  analogWrite(MOTOR_LEFT_EN, speed);
  analogWrite(MOTOR_RIGHT_EN, speed);
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void turnRight(int speed) {
  analogWrite(MOTOR_LEFT_EN, speed);
  analogWrite(MOTOR_RIGHT_EN, speed);
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, HIGH);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
  analogWrite(MOTOR_LEFT_EN, 0);
  analogWrite(MOTOR_RIGHT_EN, 0);
}

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}