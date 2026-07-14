# Pinout Table

| Component | Pin / Signal | ESP32 Pin | Notes |
|---|---:|---:|---|
| LCD1602 I2C | SDA | GPIO22 | I2C data |
| LCD1602 I2C | SCL | GPIO21 | I2C clock |
| LCD1602 I2C | VCC | 5V / 3.3V | Depending on module |
| LCD1602 I2C | GND | GND | Common ground |
| Flame Sensor | AO | GPIO34 | Analog input |
| DHT11 | DATA | GPIO33 | Temperature and humidity |
| Line Sensor | OUT2 Left | GPIO35 | Digital input |
| Line Sensor | OUT3 Center | GPIO32 | Digital input |
| Line Sensor | OUT4 Right | GPIO13 | Digital input |
| Flow Sensor YF-S201 | Signal | GPIO4 | Digital pulse input / interrupt |
| Servo MG996R | Signal | GPIO23 | External 5V recommended |
| Relay Module | IN | GPIO16 | Controls 12V pump |
| L298N Motor Driver | ENA + ENB | GPIO17 | PWM motor speed |
| L298N Motor Driver | IN1 | GPIO14 | Motor direction |
| L298N Motor Driver | IN2 | GPIO26 | Motor direction |
| L298N Motor Driver | IN3 | GPIO27 | Motor direction |
| L298N Motor Driver | IN4 | GPIO25 | Motor direction |
| Battery | +12V | Motor driver / pump | Power source |
| Buck Converter | 5V output | Servo / logic supply | Step-down from battery |
| All modules | GND | ESP32 GND | All grounds connected together |
