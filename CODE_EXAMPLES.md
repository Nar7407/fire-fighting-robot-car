# Code Examples - Fire-Fighting Robot Car

This file contains code snippets and examples for customizing and enhancing your fire-fighting robot car.

## 📱 Remote Control Features

### Example 1: Bluetooth Control

Add manual override with HC-05/HC-06 Bluetooth module.

**Wiring:**
- HC-05 TX → Arduino RX (D0)
- HC-05 RX → Arduino TX (D1)
- HC-05 VCC → 5V
- HC-05 GND → GND

**Code:**
```cpp
// Add at top of main code
char bluetoothCommand = 'S';  // S = Stop (default)

void setup() {
  Serial.begin(9600);  // For Bluetooth communication
  // ... rest of setup
}

void loop() {
  // Check for Bluetooth commands
  if (Serial.available() > 0) {
    bluetoothCommand = Serial.read();
  }
  
  // Manual control mode
  if (bluetoothCommand != 'A') {  // 'A' = Autonomous mode
    handleManualControl();
    return;  // Skip autonomous logic
  }
  
  // ... rest of autonomous code
}

void handleManualControl() {
  switch (bluetoothCommand) {
    case 'F':  // Forward
      moveForward();
      break;
    case 'B':  // Backward
      moveBackward();
      break;
    case 'L':  // Left
      turnLeft();
      break;
    case 'R':  // Right
      turnRight();
      break;
    case 'S':  // Stop
      stopMotors();
      break;
    case 'P':  // Pump ON
      digitalWrite(WATER_PUMP, HIGH);
      break;
    case 'p':  // Pump OFF
      digitalWrite(WATER_PUMP, LOW);
      break;
    case 'A':  // Autonomous mode
      // Switch to autonomous operation
      break;
  }
}
```

**Android App Commands:**
- 'F' = Forward
- 'B' = Backward
- 'L' = Left
- 'R' = Right
- 'S' = Stop
- 'P' = Pump On
- 'p' = Pump Off
- 'A' = Autonomous mode

### Example 2: IR Remote Control

Use standard TV remote for control.

**Additional Hardware:**
- IR Receiver (TSOP1838 or similar)
- Connect to Arduino D2

**Library:** IRremote

```cpp
#include <IRremote.h>

#define IR_PIN 2
IRrecv irrecv(IR_PIN);
decode_results results;

// Define your remote codes
#define REMOTE_FORWARD 0xFF18E7
#define REMOTE_BACKWARD 0xFF4AB5
#define REMOTE_LEFT 0xFF10EF
#define REMOTE_RIGHT 0xFF5AA5
#define REMOTE_STOP 0xFF38C7
#define REMOTE_PUMP 0xFF6897
#define REMOTE_AUTO 0xFF9867

void setup() {
  irrecv.enableIRIn();
  // ... rest of setup
}

void loop() {
  if (irrecv.decode(&results)) {
    handleIRCommand(results.value);
    irrecv.resume();
  }
  // ... rest of code
}

void handleIRCommand(unsigned long command) {
  switch (command) {
    case REMOTE_FORWARD:
      moveForward();
      break;
    case REMOTE_BACKWARD:
      moveBackward();
      break;
    case REMOTE_LEFT:
      turnLeft();
      break;
    case REMOTE_RIGHT:
      turnRight();
      break;
    case REMOTE_STOP:
      stopMotors();
      break;
    case REMOTE_PUMP:
      digitalWrite(WATER_PUMP, !digitalRead(WATER_PUMP));
      break;
    case REMOTE_AUTO:
      currentState = SEARCHING;
      break;
  }
}
```

## 🎯 Enhanced Fire Detection

### Example 3: Multi-Level Sensitivity

Adjust detection based on fire intensity.

```cpp
#define CLOSE_FIRE_THRESHOLD 300    // Very close, urgent
#define MEDIUM_FIRE_THRESHOLD 512   // Medium distance
#define FAR_FIRE_THRESHOLD 700      // Far away

void detectFireLevel() {
  int flameReading = analogRead(FLAME_SENSOR_CENTER);
  
  if (flameReading < CLOSE_FIRE_THRESHOLD) {
    // URGENT: Very close fire
    Serial.println("ALERT: Close fire detected!");
    stopMotors();
    digitalWrite(WATER_PUMP, HIGH);
    tone(BUZZER_PIN, 2000);  // High pitch alarm
  }
  else if (flameReading < MEDIUM_FIRE_THRESHOLD) {
    // Moderate fire, approach carefully
    Serial.println("Medium fire detected");
    analogWrite(MOTOR_ENABLE_LEFT, 150);   // Slower approach
    analogWrite(MOTOR_ENABLE_RIGHT, 150);
  }
  else if (flameReading < FAR_FIRE_THRESHOLD) {
    // Distant fire, normal approach
    Serial.println("Far fire detected");
    analogWrite(MOTOR_ENABLE_LEFT, 200);   // Normal speed
    analogWrite(MOTOR_ENABLE_RIGHT, 200);
  }
  else {
    // No fire detected
    noTone(BUZZER_PIN);
  }
}
```

