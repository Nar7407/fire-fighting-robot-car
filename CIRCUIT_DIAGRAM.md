# Circuit Diagram - Fire-Fighting Robot Car

## Overview
This document provides detailed wiring instructions for the fire-fighting robot car.

## Complete Connection Diagram

```
                                    FIRE-FIGHTING ROBOT CAR
                                    =======================

Battery (7.4V - 11.1V)
    |
    +-----> L298N Motor Driver (12V Input)
    |           |
    |           +-----> Arduino Vin (if using battery to power Arduino)
    |           |
    |           +-----> OUT1 -----> Left Motor (-)
    |           |       OUT2 -----> Left Motor (+)
    |           |       OUT3 -----> Right Motor (-)
    |           |       OUT4 -----> Right Motor (+)
    |           |
    |           +-----> GND -----> Arduino GND
    |
    +-----> Common GND


Arduino Uno
-----------
Digital Pins:
    D3  (PWM) -----> L298N ENB (Right Motor Enable)
    D5  (PWM) -----> L298N IN1 (Left Motor Forward)
    D6  (PWM) -----> L298N IN2 (Left Motor Backward)
    D7        -----> Relay Module IN (Water Pump Control)
    D9  (PWM) -----> L298N IN3 (Right Motor Forward)
    D10 (PWM) -----> L298N IN4 (Right Motor Backward)
    D11 (PWM) -----> L298N ENA (Left Motor Enable)
    D12       -----> HC-SR04 TRIG
    D13       -----> HC-SR04 ECHO

Analog Pins:
    A0        -----> Flame Sensor Left (OUT)
    A1        -----> Flame Sensor Center (OUT)
    A2        -----> Flame Sensor Right (OUT)

Power:
    5V        -----> Flame Sensors VCC (x3)
              -----> HC-SR04 VCC
              -----> Relay Module VCC
    
    GND       -----> Flame Sensors GND (x3)
              -----> HC-SR04 GND
              -----> Relay Module GND
              -----> L298N GND
              -----> Battery GND (common ground)


L298N Motor Driver
------------------
Input:
    12V       <----- Battery Positive
    GND       <----- Battery Negative (Common GND)
    
Motor Control:
    IN1       <----- Arduino D5
    IN2       <----- Arduino D6
    IN3       <----- Arduino D9
    IN4       <----- Arduino D10
    ENA       <----- Arduino D11 (with jumper removed)
    ENB       <----- Arduino D3 (with jumper removed)

Output:
    OUT1      -----> Left Motor Terminal (-)
    OUT2      -----> Left Motor Terminal (+)
    OUT3      -----> Right Motor Terminal (-)
    OUT4      -----> Right Motor Terminal (+)


Flame Sensors (x3)
------------------
Each sensor:
    VCC       <----- Arduino 5V
    GND       <----- Arduino GND
    OUT       <----- Arduino A0/A1/A2

Position: Left (A0), Center (A1), Right (A2)


HC-SR04 Ultrasonic Sensor
--------------------------
    VCC       <----- Arduino 5V
    TRIG      <----- Arduino D12
    ECHO      <----- Arduino D13
    GND       <----- Arduino GND


Relay Module (for Water Pump)
------------------------------
Control:
    VCC       <----- Arduino 5V
    IN        <----- Arduino D7
    GND       <----- Arduino GND

Switch:
    COM       <----- Pump Power Supply (+)
    NO        -----> Water Pump (+)
    NC        -----> Not connected

Water Pump:
    (+)       <----- Relay NO
    (-)       <----- Power Supply GND (Common GND)
```

## Power Distribution

```
Battery (7.4V or 11.1V Li-ion)
    |
    +---> L298N 12V Input (for motors)
    |       |
    |       +---> 5V Output (if available) can power Arduino
    |
    +---> Arduino Vin (7-12V input)
            |
            +---> Arduino 5V Output (for sensors, relay)
            |
            +---> GND (common ground for all components)
```

## Flame Sensor Details

```
Flame Sensor (KY-026 or similar)
    
    [VCC] [GND] [DO] [AO]
      |     |    |    |
      5V   GND  N/C  Arduino Analog Pin

Configuration:
- Use AO (Analog Output) pin for better sensitivity
- Adjust onboard potentiometer for detection distance
- Test range: ~0.5m to 2m depending on fire intensity
- Lower analog value = fire detected
```

