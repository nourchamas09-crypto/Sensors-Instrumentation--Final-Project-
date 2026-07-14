# Bill of Materials

This table lists the main components used in the Autonomous Firefighting and Rescue Robot project.

| # | Component | Quantity | Purpose / Notes |
|---|---|---:|---|
| 1 | ESP32 Development Board | 1 | Main microcontroller used to read sensors and control actuators |
| 2 | 12V 4400mAh 18650 Lithium-ion Battery + Charger | 1 | Main power source for the robot |
| 3 | XL4015 DC-DC Step-Down Converter | 1 | Steps down battery voltage for lower-voltage modules |
| 4 | L298N Motor Driver Module | 1 | Controls the DC motors for robot movement |
| 5 | Smart Car DC Motors with Wheels and Brackets | 4 | Robot movement |
| 6 | 1-Channel 5V Relay Module | 1 | Controls the 12V water pump |
| 7 | Ultra-Quiet DC 12V 1.5M 200L/H Brushless Submersible Water Pump | 1 | Sprays water when fire is detected |
| 8 | Servo Motor MG996R 180 Degree | 1 | Controls the nozzle scanning direction |
| 9 | 4-Pin IR Infrared Flame Detection Sensor Module | 1 | Optical sensor used for fire/flame detection |
| 10 | DHT11 Temperature and Humidity Sensor Module | 1 | Measures ambient temperature and relative humidity |
| 11 | Water Flow Sensor YF-S201 Flowmeter G1/2 1–30 L/min | 1 | Flow sensing / water flow measurement |
| 12 | 5kg Load Cell | 1 | Force/weight sensing |
| 13 | HX711 Weight Pressure Sensor Module | 1 | Interface module for the load cell |
| 14 | 5-Channel Line Tracking Sensor Module | 1 | Detects path/line for navigation |
| 15 | LCD1602 I2C Blue Backlight Display | 1 | Real-time display for temperature, fire status, pump state, water level, and obstacle alerts |
| 16 | HC-SR04 Ultrasonic Sensor | 2 | Obstacle detection support |
| 17 | GY-BMP280 Atmospheric Pressure Sensor Module | 1 | Additional environmental sensing |
| 18 | 400-Point Solderless Breadboard | 1 | Prototyping and wiring |
| 19 | Female-to-Female Dupont Wires | 1 set | Wiring between modules |
| 20 | Male-to-Male Dupont Wires | 1 set | Wiring between modules |
| 21 | Metal Film Resistors 1kΩ and 2kΩ | 2 | Signal conditioning / voltage divider use |
| 22 | Screw Kit for Robot Accessories | 1 | Mechanical assembly |

## Notes

- The ESP32 is used as the main controller because it supports multiple GPIO pins, analog inputs, PWM control, and Bluetooth communication.
- The pump is controlled through a relay module because the ESP32 cannot power the 12V pump directly.
- The servo motor is used to scan the water nozzle direction.
- The LCD provides real-time visualization of the robot status.
- The line tracking sensor uses OUT2, OUT3, and OUT4 for left, center, and right path detection.
- The flow sensor is connected to an available interrupt-capable ESP32 pin for pulse-based flow measurement.
- All modules share a common ground with the ESP32.