### Example 4: Fire Location Triangulation

More accurate fire direction using all three sensors.

```cpp
struct FireLocation {
  bool detected;
  int angle;        // -45 to +45 degrees (0 = center)
  int intensity;    // 0-1023
};

FireLocation detectFireLocation() {
  FireLocation fire;
  
  int left = analogRead(FLAME_SENSOR_LEFT);
  int center = analogRead(FLAME_SENSOR_CENTER);
  int right = analogRead(FLAME_SENSOR_RIGHT);
  
  // Check if any sensor detects fire
  if (left < FLAME_THRESHOLD || center < FLAME_THRESHOLD || right < FLAME_THRESHOLD) {
    fire.detected = true;
    
    // Calculate weighted angle
    // Lower value = stronger signal = invert for calculation
    int leftStrength = 1023 - left;
    int centerStrength = 1023 - center;
    int rightStrength = 1023 - right;
    
    // Calculate angle (-45 to +45)
    int totalStrength = leftStrength + centerStrength + rightStrength;
    fire.angle = ((rightStrength - leftStrength) * 45) / totalStrength;
    
    // Overall intensity
    fire.intensity = max(leftStrength, max(centerStrength, rightStrength));
    
    Serial.print("Fire at angle: ");
    Serial.print(fire.angle);
    Serial.print(" degrees, intensity: ");
    Serial.println(fire.intensity);
  }
  else {
    fire.detected = false;
  }
  
  return fire;
}

// Use in main loop
void loop() {
  FireLocation fire = detectFireLocation();
  
  if (fire.detected) {
    if (fire.angle > 10) {
      turnRight();
    }
    else if (fire.angle < -10) {
      turnLeft();
    }
    else {
      moveForward();
    }
  }
}
```

## 🔄 Advanced Navigation

### Example 5: Smooth Turning with Proportional Control

```cpp
void smoothTurnToAngle(int angle) {
  // angle: -45 to +45 (negative = left, positive = right)
  
  int turnSpeed = map(abs(angle), 0, 45, 50, 255);
  turnSpeed = constrain(turnSpeed, 50, 255);
  
  if (angle > 0) {
    // Turn right - right motor slower or reverse
    analogWrite(MOTOR_ENABLE_LEFT, 200);
    analogWrite(MOTOR_ENABLE_RIGHT, 200 - turnSpeed);
    digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
    digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    
    if (angle > 20) {
      // Sharp turn - reverse right motor
      digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
      digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
    } else {
      // Gentle turn - slow right motor
      digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
      digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
    }
  }
  else if (angle < 0) {
    // Turn left - left motor slower or reverse
    analogWrite(MOTOR_ENABLE_LEFT, 200 - turnSpeed);
    analogWrite(MOTOR_ENABLE_RIGHT, 200);
    digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
    digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
    
    if (angle < -20) {
      // Sharp turn - reverse left motor
      digitalWrite(MOTOR_LEFT_FORWARD, LOW);
      digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
    } else {
      // Gentle turn - slow left motor
      digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
      digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    }
  }
  else {
    // Go straight
    moveForward();
  }
}
```

### Example 6: Obstacle Avoidance Enhancement

```cpp
#define OBSTACLE_THRESHOLD 15  // cm

bool checkForObstacle() {
  int distance = getDistance();
  
  if (distance > 0 && distance < OBSTACLE_THRESHOLD) {
    Serial.print("Obstacle detected at ");
    Serial.print(distance);
    Serial.println(" cm");
    return true;
  }
  return false;
}

void navigateAroundObstacle() {
  stopMotors();
  delay(500);
  
  // Back up
  moveBackward();
  delay(1000);
  stopMotors();
  delay(500);
  
  // Try turning right first
  turnRight();
  delay(800);
  stopMotors();
  delay(500);
  
  // Check if path is clear
  if (getDistance() > OBSTACLE_THRESHOLD) {
    moveForward();
  }
  else {
    // Turn left instead (180 degree turn)
    turnLeft();
    delay(1600);
    stopMotors();
  }
}

// In APPROACHING_FIRE state:
if (checkForObstacle()) {
  navigateAroundObstacle();
}
```

## 💧 Pump Control Enhancements

### Example 7: Pulsed Water Spray

More efficient water usage.

