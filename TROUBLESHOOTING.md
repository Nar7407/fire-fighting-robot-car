# Troubleshooting Guide - Fire-Fighting Robot Car

This comprehensive guide helps you diagnose and fix common issues with your fire-fighting robot car.

## 🔍 Systematic Debugging Approach

### Step 1: Identify the Problem
- What is not working?
- When did it stop working?
- What was the last change made?
- Are there any error messages?

### Step 2: Isolate the Subsystem
- Motor control
- Flame sensors
- Water pump
- Power supply
- Code logic

### Step 3: Test Components Individually
- Use multimeter for electrical testing
- Upload simple test code for each subsystem
- Verify connections step by step

## ⚡ Power Supply Issues

### Problem: Robot Doesn't Power On

**Symptoms:**
- No LED lights on Arduino
- No motor movement
- Complete silence

**Diagnosis:**
```
1. Check battery voltage with multimeter
   - Should be 7.4V or 11.1V (depending on your battery)
   - If below 6V, battery is discharged
   
2. Check power switch (if installed)
   - Is it in ON position?
   - Test continuity with multimeter
   
3. Check fuse (if installed)
   - Has it blown?
   - Replace if necessary
   
4. Check connections
   - Battery to L298N 12V input
   - L298N GND to Arduino GND
   - All connections tight?
```

**Solutions:**
- Charge battery fully
- Replace damaged power switch
- Check and tighten all power connections
- Verify polarity (positive to positive, negative to negative)

### Problem: Arduino Resets Randomly

**Symptoms:**
- Robot stops and restarts
- Serial monitor shows repeated "Initializing..." messages
- Erratic behavior

**Diagnosis:**
```
1. Check voltage under load
   - Measure Arduino Vin when motors running
   - Should stay above 7V
   
2. Check for loose connections
   - Wiggle wires while robot running
   - Note if reset occurs
   
3. Check motor driver heat
   - Is L298N very hot?
   - May be overloading
```

**Solutions:**
- Use higher capacity battery
- Add bulk capacitor (1000µF) across battery terminals
- Ensure common ground between all components
- Reduce motor speed (decrease MOTOR_SPEED value)
- Add heat sink to L298N if missing

### Problem: Low Battery Warning

**Prevention:**
```cpp
// Add to your main code for battery monitoring
// Requires functions from main fire_fighting_robot.ino

#define BATTERY_PIN A3
#define LOW_BATTERY_THRESHOLD 650  // Adjust based on your voltage divider

void checkBattery() {
  int batteryReading = analogRead(BATTERY_PIN);
  if (batteryReading < LOW_BATTERY_THRESHOLD) {
    Serial.println("WARNING: Low battery!");
    // Stop operations for safety (using functions from main code)
    // Call your stopMotors() function
    // Set WATER_PUMP pin LOW
    currentState = STOPPED;
  }
}

// Call this function periodically in your main loop()
```

## 🚗 Motor Control Issues

### Problem: Motors Don't Run

**Symptoms:**
- No wheel movement
- Motors don't make any sound
- L298N status LEDs off

**Diagnosis:**
```
1. Test motors directly
   - Disconnect from L298N
   - Connect directly to 3-6V source
   - Should spin smoothly
   
2. Check L298N connections
   - Measure voltage at 12V input: Should match battery
   - Check if ENA/ENB jumpers removed (for PWM control)
   - Verify IN1-IN4 connections from Arduino
   
3. Check Arduino outputs
   - Upload simple test code
   - Use multimeter to check voltage at D5, D6, D9, D10
   - Should toggle between 0V and 5V
```

**Test Code:**
```cpp
void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);
  
  analogWrite(11, 200);  // Enable left motor
  analogWrite(3, 200);   // Enable right motor
}

void loop() {
  // Forward
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  delay(2000);
  
  // Stop
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  delay(1000);
}
```

**Solutions:**
- Replace faulty motors
- Re-solder loose connections
- Verify L298N is not damaged (check for burning smell)
- Ensure ENA/ENB jumpers are removed for PWM control
- Check that MOTOR_SPEED > 0 in code

### Problem: One Motor Doesn't Work

**Diagnosis:**
```
1. Swap motor connections at L298N
   - If problem follows motor: Motor is faulty
   - If problem stays at same output: Driver channel faulty
   
2. Test the working motor on both channels
   - Connect to OUT1/OUT2, then to OUT3/OUT4
   - Identifies if L298N channel is damaged
```

