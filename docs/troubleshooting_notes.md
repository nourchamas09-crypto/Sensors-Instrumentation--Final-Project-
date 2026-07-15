# Troubleshooting Notes

This file summarizes the main problems faced during development and how they were solved.

---

## 1. Flame Sensor Sensitivity

### Problem

At first, the flame sensor did not detect the flame reliably. Its sensitivity was too low, even after adjusting the calibration screw on the sensor module.

### Cause

The calibration screw mainly affects the digital output threshold of the flame sensor module. However, the digital output was not flexible enough for our testing because the sensor response changed depending on flame distance and surrounding light.

### Solution

Instead of relying on the digital output, the analog output was used.

Final wiring:

| Flame Sensor Pin | ESP32 Pin |
|---|---|
| AO | GPIO34 |
| VCC | 3.3V |
| GND | GND |

The analog readings were monitored using the Serial Monitor. Based on the observed values, a software threshold was chosen in the code.

Final detection rule:

| Condition | Result |
|---|---|
| flameValue < flameThreshold | Fire detected |
| flameValue >= flameThreshold | No fire detected |

This gave better control than using the calibration screw alone.

---

## 2. Pressure Sensor Issue

### Problem

The pressure sensor did not work properly during testing.

### Cause

The sensor readings were not reliable enough to be used in the final integrated prototype.

### Solution

The pressure sensor was removed from the final prototype. The project focused instead on the sensors that were successfully tested and integrated, such as the flame sensor, DHT11, flow sensor, line tracking sensor, and ultrasonic sensor.

---

## 3. Force Sensor / Load Cell Integration

### Problem

A load cell and HX711 module were available for force or weight sensing, but they were not integrated into the final prototype.

### Cause

After connecting the main robot subsystems, there were not enough convenient ESP32 pins left for reliable integration of the force sensor module.

The main connected systems already included:

- LCD I2C display
- Flame sensor
- DHT11 sensor
- Flow sensor
- Line tracking sensor
- Servo motor
- Relay and pump
- Motor driver
- Bluetooth communication

### Solution

The load cell and HX711 were kept as available project components, but they were not included in the final working prototype due to limited available pins after connecting the main functions.

---

## 4. Flow Sensor Leakage

### Problem

The YF-S201 flow sensor leaked during early testing.

### Cause

The water connection around the flow sensor was not sealed tightly enough at first.

### Solution

The flow sensor connection was adjusted and fixed mechanically until the leakage stopped.

After fixing the leak, the flow sensor was used in the final project. It was connected to GPIO4 and measured water flow using pulse counting.

Final wiring:

| Flow Sensor Wire | Connection |
|---|---|
| Red | 5V |
| Black | GND |
| Yellow / Signal | GPIO4 |

The final code logs flow-related values:

- flow_L_min
- flow_mL_s
- water_mL

---

## 5. Ultrasonic Voltage Divider

### Problem

The HC-SR04 ultrasonic sensor voltage divider did not work directly at first.

### Cause

The HC-SR04 echo pin outputs a 5V signal, while the ESP32 input pins are not 5V tolerant. A voltage divider was needed, but the first wiring attempt did not give stable results.

### Solution

The voltage divider wiring was checked and adjusted. The echo signal was then connected through the divider before reaching the ESP32 input pin.

Important note:

The HC-SR04 echo pin should not be connected directly to the ESP32 because it can output 5V.

Basic ultrasonic behavior:

- ESP32 sends a trigger pulse.
- Ultrasonic sensor sends a sound wave.
- Echo signal returns.
- ESP32 measures the echo time.
- Distance is calculated.
- If the obstacle is close, the robot stops and shows an obstacle warning.

---

## 6. Line Tracking Sensor Stability

### Problem

The line tracking sensor readings were not very steady during testing.

### Cause

The line sensor readings depended on the surface color, line contrast, distance from the floor, lighting, and sensor alignment. Small changes in the robot position caused the readings to change.

### Solution

The sensor was tested through the Serial Monitor to choose the most reliable outputs.

The final prototype used three outputs:

| Sensor Output | Position | ESP32 Pin |
|---|---|---|
| OUT2 | Left | GPIO35 |
| OUT3 | Center | GPIO32 |
| OUT4 | Right | GPIO13 |

The line tracking behavior was used as a navigation/path detection feature. The robot could read the line position, but the track had to be simple and clear for stable operation.

---

## 7. Servo Motor Issue

### Problem

The servo motor worked in a separate test code but did not move correctly in the full integrated robot code.

### Cause

The servo needed proper ESP32Servo setup with timer allocation and pulse width limits. The motor PWM was also avoided to reduce interference with servo PWM behavior.

### Solution

The servo setup was changed to:

The servo setup was changed to:

```cpp
ESP32PWM::allocateTimer(0);
ESP32PWM::allocateTimer(1);
ESP32PWM::allocateTimer(2);
ESP32PWM::allocateTimer(3);
```

---

## Battery Charger Issue

### Problem

During development, the original battery charger was lost.

### Solution

A replacement charger was purchased. The new charger was rated at 12V and 1A.

Because the charging current was 1A, the battery charged more slowly, but it was still suitable for charging the 12V battery safely.

---

## Relay Module Failure

### Problem

During testing, the relay module stopped working and was damaged.

### Cause

The relay was repeatedly used to switch the 12V pump during testing. Since the pump is an inductive load, switching it can stress the relay module if the wiring or protection is not ideal.

### Solution

A replacement relay module was purchased and installed.

After replacing the relay, the pump control worked again. The final system used the relay to switch the 12V pump because the ESP32 cannot power the pump directly.
nozzleServo.setPeriodHertz(50);
nozzleServo.attach(SERVO_PIN, 500, 2400);
