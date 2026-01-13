# Fire-Fighting Robot Car 🚒🤖

A **fire-fighting robotic car**: a mobile platform with a **water tank**, **pump**, and **pipe/nozzle** that **detects fire and automatically sprays water**. This is a practical, low-cost build plan, aligned with student and lab projects.

---

## 📋 Table of Contents

1. [Project Overview](#project-overview)
2. [Functionality](#functionality)
3. [Estimated Cost (INR)](#estimated-cost-inr)
4. [How the System Works](#how-the-system-works)
5. [Block Diagram](#block-diagram)
6. [Step-by-Step Build Guide](#step-by-step-build-guide)
7. [Arduino Code](#arduino-code)
8. [Possible Enhancements](#possible-enhancements)
9. [Project Level Assessment](#project-level-assessment)
10. [Additional Resources](#additional-resources)

---

## 🎯 Project Overview

This project is a **fire-fighting robot car** designed to:

* Move on wheels (remote or autonomous)
* Use a **flame sensor** to detect fire
* Activate a **water pump** when fire is detected
* Spray water through a pipe/nozzle toward the flame
* Can be extended with Bluetooth or obstacle avoidance

---

## ⚡ Functionality

| Feature | Description |
|---------|-------------|
| **Movement** | Moves on 4 wheels using DC gear motors |
| **Fire Detection** | Uses infrared flame sensor to detect fire |
| **Water Spraying** | Activates water pump to extinguish fire |
| **Control Options** | Remote control or autonomous operation |
| **Extensibility** | Can add Bluetooth, obstacle avoidance, IoT features |

---

## 💰 Estimated Cost (INR)

| Component                       | Quantity | Approx Cost (₹) |
| ------------------------------- | -------- | --------------- |
| Arduino UNO (or Nano)           | 1        | 250 – 350       |
| L298N Motor Driver              | 1        | 150             |
| DC Gear Motors + Wheels         | 4        | 350 – 500       |
| Flame Sensor Module             | 3        | 150 – 300       |
| HC-SR04 Ultrasonic Sensor       | 1        | 50 – 100        |
| SG90 Micro Servo Motor          | 1        | 100 – 150       |
| Mini DC Water Pump (5–6V)       | 1        | 150 – 250       |
| Relay Module (or MOSFET)        | 1        | 70 – 100        |
| Plastic Water Tank (200–300 ml) | 1        | 50              |
| Pipe + Nozzle                   | 1        | 30              |
| Chassis (acrylic/metal)         | 1        | 200 – 300       |
| Battery (18650 or 9V pack)      | 1        | 150 – 250       |
| Jumper Wires, switch, screws    | —        | 100             |

### **Total Estimated Cost: ₹1,900 – ₹2,550**

This fits well within typical **PBL / mini-project budgets**.

---

## 🔧 How the System Works

### System Logic

1. **Flame sensor continuously scans** for fire (IR wavelength)
2. When flame is detected:
   * Arduino **stops the motors**
   * Activates **relay → water pump ON**
3. Pump pulls water from tank and **sprays through pipe**
4. After fire disappears:
   * Pump turns OFF
   * Robot resumes movement (optional)

---

## 📊 Block Diagram

```
┌─────────────────┐
│   Flame Sensor  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   Arduino UNO   │
└────────┬────────┘
         │
    ┌────┴────┐
    │         │
    ▼         ▼
┌───────┐  ┌───────┐
│ Motor │  │ Relay │
│ Driver│  └───┬───┘
└───┬───┘      │
    │          ▼
    ▼      ┌───────┐
┌───────┐  │ Pump  │
│Motors │  └───┬───┘
└───────┘      │
               ▼
          ┌──────────┐
          │Water     │
          │Spray     │
          └──────────┘
```

---

## 🛠️ Step-by-Step Build Guide

### Step 1: Assemble the Car

* Fix motors to chassis
* Attach wheels
* Mount water tank securely
* Place nozzle facing forward

### Step 2: Wiring

| Component | Arduino Connection |
|-----------|-------------------|
| Flame sensor (3x) | Analog pins (A0, A1, A2) |
| HC-SR04 Ultrasonic | D12 (TRIG), D13 (ECHO) |
| SG90 Servo Motor | D4 (PWM) |
| L298N Motor Driver | Motor control pins (D3, D5, D6, D9, D10, D11) |
| Relay Module | Digital pin (D7) |
| Pump | Relay → Battery |

> ⚠️ **Important:** Pump must NOT be powered directly from Arduino.

> 💡 **Note:** This project uses an **active LOW** relay module. If you have an active HIGH relay, swap `HIGH` and `LOW` in the relay control code.

### Step 3: Connection Diagram

```
Arduino UNO Connections:
├── A0 ← Flame Sensor Left (Signal)
├── A1 ← Flame Sensor Center (Signal)
├── A2 ← Flame Sensor Right (Signal)
├── D3 → L298N ENB (Right Motor Enable - PWM)
├── D4 → SG90 Servo (Orange/Signal wire)
├── D5 → L298N IN1 (Left Motor Forward)
├── D6 → L298N IN2 (Left Motor Backward)
├── D7 → Relay IN (Water Pump Control)
├── D9 → L298N IN3 (Right Motor Forward)
├── D10 → L298N IN4 (Right Motor Backward)
├── D11 → L298N ENA (Left Motor Enable - PWM)
├── D12 → HC-SR04 TRIG
├── D13 → HC-SR04 ECHO
├── 5V → Sensors VCC, Relay VCC, Servo Red wire
├── GND → Common Ground, Servo Brown wire
└── VIN ← Battery (7-12V)

L298N Motor Driver:
├── OUT1, OUT2 → Left Motors
├── OUT3, OUT4 → Right Motors
├── 12V → Battery Positive
└── GND → Battery Negative

Relay Module:
├── IN ← Arduino D7
├── VCC ← Arduino 5V
├── GND ← Arduino GND
├── COM → Battery Positive
├── NO → Water Pump Positive
└── Pump Negative → Battery Negative
```

---

## 💻 Arduino Code

### Basic Fire Detection and Pump Control

> 💡 **Flame Sensor Calibration:** The threshold value (400) may need adjustment based on your specific sensor. Test your sensor by monitoring Serial output - values **decrease** when flame is detected. Adjust the threshold based on your readings (typically 100-500 range).

```cpp
// Fire Fighting Robot Car - Basic Code
// Detects flame and activates water pump
// NOTE: Uses active LOW relay module. For active HIGH relay, swap HIGH/LOW values.

int flamePin = A0;      // Flame sensor analog pin
int relayPin = 7;       // Relay control pin for water pump
int flameThreshold = 400; // Calibrate this value for your sensor

// Motor pins (L298N)
int motor1Pin1 = 2;
int motor1Pin2 = 3;
int motor2Pin1 = 4;
int motor2Pin2 = 5;
int enableA = 6;
int enableB = 9;

void setup() {
  Serial.begin(9600);
  
  // Relay pin setup
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Pump OFF (active LOW relay)
  
  // Motor pins setup
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  
  Serial.println("Fire Fighting Robot Initialized!");
}

void loop() {
  int flameValue = analogRead(flamePin);
  Serial.print("Flame Sensor Value: ");
  Serial.println(flameValue);
  
  if (flameValue < flameThreshold) {   // Fire detected (lower value = fire detected)
    Serial.println("🔥 Fire Detected! Activating pump...");
    stopMotors();                    // Stop the robot
    digitalWrite(relayPin, LOW);     // Pump ON
  } else {
    Serial.println("No fire detected. Moving forward...");
    digitalWrite(relayPin, HIGH);    // Pump OFF
    moveForward();                   // Continue moving
  }
  
  delay(100);  // Small delay for stability
}

// Motor control functions
void moveForward() {
  analogWrite(enableA, 200);
  analogWrite(enableB, 200);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void moveBackward() {
  analogWrite(enableA, 200);
  analogWrite(enableB, 200);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void turnLeft() {
  analogWrite(enableA, 150);
  analogWrite(enableB, 200);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void turnRight() {
  analogWrite(enableA, 200);
  analogWrite(enableB, 150);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void stopMotors() {
  analogWrite(enableA, 0);
  analogWrite(enableB, 0);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
```

### Simplified Code (Fire Detection Only)

```cpp
// Simplified Fire Detection Code
// NOTE: Uses active LOW relay. For active HIGH relay, swap HIGH/LOW values.
// Calibrate FLAME_THRESHOLD by monitoring Serial output with your sensor

#define FLAME_THRESHOLD 400  // Adjust based on sensor calibration

int flamePin = A0;
int relayPin = 7;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Pump OFF (active LOW relay)
}

void loop() {
  int flameValue = analogRead(flamePin);

  if (flameValue < FLAME_THRESHOLD) {  // Fire detected
    digitalWrite(relayPin, LOW);        // Pump ON
  } else {
    digitalWrite(relayPin, HIGH);       // Pump OFF
  }
}
```

---

## 🚀 Possible Enhancements

Enhance your project for better evaluation marks:

| Enhancement | Description | Difficulty |
|-------------|-------------|------------|
| **Ultrasonic Sensor** | Obstacle avoidance for autonomous navigation | Easy |
| **Servo Motor** | Rotate nozzle to spray in different directions | Easy |
| **Bluetooth Control** | Mobile app control via HC-05 module | Medium |
| **GSM Alert** | Send SMS when fire is detected | Medium |
| **ESP32 + IoT** | WiFi-enabled dashboard and remote monitoring | Medium |
| **Camera Module** | Visual fire detection using image processing | Advanced |
| **Multi-Sensor Array** | Multiple flame sensors for better coverage | Easy |
| **LCD Display** | Show sensor readings and robot status | Easy |

---

## 📈 Project Level Assessment

| Criteria             | Level                 |
| -------------------- | --------------------- |
| Hardware Integration | Intermediate          |
| Software Complexity  | Beginner–Intermediate |
| Innovation           | High (real-world use) |
| Viva / Presentation  | Excellent             |
| PBL Suitability      | Very High             |

### Why This Project is Great

✅ **Technically sound** - Demonstrates integration of sensors, actuators, and control logic

✅ **Affordable** - Budget-friendly components easily available in India

✅ **Hardware + Software** - Shows both embedded programming and electronics skills

✅ **Real-world application** - Addresses a genuine safety concern

✅ **Extensible** - Easy to add more features for advanced projects

---

## 📚 Additional Resources

For detailed setup instructions, see:

- **[QUICK_START.md](QUICK_START.md)** - Rapid setup guide with comprehensive documentation
- **Circuit Diagrams** - Available in the project documentation
- **Video Tutorials** - Links to assembly and testing videos

### What You Can Get

- ✅ Circuit diagram
- ✅ Full Arduino code (with motors)
- ✅ PPT + Project report templates
- ✅ ESP32 / IoT version code
- ✅ Component purchase links (India)

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

---

## 📄 License

This project is open source and available for educational purposes. See the LICENSE file for more details.

---

## 📞 Contact

For questions, suggestions, or support, please open an issue on GitHub.

---

**Made with ❤️ for students and makers**