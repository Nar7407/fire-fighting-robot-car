# Fire-Fighting Robot Car 🚒🤖

A low-cost autonomous fire-fighting robot built with Arduino. This robot car can detect fire using flame sensors, navigate towards it, and automatically spray water to extinguish the flames.

## 🎯 Features

- **Autonomous Fire Detection**: Uses IR flame sensors to detect fire from multiple angles
- **Smart Navigation**: Moves towards detected fire and avoids obstacles
- **Automatic Water Spray**: Activates water pump when close to fire source
- **Emergency Stop**: Built-in safety mechanisms
- **Search Mode**: Rotates to scan environment when no fire is detected

## 🔧 Components Required

### Electronic Components
1. **Arduino Uno** (or compatible board) - 1 unit
2. **L298N Motor Driver Module** - 1 unit
3. **IR Flame Sensors** - 3 units (left, center, right)
4. **DC Motors** - 2 units (for wheels)
5. **Water Pump** (mini submersible pump 3-6V) - 1 unit
6. **Ultrasonic Sensor HC-SR04** - 1 unit (for obstacle detection)
7. **Relay Module** (5V, single channel) - 1 unit (for pump control)
8. **Wheels** - 2 units (driven) + 1 castor wheel
9. **Li-ion Battery Pack** (7.4V or 11.1V, 2200mAh) - 1 unit
10. **Water Container** (small plastic bottle or tank) - 1 unit
11. **Plastic Tubing/Pipe** - suitable length
12. **Robot Chassis** - 1 unit
13. **Jumper Wires** - as needed
14. **Breadboard** (optional for prototyping) - 1 unit

### Tools Required
- Soldering iron and solder
- Wire stripper
- Screwdriver set
- Hot glue gun
- Multimeter (for testing)

## 📐 Pin Configuration

| Component | Arduino Pin | Description |
|-----------|-------------|-------------|
| Motor Left Forward | D5 | Left motor forward control |
| Motor Left Backward | D6 | Left motor backward control |
| Motor Right Forward | D9 | Right motor forward control |
| Motor Right Backward | D10 | Right motor backward control |
| Motor Enable Left | D11 (PWM) | Left motor speed control |
| Motor Enable Right | D3 (PWM) | Right motor speed control |
| Flame Sensor Left | A0 | Left flame sensor input |
| Flame Sensor Center | A1 | Center flame sensor input |
| Flame Sensor Right | A2 | Right flame sensor input |
| Water Pump | D7 | Pump relay control |
| Ultrasonic Trig | D12 | Ultrasonic sensor trigger |
| Ultrasonic Echo | D13 | Ultrasonic sensor echo |

## 🔌 Wiring Instructions

### Motor Driver (L298N) Connections
1. Connect Arduino D5 → IN1 (Left motor forward)
2. Connect Arduino D6 → IN2 (Left motor backward)
3. Connect Arduino D9 → IN3 (Right motor forward)
4. Connect Arduino D10 → IN4 (Right motor backward)
5. Connect Arduino D11 → ENA (Left motor enable/speed)
6. Connect Arduino D3 → ENB (Right motor enable/speed)
7. Connect left motor to OUT1 and OUT2
8. Connect right motor to OUT3 and OUT4
9. Connect battery positive to 12V input
10. Connect battery negative and Arduino GND to GND

### Flame Sensor Connections
1. **Left Sensor**: VCC → 5V, GND → GND, OUT → A0
2. **Center Sensor**: VCC → 5V, GND → GND, OUT → A1
3. **Right Sensor**: VCC → 5V, GND → GND, OUT → A2

### Water Pump with Relay
1. Connect Arduino D7 → Relay IN
2. Connect Relay VCC → 5V
3. Connect Relay GND → GND
4. Connect pump to relay NO (Normally Open) and COM terminals
5. Connect pump power supply through relay

### Ultrasonic Sensor (HC-SR04)
1. VCC → 5V
2. GND → GND
3. TRIG → D12
4. ECHO → D13

### Power Supply
- Arduino: Powered via USB or VIN (7-12V from battery)
- Motors: Powered from L298N via battery (7.4V or 11.1V)
- Sensors and Pump: 5V from Arduino

## 🚀 Installation & Setup

