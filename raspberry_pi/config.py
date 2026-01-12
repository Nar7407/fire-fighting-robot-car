"""
Configuration file for Raspberry Pi Fire Fighting Robot
Adjust pin numbers and thresholds based on your setup
"""

# Motor pins (BCM numbering)
MOTOR_LEFT_FWD = 17
MOTOR_LEFT_BWD = 27
MOTOR_RIGHT_FWD = 23
MOTOR_RIGHT_BWD = 24
MOTOR_LEFT_EN = 12
MOTOR_RIGHT_EN = 13

# Flame sensor pins (digital output)
FLAME_LEFT = 5
FLAME_CENTER = 6
FLAME_RIGHT = 26

# Ultrasonic sensor pins
TRIG_PIN = 20
ECHO_PIN = 21

# Other component pins
PUMP_PIN = 16
BUZZER_PIN = 19

# Thresholds and settings
OBSTACLE_DISTANCE = 20  # cm
FIRE_NEAR_DISTANCE = 15  # cm
PUMP_DURATION = 3  # seconds

# Motor speeds (0-100%)
SPEED_FAST = 90
SPEED_NORMAL = 70
SPEED_SLOW = 50
SPEED_TURN = 60