# Autonomous Firefighting Robot

## Project Overview

This project is an ESP32-based autonomous firefighting and rescue robot prototype designed for indoor emergency response.

The robot can detect fire, monitor temperature and humidity, measure water flow, detect obstacles, read line/path sensors, activate a water pump, move a servo-controlled nozzle, and display real-time status on an LCD.

Bluetooth Low Energy is also used for testing and direct control during the demonstration.

---

## Main Features

- Flame/fire detection using an analog flame sensor
- Temperature and humidity monitoring using DHT11
- Water flow measurement using YF-S201 flow sensor
- Line/path detection using a 5-channel line tracking sensor
- Obstacle detection using HC-SR04 ultrasonic sensor
- Pump activation through a relay module
- Servo-controlled nozzle scanning
- LCD1602 I2C real-time display
- Bluetooth phone control for testing and direct movement
- Serial Monitor CSV data logging

---

## Sensors Used

| Sensor | Purpose |
|---|---|
| Flame sensor | Detects fire/flame presence |
| DHT11 | Measures temperature and humidity |
| YF-S201 flow sensor | Measures water flow rate |
| 5-channel line tracking sensor | Detects line/path position |
| HC-SR04 ultrasonic sensor | Detects nearby obstacles |
| Load cell + HX711 | Available force/weight sensing module, but not integrated in the final prototype due to limited available pins |

---

## Actuators and Output Devices

| Component | Purpose |
|---|---|
| DC motors | Robot movement |
| L298N motor driver | Motor control |
| Relay module | Controls the 12V water pump |
| 12V water pump | Sprays water when fire is detected |
| MG996R servo motor | Moves the nozzle direction |
| LCD1602 I2C | Displays real-time robot status |

---

## Final System Behavior

During normal operation, the ESP32 reads the sensors, updates the LCD, and logs data through the Serial Monitor.

When fire is detected:

- The robot stops moving.
- The pump turns ON.
- The servo stops scanning.
- The LCD displays `FIRE DETECTED`.
- Flow sensor data is logged during pump operation.

When the pump is OFF:

- The servo can scan the nozzle direction.
- The robot can move using Bluetooth commands.
- Sensor values continue to be displayed and logged.

---

## Bluetooth Commands

| Command | Function |
|---|---|
| F | Move forward |
| B | Move backward |
| L | Turn left |
| R | Turn right |
| S | Stop |
| P | Pump ON |
| O | Pump OFF |
| V | Servo ON |
| X | Servo OFF |
| T | Reset tank level after refill |

---

## Repository Structure

```text
README.md
docs/
code/
data/
video/
report/
