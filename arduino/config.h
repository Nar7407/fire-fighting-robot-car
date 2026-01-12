/*
 * Configuration file for Fire Fighting Robot
 * Adjust these values based on your hardware setup
 */

#ifndef CONFIG_H
#define CONFIG_H

// Flame sensor thresholds (0-1023, lower = flame detected)
#define FLAME_THRESHOLD_VALUE 500
#define FLAME_SENSITIVITY_HIGH 400
#define FLAME_SENSITIVITY_MEDIUM 500
#define FLAME_SENSITIVITY_LOW 600

// Distance thresholds (in cm)
#define OBSTACLE_DISTANCE_CM 20
#define FIRE_NEAR_DISTANCE_CM 15
#define SAFE_DISTANCE_CM 30

// Motor speeds (0-255)
#define SPEED_FAST 255
#define SPEED_NORMAL 200
#define SPEED_SLOW 150
#define SPEED_TURN 180

// Pump settings
#define PUMP_DURATION_MS 3000
#define PUMP_COOLDOWN_MS 1000

// Servo angles
#define SERVO_CENTER 90
#define SERVO_LEFT 45
#define SERVO_RIGHT 135
#define SERVO_SCAN_DELAY_MS 300

// Timing settings
#define LOOP_DELAY_MS 50
#define OBSTACLE_AVOID_DELAY_MS 500
#define BACKUP_DELAY_MS 500

// Bluetooth settings
#define BLUETOOTH_BAUD_RATE 9600

// Debug mode
#define DEBUG_MODE true

#endif