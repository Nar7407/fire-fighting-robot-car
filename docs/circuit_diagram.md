# Circuit Diagrams and Wiring Instructions

## Arduino Uno/Nano Wiring

### Power Distribution
```
Battery (7.4V) → L298N Motor Driver (12V input)
                 ├→ Motors
                 └→ 5V output → Arduino VIN
                                └→ Sensors (5V)
```

### Motor Driver (L298N) Connections
```
L298N          Arduino
-----          -------
IN1     →      Pin 5
IN2     →      Pin 6
IN3     →      Pin 9
IN4     →      Pin 10
ENA     →      Pin 3 (PWM)
ENB     →      Pin 11 (PWM)
GND     →      GND

Motor A → Left side motors
Motor B → Right side motors
```

### Flame Sensors
```
Sensor         Arduino
------         -------
Left Sensor:
  VCC   →      5V
  GND   →      GND
  AO    →      A0

Center Sensor:
  VCC   →      5V
  GND   →      GND
  AO    →      A1

Right Sensor:
  VCC   →      5V
  GND   →      GND
  AO    →      A2
```

### Ultrasonic Sensor (HC-SR04)
```
HC-SR04        Arduino
-------        -------
VCC     →      5V
TRIG    →      Pin 12
ECHO    →      Pin 13
GND     →      GND
```

### Water Pump with Relay
```
Relay Module   Arduino
------------   -------
VCC     →      5V
GND     →      GND
IN      →      Pin 7

Relay to Pump:
COM     →      Battery +
NO      →      Pump +
Pump -  →      Battery -
```

### Servo Motor
```
Servo          Arduino
-----          -------
VCC (Red)  →   5V
GND (Brown)→   GND
Signal (Orange)→ Pin 4
```

### Buzzer
```
Buzzer         Arduino
------         -------
+       →      Pin 8
-       →      GND
```

### Bluetooth Module (HC-05)
```
HC-05          Arduino
-----          -------
VCC     →      5V
GND     →      GND
TXD     →      RX (Pin 0)
RXD     →      TX (Pin 1) through voltage divider
               (Use 1K and 2K resistors)
```

**Note**: Arduino TX (5V) to HC-05 RX (3.3V) needs voltage divider:
```
Arduino TX → 1K resistor → HC-05 RX
                         ↓
                    2K resistor → GND
```

## ESP32 Wiring

### Motor Driver (L298N)
```
L298N          ESP32
-----          -----
IN1     →      GPIO 16
IN2     →      GPIO 17
IN3     →      GPIO 18
IN4     →      GPIO 19
ENA     →      GPIO 25
ENB     →      GPIO 26
```

### Flame Sensors
```
Sensor         ESP32
------         -----
Left   (AO)→   GPIO 34 (ADC1_CH6)
Center (AO)→   GPIO 35 (ADC1_CH7)
Right  (AO)→   GPIO 32 (ADC1_CH4)
```

### Ultrasonic Sensor
```
HC-SR04        ESP32
-------        -----
TRIG    →      GPIO 5
ECHO    →      GPIO 18
```

### Other Components
```
Component      ESP32
---------      -----
Pump Relay →   GPIO 27
Servo      →   GPIO 13
Buzzer     →   GPIO 14
```

## Raspberry Pi Wiring

### Motor Driver (L298N)
```
L298N          Raspberry Pi (BCM)
-----          ------------------
IN1     →      GPIO 17
IN2     →      GPIO 27
IN3     →      GPIO 23
IN4     →      GPIO 24
ENA     →      GPIO 12 (PWM0)
ENB     →      GPIO 13 (PWM1)
```

### Flame Sensors (Digital)
```
Sensor         Raspberry Pi
------         ------------
Left   (DO)→   GPIO 5
Center (DO)→   GPIO 6
Right  (DO)→   GPIO 26
```

### Ultrasonic Sensor
```
HC-SR04        Raspberry Pi
-------        ------------
TRIG    →      GPIO 20
ECHO    →      GPIO 21 (through voltage divider)
```

**Important**: HC-SR04 ECHO (5V) to Pi GPIO (3.3V) needs voltage divider!

### Other Components
```
Component      Raspberry Pi
---------      ------------
Pump Relay →   GPIO 16
Buzzer     →   GPIO 19
```

## Complete Circuit Diagram (ASCII Art)

```
                    [Battery 7.4V]
                         |
         +---------------+---------------+
         |                               |
    [L298N Driver]                   [5V Reg]
         |                               |
    +----+----+                     +----+----+
    |         |                     |         |
[Motor L] [Motor R]            [Arduino]  [Sensors]
                                    |
                        +-----------+-----------+
                        |           |           |
                   [Flame Sen] [Ultrasonic] [Servo]
                        |
                    [Relay]
                        |
                   [Water Pump]
```

## PCB Layout Suggestions

For a permanent build, consider:
1. Use a custom PCB or prototyping board
2. Add screw terminals for easy motor connection
3. Include capacitors (100µF) across motor terminals
4. Add LED indicators for power and sensor status
5. Use proper connectors (JST-XH) for sensor connections
6. Include a power switch
7. Add fuse protection (2A) on battery line

## Safety Notes

⚠️ **Important**:
1. Always connect GND first when wiring
2. Double-check polarity before powering on
3. Use appropriate wire gauge (22-24 AWG for signals, 18-20 AWG for motors)
4. Add heatsinks to motor driver if running at high current
5. Never connect motor power directly to microcontroller pins
6. Use flyback diodes across relay coils
7. Keep high-current wires away from sensor wires

## Troubleshooting Wiring Issues

### Motors not running:
- Check motor driver power supply
- Verify ENA/ENB connected to PWM pins
- Test motors directly with battery

### Sensors not working:
- Verify 5V and GND connections
- Check sensor orientation
- Test with multimeter

### Bluetooth not connecting:
- Verify TX/RX crossover
- Check voltage divider for Arduino
- Verify baud rate (9600)

### Servo jittering:
- Add 100µF capacitor across servo power
- Use separate 5V regulator for servo
- Check power supply capacity

---

For questions or issues, please open an issue on GitHub!