### 1. Hardware Assembly
1. Mount motors on the robot chassis
2. Attach wheels to motors
3. Install castor wheel at the front or back
4. Mount Arduino and motor driver on chassis
5. Position flame sensors at the front (left, center, right)
6. Secure water tank on chassis
7. Connect pump to tank with tubing
8. Position nozzle/pipe pointing forward
9. Mount ultrasonic sensor at the front

### 2. Software Setup
1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Clone or download this repository
3. Open `fire_fighting_robot.ino` in Arduino IDE
4. Select your Arduino board: Tools → Board → Arduino Uno
5. Select the correct port: Tools → Port → (your Arduino port)
6. Click Upload button to flash the code

### 3. Calibration
1. **Flame Sensors**: Adjust the sensitivity potentiometer on each sensor
   - Test with a candle or lighter
   - Sensor LED should light up when fire is detected
   - Adjust `FLAME_THRESHOLD` in code if needed (default: 512)
2. **Motor Speed**: Adjust `MOTOR_SPEED` (0-255) for optimal speed
3. **Safe Distance**: Modify `SAFE_DISTANCE` based on your robot size
4. **Pump Duration**: Adjust `PUMP_DURATION` based on pump flow rate

## 💻 How It Works

### State Machine Logic
The robot operates in four states:

1. **SEARCHING**: Rotates slowly to scan for fire
   - When fire is detected → switches to APPROACHING_FIRE

2. **APPROACHING_FIRE**: Navigates towards the fire source
   - Fire on left → turns left
   - Fire on right → turns right
   - Fire ahead → moves forward
   - Close enough (< SAFE_DISTANCE) → switches to EXTINGUISHING

3. **EXTINGUISHING**: Stops and activates water pump
   - Runs pump for specified duration
   - Re-checks if fire is extinguished
   - If extinguished → returns to SEARCHING
   - If still burning → continues extinguishing

4. **STOPPED**: Emergency stop state

### Fire Detection
- Three IR flame sensors provide wide detection coverage
- Analog readings below threshold indicate fire presence
- Lower analog value = stronger fire signal

### Navigation
- Motors controlled via L298N driver with PWM speed control
- Ultrasonic sensor prevents collision with obstacles
- Differential steering for precise turning

## 🎮 Usage

1. Fill the water tank
2. Power on the robot
3. Place robot in open area
4. Robot will start in SEARCHING mode
5. When fire is detected, robot automatically approaches and extinguishes it
6. Monitor via Serial Monitor (9600 baud) for debugging

## 🔍 Troubleshooting

| Problem | Solution |
|---------|----------|
| Robot doesn't move | Check motor connections and power supply |
| Fire not detected | Adjust flame sensor sensitivity; check wiring |
| Pump doesn't activate | Verify relay connections; check pump power |
| Robot turns wrong way | Swap motor wires or adjust code logic |
| Sensors too sensitive | Increase FLAME_THRESHOLD value |
| Motors too slow/fast | Adjust MOTOR_SPEED value (0-255) |

## ⚠️ Safety Precautions

1. Always supervise the robot during operation
2. Use only small, controlled fires for testing
3. Keep fire extinguisher nearby during tests
4. Ensure adequate water in tank before operation
5. Test in open area away from flammable materials
6. Check all electrical connections before powering on
7. Use proper battery management and charging

## 🔧 Customization Options

### Code Modifications
- Adjust sensitivity thresholds
- Modify search patterns
- Change pump activation logic
- Add remote control capability
- Implement multiple fire detection

### Hardware Upgrades
- Add camera for visual feedback
- Use servo for directional water spray
- Implement wireless communication (Bluetooth/WiFi)
- Add temperature sensors
- Include buzzer for alerts
- Add LED indicators

## 📊 Technical Specifications

- **Operating Voltage**: 7.4V - 11.1V
- **Motor Speed**: Adjustable via PWM (0-255)
- **Fire Detection Range**: 0.5m - 2m (depends on sensor model)
- **Water Tank Capacity**: 200-500ml (depends on container)
- **Pump Flow Rate**: ~2L/min (typical mini pump)
- **Weight**: ~500g - 1kg (depends on components)
- **Dimensions**: Varies based on chassis (typical: 15cm x 12cm x 10cm)

## 📝 License

This project is open-source and available for educational purposes.

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest features
- Submit pull requests
- Share improvements

## 📧 Contact

For questions or suggestions, please open an issue on GitHub.

## 🌟 Acknowledgments

This project is designed for student and lab projects in India, focusing on practical robotics and automation concepts with affordable components.

---

**Happy Building! 🔥🤖💧**