## Motor Connection Details

```
DC Motors (2x)
    
    Left Motor:             Right Motor:
    +---> OUT2 (L298N)      +---> OUT4 (L298N)
    +---> OUT1 (L298N)      +---> OUT3 (L298N)

Note: If motor rotates in wrong direction, swap the two wires
```

## Relay Module Details

```
Relay Module (5V Single Channel)

    Arduino D7 -----> [IN]
    Arduino 5V -----> [VCC]
    Arduino GND ----> [GND]
    
    Pump Power + ---> [COM]
    Pump + ----------> [NO]
    [NC] ------------- Not Connected
    
When D7 is HIGH:
    Relay activates → COM connects to NO → Pump turns ON
    
When D7 is LOW:
    Relay deactivates → COM disconnects from NO → Pump turns OFF
```

## Assembly Steps

### Step 1: Mount Components on Chassis
1. Position Arduino at center or rear
2. Mount L298N motor driver near motors
3. Install flame sensors at front (spread across width)
4. Place ultrasonic sensor at front center
5. Secure water tank on top or side
6. Mount relay module near pump

### Step 2: Motor Connections
1. Connect left motor to L298N OUT1 and OUT2
2. Connect right motor to L298N OUT3 and OUT4
3. Ensure motors are securely mounted with wheels attached
4. Test rotation direction (swap wires if needed)

### Step 3: Power Connections
1. Connect battery to L298N 12V input
2. Connect L298N GND to common ground
3. Connect Arduino Vin to battery (through switch recommended)
4. Verify all GND connections are common

### Step 4: Sensor Connections
1. Wire three flame sensors to A0, A1, A2
2. Connect ultrasonic sensor to D12 (TRIG) and D13 (ECHO)
3. Ensure all sensors receive 5V and share common GND

### Step 5: Motor Driver Connections
1. Connect control signals D5, D6, D9, D10 to IN1-IN4
2. Remove ENA and ENB jumpers on L298N
3. Connect D11 to ENA and D3 to ENB for speed control

### Step 6: Pump and Relay
1. Connect relay control (IN) to Arduino D7
2. Wire pump through relay (COM → Pump → GND)
3. Test relay activation with simple code

### Step 7: Final Checks
1. Verify all connections with multimeter
2. Check for shorts before powering on
3. Ensure proper polarity on all components
4. Test each subsystem individually

## Cable Management Tips

1. Use zip ties to bundle wires neatly
2. Keep power wires separate from signal wires
3. Label wires for easy troubleshooting
4. Use heat shrink tubing for exposed connections
5. Secure loose wires to chassis to prevent entanglement

## Testing Checklist

- [ ] Battery voltage correct (7.4V - 11.1V)
- [ ] All GND connections common
- [ ] Arduino powers on (LED lights up)
- [ ] L298N power LED on
- [ ] Motors rotate in correct directions
- [ ] Flame sensors detect fire (test with candle)
- [ ] Ultrasonic sensor reads distance
- [ ] Relay clicks when activated
- [ ] Water pump runs when relay activated
- [ ] No loose connections or shorts

## Common Issues and Solutions

| Issue | Check | Solution |
|-------|-------|----------|
| Motors don't run | Power supply | Verify battery voltage and connections |
| One motor doesn't work | Motor wiring | Check OUT terminals and motor connections |
| Motors run but no speed control | Enable pins | Connect D3 and D11, remove L298N jumpers |
| Flame sensor always detects fire | Sensitivity | Adjust potentiometer on sensor |
| Pump doesn't activate | Relay | Check D7 connection and relay wiring |
| Arduino resets randomly | Power | Ensure adequate current supply |

## Safety Notes

⚠️ **Important:**
1. Never connect motor power directly to Arduino pins
2. Always use common ground for all components
3. Use appropriate wire gauge for motor current
4. Add fuse to battery connection for safety
5. Insulate all exposed connections
6. Double-check polarity before powering on

## Optional Enhancements

### Add Power Switch
- Install switch between battery and circuit for easy on/off

### Voltage Monitoring
- Use voltage divider on A3 to monitor battery level
- Add low voltage warning in code

### LED Indicators
- Add LEDs to show: power, fire detected, pump active

### Emergency Stop Button
- Connect button to Arduino interrupt pin for immediate stop

---

**Note:** Always test each connection with a multimeter before powering the complete system to prevent component damage.
