# Development Process

This file summarizes the main development steps followed during the project.

## 1. Project Idea

The project started as an autonomous firefighting and rescue robot for indoor emergency response. The goal was to build a mobile robot that can detect fire, monitor environmental conditions, detect obstacles, follow a path, and activate a water pump when needed.

## 2. Hardware Assembly

The robot was assembled using an ESP32 as the main controller. Sensors and actuators were connected gradually and tested one by one before being integrated into the full system.

Main hardware systems included:

- ESP32 controller
- Flame sensor
- DHT11 temperature and humidity sensor
- YF-S201 water flow sensor
- Line tracking sensor
- HC-SR04 ultrasonic sensor
- L298N motor driver
- DC motors
- Relay module
- 12V water pump
- Servo nozzle
- LCD1602 I2C display

## 3. Sensor Testing

Each sensor was tested separately before being included in the final code.

The flame sensor was tested using analog readings. The DHT11 was tested for temperature and humidity readings. The line tracking sensor was tested by checking its digital outputs. The flow sensor was tested by counting pulses during pump operation.

## 4. Actuator Testing

The motors were tested through the L298N motor driver. The pump was tested through the relay module. The servo was tested separately before being added to the full robot code.

## 5. Integration

After testing individual parts, the components were integrated into one ESP32 firmware. The system reads sensor values, updates the LCD, logs CSV data through Serial Monitor, receives Bluetooth commands, controls motors, moves the servo, and activates the pump.

## 6. Final Behavior

The final robot behavior includes:

- Fire detection using the flame sensor
- Pump activation when fire is detected
- Motor stop when pump turns ON
- Servo stop when pump turns ON
- Servo scanning when pump is OFF
- Temperature and humidity display
- Water flow monitoring
- Line sensor reading
- Obstacle warning
- Bluetooth control for testing and direct movement
- Serial Monitor CSV logging
