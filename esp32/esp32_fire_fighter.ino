/*
 * Fire Fighting Robot Car - ESP32 Version
 * With WiFi and Bluetooth support
 * Author: Nar7407
 * Date: 2026-01-12
 */

#include <ESP32Servo.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
Servo scanServo;

// Motor pins
#define MOTOR_LEFT_FWD 16
#define MOTOR_LEFT_BWD 17
#define MOTOR_RIGHT_FWD 18
#define MOTOR_RIGHT_BWD 19
#define MOTOR_LEFT_EN 25
#define MOTOR_RIGHT_EN 26

// Flame sensors
#define FLAME_LEFT 34
#define FLAME_CENTER 35
#define FLAME_RIGHT 32

// Ultrasonic
#define TRIG_PIN 5
#define ECHO_PIN 18

// Other
#define PUMP_PIN 27
#define SERVO_PIN 13
#define BUZZER_PIN 14

// PWM settings
const int freq = 5000;
const int leftChannel = 0;
const int rightChannel = 1;
const int resolution = 8;

bool autonomousMode = true;
const int FLAME_THRESHOLD = 2000; // ESP32 uses 12-bit ADC
const int OBSTACLE_DISTANCE = 20;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("FireFighter_ESP32");
  
  // Motor setup
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);
  
  ledcSetup(leftChannel, freq, resolution);
  ledcSetup(rightChannel, freq, resolution);
  ledcAttachPin(MOTOR_LEFT_EN, leftChannel);
  ledcAttachPin(MOTOR_RIGHT_EN, rightChannel);
  
  // Sensors
  pinMode(FLAME_LEFT, INPUT);
  pinMode(FLAME_CENTER, INPUT);
  pinMode(FLAME_RIGHT, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Other components
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  scanServo.attach(SERVO_PIN);
  scanServo.write(90);
  
  Serial.println("ESP32 Fire Fighting Robot Initialized");
  beep(2);
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    processCommand(cmd);
  }
  
  if (autonomousMode) {
    autonomousOperation();
  }
  
  delay(50);
}

void autonomousOperation() {
  int flameLeft = analogRead(FLAME_LEFT);
  int flameCenter = analogRead(FLAME_CENTER);
  int flameRight = analogRead(FLAME_RIGHT);
  
  long distance = getDistance();
  
  if (flameCenter < FLAME_THRESHOLD || flameLeft < FLAME_THRESHOLD || flameRight < FLAME_THRESHOLD) {
    beep(1);
    
    if (distance < 15) {
      stopMotors();
      extinguishFire();
    } else if (distance < OBSTACLE_DISTANCE) {
      avoidObstacle();
    } else {
      navigateToFire(flameLeft, flameCenter, flameRight);
    }
  } else {
    patrol(distance);
  }
}

void navigateToFire(int left, int center, int right) {
  if (center < FLAME_THRESHOLD) {
    moveForward(200);
  } else if (left < FLAME_THRESHOLD) {
    turnLeft(150);
  } else if (right < FLAME_THRESHOLD) {
    turnRight(150);
  }
}

void extinguishFire() {
  SerialBT.println("Extinguishing fire!");
  digitalWrite(PUMP_PIN, HIGH);
  beep(3);
  delay(3000);
  digitalWrite(PUMP_PIN, LOW);
  
  moveBackward(200);
  delay(1000);
  stopMotors();
}

void patrol(long distance) {
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
  
  scanServo.write(45);
  delay(300);
  int leftDist = getDistance();
  
  scanServo.write(135);
  delay(300);
  int rightDist = getDistance();
  
  scanServo.write(90);
  
  if (leftDist > rightDist) {
    turnLeft(200);
  } else {
    turnRight(200);
  }
  delay(500);
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void processCommand(char cmd) {
  switch(cmd) {
    case 'F':
      autonomousMode = false;
      moveForward(200);
      break;
    case 'B':
      autonomousMode = false;
      moveBackward(200);
      break;
    case 'L':
      autonomousMode = false;
      turnLeft(200);
      break;
    case 'R':
      autonomousMode = false;
      turnRight(200);
      break;
    case 'S':
      autonomousMode = false;
      stopMotors();
      break;
    case 'W':
      digitalWrite(PUMP_PIN, HIGH);
      delay(2000);
      digitalWrite(PUMP_PIN, LOW);
      break;
    case 'A':
      autonomousMode = true;
      SerialBT.println("Autonomous ON");
      break;
    case 'M':
      autonomousMode = false;
      stopMotors();
      SerialBT.println("Manual ON");
      break;
  }
}

void moveForward(int speed) {
  ledcWrite(leftChannel, speed);
  ledcWrite(rightChannel, speed);
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void moveBackward(int speed) {
  ledcWrite(leftChannel, speed);
  ledcWrite(rightChannel, speed);
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, HIGH);
}

void turnLeft(int speed) {
  ledcWrite(leftChannel, speed);
  ledcWrite(rightChannel, speed);
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void turnRight(int speed) {
  ledcWrite(leftChannel, speed);
  ledcWrite(rightChannel, speed);
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
  ledcWrite(leftChannel, 0);
  ledcWrite(rightChannel, 0);
}

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}