```cpp
void pulsedSpray(int duration, int pulseLength, int pauseLength) {
  unsigned long startTime = millis();
  
  while (millis() - startTime < duration) {
    digitalWrite(WATER_PUMP, HIGH);
    delay(pulseLength);
    digitalWrite(WATER_PUMP, LOW);
    delay(pauseLength);
  }
}

// In EXTINGUISHING state:
pulsedSpray(3000, 500, 200);  // 3 seconds total, 500ms on, 200ms off
```

### Example 8: Directional Water Spray with Servo

**Additional Hardware:**
- SG90 Servo motor
- Connect to Arduino D8

```cpp
#include <Servo.h>

#define SERVO_PIN 8
Servo sprayServo;

void setup() {
  sprayServo.attach(SERVO_PIN);
  sprayServo.write(90);  // Center position
  // ... rest of setup
}

void sprayInDirection(int angle) {
  // angle: 0-180 (90 = center)
  sprayServo.write(angle);
  delay(500);  // Wait for servo to position
  digitalWrite(WATER_PUMP, HIGH);
  delay(2000);
  digitalWrite(WATER_PUMP, LOW);
}

void sweepSpray() {
  // Sweep from left to right while spraying
  digitalWrite(WATER_PUMP, HIGH);
  
  for (int angle = 45; angle <= 135; angle += 5) {
    sprayServo.write(angle);
    delay(100);
  }
  
  digitalWrite(WATER_PUMP, LOW);
  sprayServo.write(90);  // Return to center
}

// In EXTINGUISHING state:
void extinguishFire() {
  FireLocation fire = detectFireLocation();
  
  // Point servo at fire
  int servoAngle = map(fire.angle, -45, 45, 45, 135);
  sprayInDirection(servoAngle);
  
  // Or do a sweep
  // sweepSpray();
}
```

## 📊 Monitoring and Display

### Example 9: LCD Status Display

**Additional Hardware:**
- 16x2 I2C LCD
- Connect: SDA → A4, SCL → A5, VCC → 5V, GND → GND

**Library:** LiquidCrystal_I2C

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address may be 0x3F

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Fire Robot");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  // ... rest of setup
}

void updateDisplay() {
  lcd.clear();
  
  // Line 1: State
  lcd.setCursor(0, 0);
  switch(currentState) {
    case SEARCHING:
      lcd.print("State: SEARCH");
      break;
    case APPROACHING_FIRE:
      lcd.print("State: APPROACH");
      break;
    case EXTINGUISHING:
      lcd.print("State: SPRAY");
      break;
  }
  
  // Line 2: Fire status or distance
  lcd.setCursor(0, 1);
  if (fireDetected) {
    lcd.print("Fire: ");
    int distance = getDistance();
    lcd.print(distance);
    lcd.print("cm");
  }
  else {
    lcd.print("No Fire");
  }
}

// Call in loop every second
static unsigned long lastDisplayUpdate = 0;
if (millis() - lastDisplayUpdate > 1000) {
  updateDisplay();
  lastDisplayUpdate = millis();
}
```

### Example 10: LED Status Indicators

```cpp
#define LED_POWER 4      // Green - System on
#define LED_FIRE 2       // Red - Fire detected
#define LED_PUMP A4      // Blue - Pump active
#define LED_STATUS A5    // Yellow - General status

void setup() {
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_FIRE, OUTPUT);
  pinMode(LED_PUMP, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  
  digitalWrite(LED_POWER, HIGH);  // Power always on
  // ... rest of setup
}

void updateLEDs() {
  // Fire detection LED
  digitalWrite(LED_FIRE, fireDetected ? HIGH : LOW);
  
  // Pump LED
  digitalWrite(LED_PUMP, digitalRead(WATER_PUMP));
  
  // Status LED - blink pattern based on state
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  int blinkRate;
  switch(currentState) {
    case SEARCHING:
      blinkRate = 1000;  // Slow blink
      break;
    case APPROACHING_FIRE:
      blinkRate = 250;   // Fast blink
      break;
    case EXTINGUISHING:
      blinkRate = 100;   // Very fast blink
      break;
    default:
      blinkRate = 500;
  }
  
  if (millis() - lastBlink > blinkRate) {
    ledState = !ledState;
    digitalWrite(LED_STATUS, ledState);
    lastBlink = millis();
  }
}

// Call in main loop
updateLEDs();
```

## 🔊 Sound and Alerts

### Example 11: Buzzer Alerts

```cpp
#define BUZZER_PIN A3

void playTone(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}

void alertFireDetected() {
  // Rising tone
  for (int freq = 500; freq < 2000; freq += 100) {
    tone(BUZZER_PIN, freq, 50);
    delay(50);
  }
  noTone(BUZZER_PIN);
}

void alertFireExtinguished() {
  // Descending tone
  for (int freq = 2000; freq > 500; freq -= 100) {
    tone(BUZZER_PIN, freq, 50);
    delay(50);
  }
  noTone(BUZZER_PIN);
}

