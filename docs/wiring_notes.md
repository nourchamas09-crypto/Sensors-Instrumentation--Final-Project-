# Wiring Notes

This file explains the main wiring connections used in the Autonomous Firefighting and Rescue Robot.

---

## 1. ESP32 Main Controller

The ESP32 is used as the main controller for the robot. It reads the sensors, controls the actuators, updates the LCD, receives Bluetooth commands, and logs data through the Serial Monitor.

All modules must share a common ground with the ESP32.

---

## 2. LCD1602 I2C Display

| LCD Pin | ESP32 Connection |
|---|---|
| SDA | GPIO22 |
| SCL | GPIO21 |
| VCC | 5V / 3.3V depending on module |
| GND | GND |

The LCD is used to display real-time robot status such as temperature, humidity, pump status, water level, fire warning, and obstacle warning.

---

## 3. Flame Sensor

| Flame Sensor Pin | ESP32 Connection |
|---|---|
| AO | GPIO34 |
| VCC | 3.3V |
| GND | GND |

The flame sensor is connected through the analog output pin. The ESP32 reads the analog value and compares it with a threshold to detect fire.

Fire detection rule:

- If flame reading is below the threshold, fire is detected.
- If flame reading is above the threshold, no fire is detected.

---

## 4. DHT11 Temperature and Humidity Sensor

| DHT11 Pin | ESP32 Connection |
|---|---|
| DATA | GPIO33 |
| VCC | 3.3V |
| GND | GND |

The DHT11 sensor measures ambient temperature and relative humidity.

---

## 5. YF-S201 Water Flow Sensor

| Flow Sensor Wire | ESP32 Connection |
|---|---|
| Signal / Yellow | GPIO4 |
| VCC / Red | 5V |
| GND / Black | GND |

The YF-S201 flow sensor is used to measure water flow during pump operation.

The sensor produces pulses when water passes through it. The ESP32 counts these pulses using an interrupt on GPIO4 and calculates the flow rate.

The flow sensor is actually used in the project code, and the logged data includes:

- flow_L_min
- flow_mL_s
- water_mL

---

## 6. Line Tracking Sensor

The 5-channel line tracking sensor is used for line/path detection.

| Line Sensor Output | Position | ESP32 Pin |
|---|---|---|
| OUT2 | Left | GPIO35 |
| OUT3 | Center | GPIO32 |
| OUT4 | Right | GPIO13 |

The line tracking sensor can detect the position of the line and help the robot correct its movement.

---

## 7. Servo Motor MG996R

| Servo Wire | Connection |
|---|---|
| Signal | GPIO23 |
| VCC / Red | 5V from buck converter |
| GND / Brown or Black | GND |

The servo motor is powered from the buck converter, not directly from the ESP32.

The ESP32 ground and buck converter ground must be connected together.

The servo moves the nozzle while the pump is OFF. When the pump turns ON, the servo stops so water is sprayed in one direction.

---

## 8. Relay Module and Water Pump

| Relay Pin | ESP32 / Power Connection |
|---|---|
| IN | GPIO16 |
| VCC | 5V |
| GND | GND |

The relay is used to switch the 12V water pump.

The ESP32 cannot power the pump directly, so the relay acts as an electrical switch.

Pump behavior:

- Pump ON when fire is detected or when command P is received.
- Pump OFF when command O is received.
- When the pump turns ON, the robot motors stop.
- When the pump turns ON, the servo stops scanning.

---

## 9. L298N Motor Driver

| L298N Pin | ESP32 Connection |
|---|---|
| ENA + ENB | GPIO17 |
| IN1 | GPIO14 |
| IN2 | GPIO26 |
| IN3 | GPIO27 |
| IN4 | GPIO25 |

The L298N motor driver controls the robot DC motors.

The motors are powered from the 12V battery through the motor driver.

---

## 10. HC-SR04 Ultrasonic Sensor

The HC-SR04 ultrasonic sensor is used for obstacle detection.

| HC-SR04 Pin | ESP32 Connection |
|---|---|
| TRIG | ESP32 digital output pin |
| ECHO | ESP32 digital input pin through voltage divider |
| VCC | 5V |
| GND | GND |

The echo pin can output 5V, so a voltage divider should be used before connecting it to the ESP32 input pin.

Obstacle behavior:

- The ultrasonic sensor measures distance.
- If the obstacle is closer than the safety threshold, the robot stops.
- The LCD shows an obstacle warning.

---

## 11. Power Wiring

The robot uses a 12V 4400mAh lithium-ion battery as the main power source.

Power distribution:

| Component | Power Source |
|---|---|
| Motors | 12V battery through L298N |
| Pump | 12V battery through relay |
| Servo | 5V buck converter |
| ESP32 | USB / regulated supply |
| Sensors | 3.3V or 5V depending on module |
| LCD | 5V or 3.3V depending on module |

Important note:

All grounds must be connected together:

- ESP32 GND
- Battery GND
- Buck converter GND
- L298N GND
- Relay GND
- Sensor GND
- Servo GND

Without common ground, the robot may behave randomly or some modules may not work.

---

## 12. Wiring Summary

| Component | ESP32 Pin |
|---|---|
| LCD SDA | GPIO22 |
| LCD SCL | GPIO21 |
| Flame sensor AO | GPIO34 |
| DHT11 DATA | GPIO33 |
| Flow sensor signal | GPIO4 |
| Line sensor OUT2 | GPIO35 |
| Line sensor OUT3 | GPIO32 |
| Line sensor OUT4 | GPIO13 |
| Servo signal | GPIO23 |
| Relay IN | GPIO16 |
| L298N ENA + ENB | GPIO17 |
| L298N IN1 | GPIO14 |
| L298N IN2 | GPIO26 |
| L298N IN3 | GPIO27 |
| L298N IN4 | GPIO25 |
