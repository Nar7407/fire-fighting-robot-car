#!/usr/bin/env python3
"""
Fire Fighting Robot Car - Raspberry Pi Version
Supports autonomous and remote control via WiFi
Author: Nar7407
Date: 2026-01-12
"""

import RPi.GPIO as GPIO
import time
import threading
from config import *

class FireFightingRobot:
    def __init__(self):
        # Setup GPIO
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        
        # Motor pins setup
        GPIO.setup(MOTOR_LEFT_FWD, GPIO.OUT)
        GPIO.setup(MOTOR_LEFT_BWD, GPIO.OUT)
        GPIO.setup(MOTOR_RIGHT_FWD, GPIO.OUT)
        GPIO.setup(MOTOR_RIGHT_BWD, GPIO.OUT)
        
        # PWM for motor speed control
        GPIO.setup(MOTOR_LEFT_EN, GPIO.OUT)
        GPIO.setup(MOTOR_RIGHT_EN, GPIO.OUT)
        self.left_pwm = GPIO.PWM(MOTOR_LEFT_EN, 1000)
        self.right_pwm = GPIO.PWM(MOTOR_RIGHT_EN, 1000)
        self.left_pwm.start(0)
        self.right_pwm.start(0)
        
        # Flame sensors
        GPIO.setup(FLAME_LEFT, GPIO.IN)
        GPIO.setup(FLAME_CENTER, GPIO.IN)
        GPIO.setup(FLAME_RIGHT, GPIO.IN)
        
        # Ultrasonic sensor
        GPIO.setup(TRIG_PIN, GPIO.OUT)
        GPIO.setup(ECHO_PIN, GPIO.IN)
        
        # Pump and buzzer
        GPIO.setup(PUMP_PIN, GPIO.OUT)
        GPIO.setup(BUZZER_PIN, GPIO.OUT)
        GPIO.output(PUMP_PIN, GPIO.LOW)
        
        self.autonomous_mode = True
        self.running = True
        
        print("Fire Fighting Robot Initialized")
        self.beep(2)
    
    def get_distance(self):
        """Measure distance using ultrasonic sensor"""
        GPIO.output(TRIG_PIN, GPIO.LOW)
        time.sleep(0.00001)
        GPIO.output(TRIG_PIN, GPIO.HIGH)
        time.sleep(0.00001)
        GPIO.output(TRIG_PIN, GPIO.LOW)
        
        pulse_start = time.time()
        pulse_end = time.time()
        
        timeout = time.time() + 0.1
        while GPIO.input(ECHO_PIN) == GPIO.LOW and time.time() < timeout:
            pulse_start = time.time()
        
        timeout = time.time() + 0.1
        while GPIO.input(ECHO_PIN) == GPIO.HIGH and time.time() < timeout:
            pulse_end = time.time()
        
        pulse_duration = pulse_end - pulse_start
        distance = pulse_duration * 17150
        distance = round(distance, 2)
        
        return distance if distance < 400 else 400
    
    def read_flame_sensors(self):
        """Read all flame sensors"""
        return {
            'left': not GPIO.input(FLAME_LEFT),      # Inverted logic
            'center': not GPIO.input(FLAME_CENTER),
            'right': not GPIO.input(FLAME_RIGHT)
        }
    
    def move_forward(self, speed=70):
        """Move robot forward"""
        self.left_pwm.ChangeDutyCycle(speed)
        self.right_pwm.ChangeDutyCycle(speed)
        GPIO.output(MOTOR_LEFT_FWD, GPIO.HIGH)
        GPIO.output(MOTOR_LEFT_BWD, GPIO.LOW)
        GPIO.output(MOTOR_RIGHT_FWD, GPIO.HIGH)
        GPIO.output(MOTOR_RIGHT_BWD, GPIO.LOW)
    
    def move_backward(self, speed=70):
        """Move robot backward"""
        self.left_pwm.ChangeDutyCycle(speed)
        self.right_pwm.ChangeDutyCycle(speed)
        GPIO.output(MOTOR_LEFT_FWD, GPIO.LOW)
        GPIO.output(MOTOR_LEFT_BWD, GPIO.HIGH)
        GPIO.output(MOTOR_RIGHT_FWD, GPIO.LOW)
        GPIO.output(MOTOR_RIGHT_BWD, GPIO.HIGH)
    
    def turn_left(self, speed=60):
        """Turn robot left"""
        self.left_pwm.ChangeDutyCycle(speed)
        self.right_pwm.ChangeDutyCycle(speed)
        GPIO.output(MOTOR_LEFT_FWD, GPIO.LOW)
        GPIO.output(MOTOR_LEFT_BWD, GPIO.HIGH)
        GPIO.output(MOTOR_RIGHT_FWD, GPIO.HIGH)
        GPIO.output(MOTOR_RIGHT_BWD, GPIO.LOW)
    
    def turn_right(self, speed=60):
        """Turn robot right"""
        self.left_pwm.ChangeDutyCycle(speed)
        self.right_pwm.ChangeDutyCycle(speed)
        GPIO.output(MOTOR_LEFT_FWD, GPIO.HIGH)
        GPIO.output(MOTOR_LEFT_BWD, GPIO.LOW)
        GPIO.output(MOTOR_RIGHT_FWD, GPIO.LOW)
        GPIO.output(MOTOR_RIGHT_BWD, GPIO.HIGH)
    
    def stop_motors(self):
        """Stop all motors"""
        GPIO.output(MOTOR_LEFT_FWD, GPIO.LOW)
        GPIO.output(MOTOR_LEFT_BWD, GPIO.LOW)
        GPIO.output(MOTOR_RIGHT_FWD, GPIO.LOW)
        GPIO.output(MOTOR_RIGHT_BWD, GPIO.LOW)
        self.left_pwm.ChangeDutyCycle(0)
        self.right_pwm.ChangeDutyCycle(0)
    
    def extinguish_fire(self):
        """Activate water pump to extinguish fire"""
        print("Extinguishing fire!")
        GPIO.output(PUMP_PIN, GPIO.HIGH)
        self.beep(3)
        time.sleep(PUMP_DURATION)
        GPIO.output(PUMP_PIN, GPIO.LOW)
        
        # Move back
        self.move_backward(70)
        time.sleep(1)
        self.stop_motors()
    
    def avoid_obstacle(self):
        """Obstacle avoidance logic"""
        self.stop_motors()
        time.sleep(0.3)
        self.move_backward(70)
        time.sleep(0.5)
        self.stop_motors()
        
        # Turn to avoid
        self.turn_right(60)
        time.sleep(0.5)
        self.stop_motors()
    
    def navigate_to_fire(self, flames):
        """Navigate towards detected fire"""
        if flames['center']:
            self.move_forward(70)
        elif flames['left']:
            self.turn_left(60)
        elif flames['right']:
            self.turn_right(60)
    
    def patrol(self):
        """Patrol mode - search for fire"""
        distance = self.get_distance()
        
        if distance < OBSTACLE_DISTANCE:
            self.avoid_obstacle()
        else:
            self.move_forward(60)
    
    def autonomous_operation(self):
        """Main autonomous operation loop"""
        while self.running:
            if not self.autonomous_mode:
                time.sleep(0.1)
                continue
            
            flames = self.read_flame_sensors()
            distance = self.get_distance()
            
            if any(flames.values()):
                print(f"Fire detected! L:{flames['left']} C:{flames['center']} R:{flames['right']}")
                self.beep(1)
                
                if distance < FIRE_NEAR_DISTANCE:
                    self.stop_motors()
                    self.extinguish_fire()
                elif distance < OBSTACLE_DISTANCE:
                    self.avoid_obstacle()
                else:
                    self.navigate_to_fire(flames)
            else:
                self.patrol()
            
            time.sleep(0.05)
    
    def beep(self, times):
        """Buzzer beep"""
        for _ in range(times):
            GPIO.output(BUZZER_PIN, GPIO.HIGH)
            time.sleep(0.1)
            GPIO.output(BUZZER_PIN, GPIO.LOW)
            time.sleep(0.1)
    
    def cleanup(self):
        """Cleanup GPIO"""
        self.running = False
        self.stop_motors()
        self.left_pwm.stop()
        self.right_pwm.stop()
        GPIO.cleanup()
        print("Cleaned up GPIO")

def main():
    robot = FireFightingRobot()
    
    try:
        # Start autonomous operation in separate thread
        auto_thread = threading.Thread(target=robot.autonomous_operation)
        auto_thread.start()
        
        print("Robot running. Press Ctrl+C to stop.")
        
        while True:
            time.sleep(1)
            
    except KeyboardInterrupt:
        print("\nStopping robot...")
        robot.cleanup()

if __name__ == "__main__":
    main()