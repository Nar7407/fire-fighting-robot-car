/*
 * Fire-Fighting Robot Car
 * 
 * A mobile robot that detects fire using flame sensors and automatically
 * sprays water to extinguish it.
 * 
 * Features:
 * - Autonomous movement with obstacle avoidance
 * - Flame detection using IR flame sensors
 * - Automatic water pump activation
 * - Emergency stop functionality
 */

// Motor Driver Pins (L298N)
#define MOTOR_LEFT_FORWARD 5
#define MOTOR_LEFT_BACKWARD 6
#define MOTOR_RIGHT_FORWARD 9
#define MOTOR_RIGHT_BACKWARD 10
#define MOTOR_ENABLE_LEFT 11
#define MOTOR_ENABLE_RIGHT 3

// Flame Sensor Pins
#define FLAME_SENSOR_LEFT A0
#define FLAME_SENSOR_CENTER A1
#define FLAME_SENSOR_RIGHT A2

// Water Pump Pin
#define WATER_PUMP 7

// Ultrasonic Sensor Pins (for obstacle detection)
#define TRIG_PIN 12
#define ECHO_PIN 13

// Constants
#define FLAME_THRESHOLD 512    // Analog threshold for flame detection (lower value = flame detected)
#define SAFE_DISTANCE 20       // Safe distance from obstacles in cm
#define MOTOR_SPEED 200        // PWM value for motor speed (0-255)
#define PUMP_DURATION 3000     // Time to run pump in milliseconds

// States
enum RobotState {
  SEARCHING,
  APPROACHING_FIRE,
  EXTINGUISHING,
  STOPPED
};

RobotState currentState = SEARCHING;
unsigned long pumpStartTime = 0;

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  Serial.println("Fire-Fighting Robot Initializing...");
  
  // Motor pins
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  pinMode(MOTOR_ENABLE_LEFT, OUTPUT);
  pinMode(MOTOR_ENABLE_RIGHT, OUTPUT);
  
  // Flame sensor pins
  pinMode(FLAME_SENSOR_LEFT, INPUT);
  pinMode(FLAME_SENSOR_CENTER, INPUT);
  pinMode(FLAME_SENSOR_RIGHT, INPUT);
  
  // Water pump pin
  pinMode(WATER_PUMP, OUTPUT);
  digitalWrite(WATER_PUMP, LOW);
  
  // Ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Enable motors
  analogWrite(MOTOR_ENABLE_LEFT, MOTOR_SPEED);
  analogWrite(MOTOR_ENABLE_RIGHT, MOTOR_SPEED);
  
  Serial.println("Robot Ready!");
  delay(2000);
}

void loop() {
  // Read flame sensors
  int flameLeft = analogRead(FLAME_SENSOR_LEFT);
  int flameCenter = analogRead(FLAME_SENSOR_CENTER);
  int flameRight = analogRead(FLAME_SENSOR_RIGHT);
  
  // Check for fire detection
  bool fireDetectedLeft = flameLeft < FLAME_THRESHOLD;
  bool fireDetectedCenter = flameCenter < FLAME_THRESHOLD;
  bool fireDetectedRight = flameRight < FLAME_THRESHOLD;
  bool fireDetected = fireDetectedLeft || fireDetectedCenter || fireDetectedRight;
  
  // Debug output
  if (Serial.available() == 0) {  // Only print when no serial input
    Serial.print("Sensors - L:");
    Serial.print(flameLeft);
    Serial.print(" C:");
    Serial.print(flameCenter);
    Serial.print(" R:");
    Serial.print(flameRight);
    Serial.print(" | Fire: ");
    Serial.println(fireDetected ? "YES" : "NO");
  }
  
  // State machine
  switch (currentState) {
    case SEARCHING:
      if (fireDetected) {
        Serial.println("Fire detected! Approaching...");
        currentState = APPROACHING_FIRE;
      } else {
        searchForFire();
      }
      break;
      
    case APPROACHING_FIRE:
      if (fireDetectedCenter) {
        // Fire directly ahead, check distance
        int distance = getDistance();
        if (distance < SAFE_DISTANCE && distance > 0) {
          Serial.println("Close enough! Starting extinguishing...");
          stopMotors();
          currentState = EXTINGUISHING;
          pumpStartTime = millis();
          digitalWrite(WATER_PUMP, HIGH);
        } else {
          moveForward();
        }
      } else if (fireDetectedLeft) {
        turnLeft();
      } else if (fireDetectedRight) {
        turnRight();
      } else {
        // Lost the fire signal
        Serial.println("Fire signal lost. Returning to search mode.");
        currentState = SEARCHING;
      }
      break;
      
    case EXTINGUISHING:
      // Run pump for specified duration
      if (millis() - pumpStartTime >= PUMP_DURATION) {
        digitalWrite(WATER_PUMP, LOW);
        Serial.println("Extinguishing complete. Checking for fire...");
        delay(1000);
        
        // Re-check for fire
        flameCenter = analogRead(FLAME_SENSOR_CENTER);
        if (flameCenter >= FLAME_THRESHOLD) {
          Serial.println("Fire extinguished! Returning to search mode.");
          currentState = SEARCHING;
        } else {
          Serial.println("Fire still detected. Continuing to extinguish...");
          pumpStartTime = millis();
          digitalWrite(WATER_PUMP, HIGH);
        }
      }
      break;
      
    case STOPPED:
      stopMotors();
      digitalWrite(WATER_PUMP, LOW);
      break;
  }
  
  delay(100);  // Small delay for stability
}

// Movement Functions
void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

// Search pattern - rotate slowly to scan for fire
void searchForFire() {
  static unsigned long lastTurnTime = 0;
  static bool turningRight = true;
  
  if (millis() - lastTurnTime > 500) {  // Change direction every 500ms
    turningRight = !turningRight;
    lastTurnTime = millis();
  }
  
  if (turningRight) {
    turnRight();
  } else {
    turnLeft();
  }
}

// Ultrasonic sensor distance measurement
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // Timeout after 30ms
  
  if (duration == 0) {
    return -1;  // No echo received
  }
  
  int distance = duration * 0.034 / 2;  // Convert to cm
  return distance;
}