**Solutions:**
- Replace faulty motor
- Use different L298N channel (swap wiring)
- Replace L298N if channel is damaged

### Problem: Motor Runs in Wrong Direction

**Diagnosis:**
- Motor spins opposite to intended direction

**Solutions:**
- Swap the two motor wires at L298N outputs
  - If connected to OUT1 and OUT2, reverse them
- OR modify code to invert logic for that motor

### Problem: Motors Run Too Slow/Fast

**Diagnosis:**
- Check MOTOR_SPEED value in code
- Measure voltage at motor terminals

**Solutions:**
```cpp
// Adjust this value (0-255)
#define MOTOR_SPEED 200  // Increase for faster, decrease for slower

// For different speeds on each motor (if one motor is stronger)
analogWrite(MOTOR_ENABLE_LEFT, 180);   // Slower
analogWrite(MOTOR_ENABLE_RIGHT, 200);  // Faster
```

### Problem: Motors Stutter or Jerk

**Diagnosis:**
- Insufficient power
- Poor PWM frequency
- Mechanical binding

**Solutions:**
- Increase battery capacity
- Add capacitors (100µF) across each motor
- Check for mechanical obstructions
- Ensure wheels spin freely
- Use higher quality motors

## 🔥 Flame Sensor Issues

### Problem: Sensors Always Detect Fire

**Symptoms:**
- Serial monitor shows LOW readings (< 512) constantly
- Robot always thinks fire is present
- Sensors LEDs always on

**Diagnosis:**
```
1. Check sensor readings via Serial Monitor
   Serial.print("Sensor: ");
   Serial.println(analogRead(A0));
   
2. Cover sensor with hand
   - Reading should INCREASE (800-1000)
   
3. Point at bright light
   - Reading should DECREASE
```

**Solutions:**
- Adjust potentiometer clockwise (reduce sensitivity)
- Move sensors away from bright lights
- Increase FLAME_THRESHOLD in code to 600-700
- Shield sensors from ambient light
- Check for damaged sensors (replace if needed)

### Problem: Sensors Never Detect Fire

**Symptoms:**
- Readings always HIGH (> 900)
- LED on sensor never lights
- Robot never responds to flame

**Diagnosis:**
```
1. Test with lighter/candle at 10cm distance
   - Should show significant drop in reading
   
2. Check connections
   - VCC to 5V: Use multimeter
   - GND to GND: Check continuity
   - OUT to analog pin: Verify correct pin
   
3. Test sensor directly
   - Read voltage at OUT pin with multimeter
   - Should vary with flame presence
```

**Solutions:**
- Adjust potentiometer counter-clockwise (increase sensitivity)
- Decrease FLAME_THRESHOLD in code to 400-500
- Replace faulty sensor
- Check 5V supply with multimeter (should be 4.8-5.2V)
- Verify analog pins are working (test with potentiometer)

### Problem: Inconsistent Detection

**Symptoms:**
- Sometimes detects, sometimes doesn't
- Readings fluctuate wildly
- Unreliable operation

**Diagnosis:**
```
1. Check for loose connections
   - Wiggle wires while monitoring readings
   
2. Monitor readings over time
   - Look for patterns
   - Check if affected by robot movement
```

**Solutions:**
- Secure all connections with solder
- Add small capacitor (10µF) between sensor OUT and GND
- Use shielded cable for sensor wires
- Add averaging in code:
```cpp
int readFlame(int pin) {
  long sum = 0;
  for(int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(10);
  }
  return sum / 10;  // Return average
}
```

### Problem: Wrong Sensor Triggers

**Symptoms:**
- Left sensor triggers but robot turns right
- Center sensor works but sides don't

**Diagnosis:**
- Check which analog pin each sensor connects to
- Verify in code which pin corresponds to which direction

**Solutions:**
- Swap sensor connections at Arduino
- OR modify code to match your wiring:
```cpp
#define FLAME_SENSOR_LEFT A2    // Change these
#define FLAME_SENSOR_CENTER A1  // to match
#define FLAME_SENSOR_RIGHT A0   // your wiring
```

## 💧 Water Pump Issues

### Problem: Pump Doesn't Run

**Symptoms:**
- No water flow
- No pump sound
- Relay doesn't click

**Diagnosis:**
```
1. Test pump directly
   - Disconnect from relay
   - Connect directly to 3-6V battery
   - Should pump water
   
2. Test relay
   - Listen for click when activated
   - Measure voltage across relay output
   - Should switch when D7 goes HIGH
   
3. Check code logic
   - Add Serial.println("Pump ON") to verify code execution
   - Ensure pump activation code is reached
```

