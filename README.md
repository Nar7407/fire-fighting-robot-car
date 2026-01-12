# Fire Fighting Robot Car

An intelligent autonomous and remotely controlled robot car designed to detect and extinguish fires. Supports multiple platforms including Arduino, ESP32, and Raspberry Pi.

## Features

- 🔥 Autonomous fire detection and navigation
- 🎮 Remote control capability (Bluetooth/WiFi)
- 🚗 Obstacle avoidance using ultrasonic sensors
- 💧 Automatic fire extinguishing mechanism
- 📡 Multi-platform support (Arduino/ESP32/Raspberry Pi)
- 🔋 Battery monitoring
- 📊 Real-time status feedback

## Hardware Requirements

### Core Components
- **Microcontroller**: Arduino Uno/Mega/Nano OR ESP32 OR Raspberry Pi
- **Motor Driver**: L298N or L293D
- **Motors**: 4x DC motors with wheels (or 2x motors for basic version)
- **Flame Sensors**: 3x flame sensor modules (left, center, right)
- **Ultrasonic Sensor**: HC-SR04 (for obstacle detection)
- **Water Pump**: 5V mini water pump with relay module
- **Servo Motor**: SG90 (for sensor scanning)
- **Bluetooth Module**: HC-05 (for Arduino) or built-in (ESP32/Pi)
- **Power Supply**: 7.4V LiPo battery or 6x AA batteries
- **Buzzer**: Active buzzer for alerts
- **LEDs**: Status indicator LEDs

### Optional Components
- IR sensors for line following
- Temperature sensor (DHT11/DHT22)
- Camera module (for Raspberry Pi version)
- LCD display (16x2 or OLED)

## Pin Configuration

### Arduino Uno/Nano
```
Motor Driver (L298N):
- IN1 → Pin 5
- IN2 → Pin 6
- IN3 → Pin 9
- IN4 → Pin 10
- ENA → Pin 3 (PWM)
- ENB → Pin 11 (PWM)

Flame Sensors:
- Left → A0
- Center → A1
- Right → A2

Ultrasonic Sensor (HC-SR04):
- TRIG → Pin 12
- ECHO → Pin 13

Water Pump Relay → Pin 7
Servo Motor → Pin 4
Buzzer → Pin 8
Bluetooth (HC-05):
- TX → Pin 0 (RX)
- RX → Pin 1 (TX)
```

### ESP32
```
Motor Driver:
- IN1 → GPIO 16
- IN2 → GPIO 17
- IN3 → GPIO 18
- IN4 → GPIO 19
- ENA → GPIO 25 (PWM)
- ENB → GPIO 26 (PWM)

Flame Sensors:
- Left → GPIO 34
- Center → GPIO 35
- Right → GPIO 32

Ultrasonic:
- TRIG → GPIO 5
- ECHO → GPIO 18

Water Pump → GPIO 27
Servo → GPIO 13
Buzzer → GPIO 14
```

## Software Setup

### Arduino IDE Setup
1. Install Arduino IDE from [arduino.cc](https://www.arduino.cc)
2. Install required libraries:
   - Servo library (built-in)
   - NewPing library (for ultrasonic)
3. Open `arduino/fire_fighting_robot.ino`
4. Select your board and port
5. Upload the code

### ESP32 Setup
1. Install Arduino IDE
2. Add ESP32 board support:
   - Go to File → Preferences
   - Add to Additional Board Manager URLs: `https://dl.espressif.com/dl/package_esp32_index.json`
3. Install ESP32 board from Board Manager
4. Install libraries:
   - ESP32Servo
   - NewPing
5. Open `esp32/esp32_fire_fighter.ino`
6. Select ESP32 board and upload

### Raspberry Pi Setup
1. Install Raspbian OS
2. Install Python dependencies:
   ```bash
   pip3 install -r requirements.txt
   ```
3. Enable hardware interfaces (I2C, SPI, GPIO)
4. Run the code:
   ```bash
   python3 raspberry_pi/fire_fighter.py
   ```

## Operation Modes

### 1. Autonomous Mode
- Robot continuously scans for fire using flame sensors
- Navigates towards detected fire source
- Avoids obstacles automatically
- Activates water pump when close to fire
- Returns to patrol mode after extinguishing

### 2. Remote Control Mode
- Control via Bluetooth or WiFi
- Manual direction control (Forward, Backward, Left, Right)
- Manual pump activation
- Switch between manual and autonomous mode

## Remote Control Commands

### Bluetooth Commands (Arduino/ESP32)
- `F` - Forward
- `B` - Backward
- `L` - Left
- `R` - Right
- `S` - Stop
- `W` - Activate water pump
- `A` - Toggle autonomous mode
- `M` - Manual mode

## Circuit Diagrams

See `docs/circuit_diagram.md` for detailed wiring instructions and diagrams.

## Configuration

Edit `config.h` (Arduino/ESP32) or `config.py` (Raspberry Pi) to customize:
- Sensor thresholds
- Motor speeds
- Pump duration
- Detection ranges
- Pin assignments

## Troubleshooting

### Robot not moving
- Check motor driver connections
- Verify power supply voltage
- Test motors individually

### Flame not detected
- Adjust flame sensor sensitivity (potentiometer on sensor)
- Check sensor threshold values in config
- Ensure sensors face forward

### Bluetooth not connecting
- Check baud rate (default: 9600)
- Verify HC-05 wiring
- Pair device before connecting

## Safety Precautions

⚠️ **Important Safety Notes**:
- Always test in controlled environment
- Use appropriate fire extinguishing medium (water for paper/wood only)
- Keep battery charged and monitor temperature
- Supervise robot during operation
- Not suitable for large fires or electrical fires

## Contributing

Feel free to fork this project and submit pull requests for improvements!

## License

MIT License - Feel free to use and modify for your projects.

## Author

Nar7407

## Acknowledgments

- Arduino community
- ESP32 development resources
- Robotics enthusiasts worldwide

---

**Happy Building! 🤖🔥**