void alertLowBattery() {
  // Intermittent beep
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 1000, 200);
    delay(400);
  }
}

// Use in state transitions
case APPROACHING_FIRE:
  if (previousState != APPROACHING_FIRE) {
    alertFireDetected();
  }
  // ... rest of code
```

## 📈 Data Logging

### Example 12: SD Card Logging

**Additional Hardware:**
- SD card module
- Connect: CS → D4, MOSI → D11, MISO → D12, SCK → D13

**Library:** SD

```cpp
#include <SD.h>
#include <SPI.h>

#define SD_CS_PIN 4
File logFile;

void setup() {
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
  }
  else {
    Serial.println("SD card initialized");
  }
  // ... rest of setup
}

void logData(String message) {
  logFile = SD.open("firelog.txt", FILE_WRITE);
  
  if (logFile) {
    logFile.print(millis());
    logFile.print(",");
    logFile.println(message);
    logFile.close();
  }
}

// Usage
logData("Fire detected at left sensor");
logData("Extinguishing started");
logData("Fire extinguished successfully");
```

## 🔋 Battery Management

### Example 13: Battery Voltage Monitoring

**Wiring:**
- Voltage divider: Battery → 10KΩ → A3 → 10KΩ → GND

```cpp
#define BATTERY_PIN A3
#define LOW_VOLTAGE 650     // Adjust based on your divider
#define CRITICAL_VOLTAGE 600

float readBatteryVoltage() {
  int reading = analogRead(BATTERY_PIN);
  // Adjust multiplier based on your voltage divider
  float voltage = reading * (5.0 / 1023.0) * 2.0;  // Assuming 1:1 divider
  return voltage;
}

void checkBattery() {
  int reading = analogRead(BATTERY_PIN);
  
  if (reading < CRITICAL_VOLTAGE) {
    // Critical - stop everything
    Serial.println("CRITICAL: Battery voltage too low!");
    stopMotors();
    digitalWrite(WATER_PUMP, LOW);
    currentState = STOPPED;
    
    // Flash LED or beep
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_STATUS, HIGH);
      tone(BUZZER_PIN, 2000, 100);
      delay(200);
      digitalWrite(LED_STATUS, LOW);
      delay(200);
    }
  }
  else if (reading < LOW_VOLTAGE) {
    Serial.println("WARNING: Low battery");
    // Reduce motor speed to conserve power
    analogWrite(MOTOR_ENABLE_LEFT, 150);
    analogWrite(MOTOR_ENABLE_RIGHT, 150);
  }
}

// Check every 5 seconds
static unsigned long lastBatteryCheck = 0;
if (millis() - lastBatteryCheck > 5000) {
  checkBattery();
  lastBatteryCheck = millis();
}
```

## 🌡️ Temperature Sensing

### Example 14: DHT11 Temperature Sensor

```cpp
#include <DHT.h>

#define DHT_PIN A4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  dht.begin();
  // ... rest of setup
}

void checkTemperature() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (!isnan(temp)) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    
    if (temp > 40) {
      Serial.println("HIGH TEMPERATURE WARNING!");
      // Could trigger emergency response
    }
  }
}
```

## 🎮 Complete Enhanced Example

### Example 15: Full-Featured Robot Code

```cpp
// This combines multiple enhancements
// Requires: Bluetooth, LCD, Buzzer, LEDs

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ... all pin definitions ...

LiquidCrystal_I2C lcd(0x27, 16, 2);
bool manualMode = false;
int fireCount = 0;

void setup() {
  // ... standard setup ...
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Handle Bluetooth commands
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'M') manualMode = !manualMode;
    if (manualMode) {
      handleManualControl(cmd);
      return;
    }
  }
  
  // Battery check
  checkBattery();
  
  // Main logic with enhancements
  FireLocation fire = detectFireLocation();
  
  if (fire.detected && currentState == SEARCHING) {
    alertFireDetected();
    currentState = APPROACHING_FIRE;
    fireCount++;
  }
  
  // ... rest of logic ...
  
  // Update displays
  updateLCD();
  updateLEDs();
}
```

---

## 📚 Additional Resources

### Useful Libraries
- IRremote: IR remote control
- Servo: Servo motor control
- LiquidCrystal_I2C: LCD display
- SD: SD card logging
- DHT: Temperature/humidity sensors

### Installation
```
Arduino IDE → Sketch → Include Library → Manage Libraries
Search for library name and click Install
```

### Testing Tips
1. Test each enhancement separately
2. Use Serial Monitor for debugging
3. Document your modifications
4. Keep backup of working code
5. Add comments for future reference

---

**Happy Coding! Feel free to mix and match these examples to create your perfect fire-fighting robot!**