**Test Code:**
```cpp
void setup() {
  pinMode(7, OUTPUT);
}

void loop() {
  digitalWrite(7, HIGH);
  delay(2000);
  digitalWrite(7, LOW);
  delay(2000);
}
```

**Solutions:**
- Replace faulty pump
- Replace faulty relay
- Check relay wiring (VCC, GND, IN, COM, NO)
- Verify pump power supply adequate (3-6V, sufficient current)
- Check D7 connection from Arduino to relay

### Problem: Pump Runs Continuously

**Diagnosis:**
- Check if relay is stuck
- Verify code logic
- Check for short in relay wiring

**Solutions:**
- Replace relay
- Add explicit digitalWrite(WATER_PUMP, LOW) in setup()
- Check relay wiring polarity

### Problem: Weak Water Flow

**Diagnosis:**
- Low battery voltage
- Pump partially clogged
- Tubing kinked or blocked

**Solutions:**
- Charge battery fully
- Clean pump filter/impeller
- Check tubing for kinks or blockages
- Use larger diameter tubing
- Use more powerful pump

## 🤖 Navigation Issues

### Problem: Robot Moves But Doesn't Approach Fire

**Symptoms:**
- Sensors detect fire
- Motors run
- But robot doesn't move toward fire

**Diagnosis:**
```
1. Check Serial Monitor output
   - Verify fire detection: "Fire: YES"
   - Check which sensor detects: L/C/R
   
2. Observe motor behavior
   - Do motors respond to sensor inputs?
   - Are both motors running?
   
3. Verify state machine
   - Is robot stuck in SEARCHING state?
   - Add debug prints to each state
```

**Solutions:**
- Check motor directions (may need to swap wires)
- Verify turn logic matches your motor wiring
- Add debug prints:
```cpp
Serial.print("State: ");
switch(currentState) {
  case SEARCHING: Serial.println("SEARCHING"); break;
  case APPROACHING_FIRE: Serial.println("APPROACHING"); break;
  case EXTINGUISHING: Serial.println("EXTINGUISHING"); break;
}
```

### Problem: Robot Turns Wrong Way

**Diagnosis:**
- Left/right motors may be swapped
- Turn logic may be inverted

**Solutions:**
Option 1: Swap motor wires at L298N
Option 2: Modify turn functions:
```cpp
void turnLeft() {
  // Swap these if turning wrong way
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}
```

### Problem: Robot Gets Too Close to Fire

**Diagnosis:**
- SAFE_DISTANCE value too small
- Ultrasonic sensor not working

**Solutions:**
- Increase SAFE_DISTANCE in code (try 25-30cm)
- Check ultrasonic sensor wiring
- Test ultrasonic sensor separately:
```cpp
void loop() {
  int dist = getDistance();
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");
  delay(500);
}
```

### Problem: Robot Never Stops at Fire

**Diagnosis:**
- Ultrasonic sensor failing
- SAFE_DISTANCE logic not working

**Solutions:**
- Add timeout-based stopping:
```cpp
// In APPROACHING_FIRE state
unsigned long approachStartTime = millis();
if(millis() - approachStartTime > 5000) {  // 5 seconds max
  stopMotors();
  currentState = EXTINGUISHING;
}
```

## 🔧 Code and Logic Issues

### Problem: Robot Stuck in One State

**Diagnosis:**
- State machine not transitioning
- Logic condition never met

**Debug Code:**
```cpp
void loop() {
  // Add at start of loop
  static unsigned long lastDebug = 0;
  if(millis() - lastDebug > 1000) {
    Serial.print("State: ");
    Serial.print(currentState);
    Serial.print(" | Sensors: L=");
    Serial.print(analogRead(FLAME_SENSOR_LEFT));
    Serial.print(" C=");
    Serial.print(analogRead(FLAME_SENSOR_CENTER));
    Serial.print(" R=");
    Serial.println(analogRead(FLAME_SENSOR_RIGHT));
    lastDebug = millis();
  }
  
  // Rest of your code...
}
```

**Solutions:**
- Review state transition conditions
- Check if sensor thresholds are correct
- Ensure distance checking works properly

### Problem: Erratic Behavior

**Symptoms:**
- Robot does random things
- Inconsistent responses
- Sometimes works, sometimes doesn't

