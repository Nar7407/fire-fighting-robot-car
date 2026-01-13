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
 * - Bluetooth remote control (HC-05/HC-06)
 * - Dual mode operation (Manual/Autonomous)
 * 
 * Bluetooth Module Wiring:
 * HC-05/HC-06 → Arduino
 * VCC → 5V
 * GND → GND
 * TXD → RX (Pin 0)
 * RXD → TX (Pin 1)
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
#define SOUND_SPEED_CM_US 0.034  // Speed of sound in cm/microsecond
#define DEBUG_INTERVAL 1000    // Debug output interval in milliseconds

// States
enum RobotState {
  SEARCHING,
  APPROACHING_FIRE,
  EXTINGUISHING,
  STOPPED,
  MANUAL_CONTROL
};

RobotState currentState = SEARCHING;
unsigned long pumpStartTime = 0;
unsigned long lastDebugTime = 0;  // Track last debug output time
bool autonomousMode = true;  // Default to autonomous mode

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  Serial.println("🤖 Fire-Fighting Robot Initializing...");
  Serial.println("========================================");
  
  // Motor pins
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  pinMode(MOTOR_ENABLE_LEFT, OUTPUT);
  pinMode(MOTOR_ENABLE_RIGHT, OUTPUT);
  
  // Flame sensor pins (analog pins auto-configure, but explicit for clarity)
  // pinMode() not required for analog inputs, but shown for documentation
  
  // Water pump pin
  pinMode(WATER_PUMP, OUTPUT);
  digitalWrite(WATER_PUMP, LOW);
  
  // Ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Enable motors
  analogWrite(MOTOR_ENABLE_LEFT, MOTOR_SPEED);
  analogWrite(MOTOR_ENABLE_RIGHT, MOTOR_SPEED);
  
  Serial.println("✅ Robot Ready!");
  Serial.println("========================================");
  Serial.println("📱 Bluetooth Command Reference:");
  Serial.println("  F - Move Forward");
  Serial.println("  B - Move Backward");
  Serial.println("  L - Turn Left");
  Serial.println("  R - Turn Right");
  Serial.println("  S - Stop Motors");
  Serial.println("  W - Activate Water Pump (2s)");
  Serial.println("  A - Enable Autonomous Mode");
  Serial.println("  M - Enable Manual Mode");
  Serial.println("  I - Get Status Information");
  Serial.println("========================================");
  Serial.println("🔥 Starting in AUTONOMOUS mode");
  Serial.println("========================================");
  delay(2000);
}

