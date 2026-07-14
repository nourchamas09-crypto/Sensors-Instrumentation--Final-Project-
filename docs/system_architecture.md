# System Architecture

## Overview

The robot is an ESP32-based indoor firefighting and rescue robot prototype.

The ESP32 acts as the main controller. It reads sensors, controls actuators, displays status on the LCD, logs data through the Serial Monitor, and receives commands from a phone through Bluetooth.

The robot monitors fire, temperature, humidity, water flow, navigation status, and obstacle alerts.

A load cell and HX711 module were included in the project components, but they were not integrated into the final prototype because the available ESP32 pins were already assigned to the main sensing, movement, display, pump, and communication subsystems.

The system is divided into five main parts:

1. Control unit
2. Sensor subsystem
3. Actuator subsystem
4. Power subsystem
5. Communication and data logging

---

## 1. Control Unit

The ESP32 is the main microcontroller used in the project.

It is responsible for:

- Reading sensor values
- Detecting fire conditions
- Controlling the motors
- Controlling the pump relay
- Moving the servo nozzle
- Updating the LCD display
- Receiving Bluetooth commands
- Printing CSV data through the Serial Monitor

---

## 2. Sensor Subsystem

| Sensor | Purpose | Interface |
|---|---|---|
| Flame sensor | Detects flame/fire presence | Analog input |
| DHT11 | Measures temperature and humidity | Digital input |
| Flow sensor YF-S201 | Measures water flow rate using pulses | Digital pulse input |
| 5-channel line tracking sensor | Detects path/line for navigation | Digital inputs |
| HC-SR04 ultrasonic sensor | Detects obstacles by measuring distance | Digital trigger/echo |
| Load cell + HX711 | Force/weight sensing module available, but not integrated in the final prototype due to limited available pins after connecting the main robot subsystems | HX711 digital interface |

The flame sensor is used as the main emergency detection sensor. When the flame sensor detects fire, the robot automatically stops and activates the water pump.

The flow sensor is used to measure water flow during pump operation by producing pulses that are counted by the ESP32.

The line tracking sensor uses OUT2, OUT3, and OUT4 for left, center, and right path detection.

---

## 3. Actuator Subsystem

| Actuator | Purpose |
|---|---|
| DC motors | Move the robot forward, backward, left, and right |
| L298N motor driver | Controls motor direction and speed |
| Relay module | Switches the 12V pump ON/OFF |
| 12V water pump | Sprays water when fire is detected |
| Servo motor | Rotates the nozzle direction |
| LCD1602 I2C | Displays real-time system status |

The pump is controlled through a relay because the ESP32 cannot power the pump directly.

The servo scans the nozzle direction while the pump is OFF. When the pump turns ON, the servo automatically stops so the water is sprayed in one direction.

---

## 4. Power Subsystem

The robot uses a 12V 4400mAh lithium-ion battery as the main power source.

Power distribution:

| Part | Power Source |
|---|---|
| Motors | 12V battery through motor driver |
| Pump | 12V battery through relay |
| ESP32 | Regulated supply / USB during programming |
| Servo | 5V from buck converter |
| Sensors | 3.3V or 5V depending on module |
| LCD | 5V or 3.3V depending on module |

All modules share a common ground with the ESP32.

---

## 5. Communication

The robot uses Bluetooth Low Energy communication between the phone and the ESP32.

The phone sends single-character commands:

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
| D | Obstacle detected |
| T | Reset tank level after refill |

---

## 6. Main System Behavior

### Normal Operation

- ESP32 reads the sensors.
- LCD shows temperature, humidity, and water status.
- Robot receives Bluetooth movement commands.
- Servo scans the nozzle direction while the pump is OFF.
- Serial Monitor logs sensor data in CSV format.

### Fire Detection

- Flame sensor detects fire.
- ESP32 stops the motors.
- Relay turns the pump ON.
- Servo stops scanning.
- LCD shows FIRE DETECTED and Pump ON.
- Flow and water data are updated during pump operation.

### Pump and Flow Monitoring

The robot uses a YF-S201 water flow sensor to measure the water flow rate during pump operation.

Pump monitoring sequence:

- Pump turns ON.
- Flow sensor generates pulses.
- ESP32 counts the pulses.
- Flow rate is calculated.
- LCD displays water and pump status.
- Serial Monitor logs flow-related data.

The pump was also experimentally tested. During testing, 200 mL of water was discharged in 8 seconds.


This experimental value was used to validate the pump performance and compare it with the flow-sensor results behavior of the system.

### Obstacle Detection

The robot uses HC-SR04 ultrasonic sensors to detect nearby obstacles.

Obstacle detection behavior:

- The ultrasonic sensor sends a trigger pulse.
- The echo signal is measured by the ESP32.
- The distance to the obstacle is calculated.
- If the measured distance is below the safety threshold, the robot stops.
- LCD shows OBSTACLE DETECTED.
- The message stays for 6 seconds.
- The robot can continue after the obstacle is cleared.
---

## 7. Data Logging

The ESP32 prints CSV-formatted data through the Serial Monitor.

Logged values include:

- time_ms
- temp_C
- humidity
- flame_raw
- fire
- pump
- water_mL
- line_L
- line_C
- line_R
- mode

This data can be saved as a `.csv` file and used for plotting and analysis.

---
## 8. System Block Diagram

Phone App  
↓  
Bluetooth manual override / testing  
↓  
ESP32  

Sensors connected to ESP32:

- Flame sensor
- DHT11 temperature and humidity sensor
- YF-S201 water flow sensor
- 5-channel line tracking sensor
- HC-SR04 ultrasonic obstacle sensor
- HX711 + load cell module, not integrated in final prototype

Outputs controlled by ESP32:

- LCD display
- L298N motor driver
- DC motors
- Relay module
- 12V water pump
- Servo nozzle

Main automatic responses:

- Flame detected → motors stop → pump turns ON → LCD shows fire alert
- Obstacle detected → motors stop → LCD shows obstacle alert
- Line detected → robot adjusts movement according to sensor readings