**Diagnosis:**
- Check for memory corruption
- Look for array out of bounds
- Check for uninitialized variables

**Solutions:**
- Add more delay in main loop (if too fast)
- Reset Arduino and re-upload code
- Check for loose connections causing noise

### Problem: Serial Monitor Shows Garbage

**Diagnosis:**
- Baud rate mismatch

**Solutions:**
- Ensure Serial.begin(9600) in setup()
- Set Serial Monitor to 9600 baud
- Check for loose USB connection

## 🛠️ Mechanical Issues

### Problem: Wheels Don't Spin Freely

**Diagnosis:**
- Mechanical binding
- Motor shaft alignment

**Solutions:**
- Check for obstructions
- Loosen and realign motor mounting
- Ensure chassis not dragging on ground
- Check castor wheel for freedom of movement

### Problem: Robot Doesn't Move Straight

**Diagnosis:**
- Motors running at different speeds
- Wheels different sizes
- Weight unbalanced

**Solutions:**
- Balance weight distribution
- Adjust individual motor speeds:
```cpp
analogWrite(MOTOR_ENABLE_LEFT, 180);   // Adjust these
analogWrite(MOTOR_ENABLE_RIGHT, 200);  // individually
```
- Ensure wheels are same size

### Problem: Water Tank Leaking

**Solutions:**
- Seal lid with hot glue or silicone
- Check pump connections
- Ensure tubing is secure
- Use rubber grommets for pipe holes

## 📊 Diagnostic Tools

### Test Each Subsystem

**Motor Test:**
```cpp
void testMotors() {
  Serial.println("Testing Left Motor Forward");
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  delay(2000);
  stopMotors();
  delay(1000);
  
  Serial.println("Testing Right Motor Forward");
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  delay(2000);
  stopMotors();
  delay(1000);
}
```

**Sensor Test:**
```cpp
void testSensors() {
  Serial.print("Left: "); Serial.print(analogRead(A0));
  Serial.print(" Center: "); Serial.print(analogRead(A1));
  Serial.print(" Right: "); Serial.println(analogRead(A2));
  delay(500);
}
```

**Pump Test:**
```cpp
void testPump() {
  Serial.println("Pump ON");
  digitalWrite(WATER_PUMP, HIGH);
  delay(2000);
  Serial.println("Pump OFF");
  digitalWrite(WATER_PUMP, LOW);
  delay(2000);
}
```

## 🔍 Multimeter Usage

### Key Measurements

**Voltage Checks:**
- Battery: 7.4V or 11.1V (nominal)
- Arduino 5V pin: 4.8-5.2V
- Arduino Vin: Same as battery
- Sensor VCC: ~5V
- Motor terminals: Varies with PWM

**Continuity Checks:**
- All GND connections should have continuity
- Check for shorts between power and ground (should be infinite resistance)

**Current Checks:**
- Idle current: 50-100mA
- Motors running: 500-1500mA
- Pump running: 300-800mA
- Total: Should be within battery capacity

## 🆘 When to Ask for Help

If you've tried all troubleshooting steps and still have issues:

1. **Document the problem:**
   - What exactly is happening?
   - What have you already tried?
   - Any error messages?
   - Photos of your setup

2. **Gather information:**
   - Serial Monitor output
   - Multimeter readings
   - Component specifications

3. **Ask in the right place:**
   - GitHub Issues (this repository)
   - Arduino Forums
   - Reddit r/arduino
   - Local maker spaces

4. **Provide context:**
   - Your code (if modified)
   - Wiring photos
   - Component list
   - What works and what doesn't

## 📝 Preventive Maintenance

### Regular Checks (Before Each Use)
- [ ] Battery voltage adequate
- [ ] All connections secure
- [ ] Wheels spin freely
- [ ] Water tank filled
- [ ] Sensors clean
- [ ] No visible damage

### Periodic Maintenance (Weekly)
- [ ] Check motor brushes (if accessible)
- [ ] Clean pump filter
- [ ] Inspect wires for wear
- [ ] Test battery capacity
- [ ] Verify all screws tight
- [ ] Clean sensor lenses

### Long-term Care
- Store in dry place
- Remove battery if storing > 1 month
- Keep away from moisture
- Protect from dust and debris
- Document any modifications

---

**Remember:** Patience and systematic debugging are key! Most issues can be resolved by careful checking of connections and logic.

**Still stuck? Open an issue on GitHub with detailed information about your problem!**