void loop() {
  // Check for Bluetooth commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    processBluetoothCommand(cmd);
  }
  
  // Run autonomous mode if enabled
  if (autonomousMode) {
    runAutonomousMode();
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

// Process Bluetooth commands
void processBluetoothCommand(char cmd) {
  // Convert to uppercase for case-insensitive processing
  cmd = toupper(cmd);
  
  Serial.print("📱 Command received: ");
  Serial.println(cmd);
  
  switch (cmd) {
    case 'F':  // Move Forward
      autonomousMode = false;
      currentState = MANUAL_CONTROL;
      moveForward();
      Serial.println("⬆️  Moving Forward");
      break;
      
    case 'B':  // Move Backward
      autonomousMode = false;
      currentState = MANUAL_CONTROL;
      moveBackward();
      Serial.println("⬇️  Moving Backward");
      break;
      
    case 'L':  // Turn Left
      autonomousMode = false;
      currentState = MANUAL_CONTROL;
      turnLeft();
      Serial.println("⬅️  Turning Left");
      break;
      
    case 'R':  // Turn Right
      autonomousMode = false;
      currentState = MANUAL_CONTROL;
      turnRight();
      Serial.println("➡️  Turning Right");
      break;
      
    case 'S':  // Stop Motors
      autonomousMode = false;
      currentState = MANUAL_CONTROL;
      stopMotors();
      Serial.println("🛑 Motors Stopped");
      break;
      
    case 'W':  // Activate Water Pump
      Serial.println("💧 Activating Water Pump for 2 seconds...");
      digitalWrite(WATER_PUMP, HIGH);
      delay(2000);
      digitalWrite(WATER_PUMP, LOW);
      Serial.println("💧 Water Pump Deactivated");
      break;
      
    case 'A':  // Enable Autonomous Mode
      autonomousMode = true;
      currentState = SEARCHING;
      stopMotors();
      Serial.println("========================================");
      Serial.println("🔥 AUTONOMOUS MODE ENABLED");
      Serial.println("   Robot will now search for fires");
      Serial.println("========================================");
      break;
      
    case 'M':  // Enable Manual Mode
      autonomousMode = false;
      currentState = MANUAL_CONTROL;
      stopMotors();
      digitalWrite(WATER_PUMP, LOW);
      Serial.println("========================================");
      Serial.println("🎮 MANUAL MODE ENABLED");
      Serial.println("   Waiting for commands...");
      Serial.println("========================================");
      break;
      
    case 'I':  // Get Status Information
      printStatus();
      break;
      
    default:
      Serial.print("❓ Unknown command: ");
      Serial.println(cmd);
      break;
  }
}

// Print robot status information
void printStatus() {
  Serial.println("========================================");
  Serial.println("📊 ROBOT STATUS INFORMATION");
  Serial.println("========================================");
  
  // Mode
  Serial.print("Mode: ");
  Serial.println(autonomousMode ? "🔥 AUTONOMOUS" : "🎮 MANUAL");
  
  // State
  Serial.print("State: ");
  switch (currentState) {
    case SEARCHING:
      Serial.println("🔍 SEARCHING");
      break;
    case APPROACHING_FIRE:
      Serial.println("🚗 APPROACHING FIRE");
      break;
    case EXTINGUISHING:
      Serial.println("💧 EXTINGUISHING");
      break;
    case STOPPED:
      Serial.println("🛑 STOPPED");
      break;
    case MANUAL_CONTROL:
      Serial.println("🎮 MANUAL CONTROL");
      break;
  }
  
  // Sensor readings
  int flameLeft = analogRead(FLAME_SENSOR_LEFT);
  int flameCenter = analogRead(FLAME_SENSOR_CENTER);
  int flameRight = analogRead(FLAME_SENSOR_RIGHT);
  
  Serial.println("Flame Sensors:");
  Serial.print("  Left: ");
  Serial.print(flameLeft);
  Serial.println(flameLeft < FLAME_THRESHOLD ? " 🔥" : " ✓");
  Serial.print("  Center: ");
  Serial.print(flameCenter);
  Serial.println(flameCenter < FLAME_THRESHOLD ? " 🔥" : " ✓");
  Serial.print("  Right: ");
  Serial.print(flameRight);
  Serial.println(flameRight < FLAME_THRESHOLD ? " 🔥" : " ✓");
  
  // Distance
  int distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  Serial.println("========================================");
}

// Autonomous mode operation
void runAutonomousMode() {
  // Read flame sensors
  int flameLeft = analogRead(FLAME_SENSOR_LEFT);
  int flameCenter = analogRead(FLAME_SENSOR_CENTER);
  int flameRight = analogRead(FLAME_SENSOR_RIGHT);
  
  // Check for fire detection
  bool fireDetectedLeft = flameLeft < FLAME_THRESHOLD;
  bool fireDetectedCenter = flameCenter < FLAME_THRESHOLD;
  bool fireDetectedRight = flameRight < FLAME_THRESHOLD;
  bool fireDetected = fireDetectedLeft || fireDetectedCenter || fireDetectedRight;
  
  // Debug output (throttled to avoid flooding Serial Monitor)
  if (millis() - lastDebugTime >= DEBUG_INTERVAL) {
    Serial.print("Sensors - L:");
    Serial.print(flameLeft);
    Serial.print(" C:");
    Serial.print(flameCenter);
    Serial.print(" R:");
    Serial.print(flameRight);
    Serial.print(" | Fire: ");
    Serial.println(fireDetected ? "YES" : "NO");
    lastDebugTime = millis();
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
      
    case MANUAL_CONTROL:
      // In manual mode, do nothing (controlled by commands)
      break;
  }
}

// Ultrasonic sensor distance measurement
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Timeout after 30ms (30000 microseconds) - allows measuring up to ~5m distance
  long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  
  if (duration == 0) {
    return -1;  // No echo received
  }
  
  int distance = duration * SOUND_SPEED_CM_US / 2;  // Convert to cm (divide by 2 for round trip)
  return distance;
}
