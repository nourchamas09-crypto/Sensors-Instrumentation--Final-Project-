/*
  Autonomous Firefighting and Rescue Robot
  Sensors and Instrumentation Final Project

  Main functions:
  - Flame detection
  - Temperature and humidity monitoring
  - Water flow monitoring using YF-S201
  - Line tracking sensor reading
  - Pump control through relay
  - Servo nozzle movement
  - Bluetooth phone control for testing/manual override
  - LCD real-time display
  - Serial Monitor CSV logging
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP32Servo.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ================= LCD =================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= PINS =================

// I2C LCD
#define LCD_SDA 22
#define LCD_SCL 21

// Sensors
#define FLAME_PIN 34
#define DHT_PIN 33
#define DHT_TYPE DHT11

#define LINE_LEFT 35      // OUT2
#define LINE_CENTER 32    // OUT3
#define LINE_RIGHT 13     // OUT4

#define FLOW_PIN 4        // YF-S201 signal wire

// Actuators
#define SERVO_PIN 23
#define RELAY_PIN 16

// L298N Motor Driver
#define ENA_ENB 17
#define IN1 14
#define IN2 26
#define IN3 27
#define IN4 25

// Relay logic
// Most relay modules are active LOW.
// If your relay works reversed, swap LOW and HIGH here.
#define RELAY_ON LOW
#define RELAY_OFF HIGH

// ================= OBJECTS =================

DHT dht(DHT_PIN, DHT_TYPE);
Servo nozzleServo;

// ================= BLE =================

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

bool deviceConnected = false;

#define SERVICE_UUID        "0000FFE0-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID "0000FFE1-0000-1000-8000-00805F9B34FB"

// ================= VARIABLES =================

// Flame sensor
int flameValue = 0;

// Fire is detected when flameValue < flameThreshold.
// Keep this low to avoid false fire detection.
int flameThreshold = 1000;

bool fireDetected = false;

// DHT11
float temperature = 0;
float humidity = 0;

// Motors
char command = 'S';

// Pump
bool pumpIsOn = false;

// Internal pump OFF override.
// Pressing O keeps pump OFF even if the flame sensor falsely detects fire.
// This is NOT shown on the LCD.
bool pumpOffOverride = false;

// Servo
bool servoScanEnabled = true;
int servoAngle = 90;
int servoDirection = 1;
int servoStep = 3;
unsigned long lastServoMove = 0;
int servoDelay = 15;

// Flow sensor
volatile unsigned long flowPulseCount = 0;
unsigned long lastFlowTime = 0;
float flowRate_L_min = 0;
float flowRate_mL_s = 0;
float totalWaterUsed_mL = 0;

// Tank
float tankCapacity_mL = 300.0;
float waterRemaining_mL = 300.0;
float lowWaterWarning_mL = 60.0;

// Line sensors
int lineL = 0;
int lineC = 0;
int lineR = 0;

// LCD timing
unsigned long lastLCDUpdate = 0;
int lcdInterval = 500;

// Serial logging
unsigned long lastLogTime = 0;
int logInterval = 1000;

// Obstacle alert testing command
bool obstacleAlert = false;
unsigned long obstacleStartTime = 0;
int obstacleDisplayTime = 6000;

// ================= FUNCTION DECLARATIONS =================

void stopMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();

void handleCommand(char cmd);

void readSensors();
void updateFireResponse();
void updatePumpFlow();
void updateServoScan();
void updateLCD();
void logData();

void pumpOn();
void pumpOff();

void IRAM_ATTR flowISR();

// ================= BLE CALLBACKS =================

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("BLE phone connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("BLE phone disconnected");
    pServer->getAdvertising()->start();
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();

    if (value.length() > 0) {
      char cmd = value.charAt(0);

      Serial.print("BLE command: ");
      Serial.println(cmd);

      handleCommand(cmd);
    }
  }
};

// ================= SETUP =================

void setup() {
  Serial.begin(115200);

  // LCD
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Fire Robot");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1500);
  lcd.clear();

  // Sensors
  pinMode(FLAME_PIN, INPUT);
  pinMode(LINE_LEFT, INPUT);
  pinMode(LINE_CENTER, INPUT);
  pinMode(LINE_RIGHT, INPUT);

  // Flow sensor
  pinMode(FLOW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flowISR, RISING);

  dht.begin();

  // Relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);
  pumpIsOn = false;

  // Motors
  pinMode(ENA_ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // No motor PWM, to avoid interfering with servo PWM.
  digitalWrite(ENA_ENB, HIGH);
  stopMotors();

  // Servo
  // This is the setup that worked in your servo-only test.
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  nozzleServo.setPeriodHertz(50);
  nozzleServo.attach(SERVO_PIN, 500, 2400);
  nozzleServo.write(90);
  servoScanEnabled = true;

  // BLE
  BLEDevice::init("FireRobotBLE");

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_WRITE_NR |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("Fire Robot Ready");
  Serial.println("time_ms,temp_C,humidity,flame_raw,fire,pump,flow_L_min,flow_mL_s,water_mL,line_L,line_C,line_R,command");

  lcd.setCursor(0, 0);
  lcd.print("BLE Ready");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

// ================= LOOP =================

void loop() {
  readSensors();

  updateFireResponse();
  updatePumpFlow();

  // Servo scans only when pump is OFF.
  if (servoScanEnabled && !pumpIsOn) {
    updateServoScan();
  }

  if (millis() - lastLCDUpdate >= lcdInterval) {
    lastLCDUpdate = millis();
    updateLCD();
  }

  if (millis() - lastLogTime >= logInterval) {
    lastLogTime = millis();
    logData();
  }
}

// ================= SENSOR READING =================

void readSensors() {
  flameValue = analogRead(FLAME_PIN);

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  lineL = digitalRead(LINE_LEFT);
  lineC = digitalRead(LINE_CENTER);
  lineR = digitalRead(LINE_RIGHT);

  fireDetected = flameValue < flameThreshold;
}

// ================= FIRE RESPONSE =================

void updateFireResponse() {
  if (fireDetected && !pumpOffOverride) {
    pumpOn();
  }
}

// ================= PUMP =================

void pumpOn() {
  pumpIsOn = true;
  digitalWrite(RELAY_PIN, RELAY_ON);

  // When pump turns ON, the car must stop.
  stopMotors();

  // When pump turns ON, the servo must stop scanning.
  servoScanEnabled = false;
}

void pumpOff() {
  pumpIsOn = false;
  digitalWrite(RELAY_PIN, RELAY_OFF);

  // When pump turns OFF, the servo can scan again.
  servoScanEnabled = true;
}

// ================= FLOW SENSOR =================

void IRAM_ATTR flowISR() {
  flowPulseCount++;
}

void updatePumpFlow() {
  unsigned long currentTime = millis();

  if (currentTime - lastFlowTime >= 1000) {
    noInterrupts();
    unsigned long pulses = flowPulseCount;
    flowPulseCount = 0;
    interrupts();

    lastFlowTime = currentTime;

    /*
      Common YF-S201 approximation:
      Frequency in Hz = 7.5 * flow rate in L/min

      Since pulses are counted for 1 second:
      pulses per second = frequency
    */

    flowRate_L_min = pulses / 7.5;
    flowRate_mL_s = (flowRate_L_min * 1000.0) / 60.0;

    if (pumpIsOn) {
      totalWaterUsed_mL += flowRate_mL_s;
      waterRemaining_mL = tankCapacity_mL - totalWaterUsed_mL;

      if (waterRemaining_mL < 0) {
        waterRemaining_mL = 0;
      }
    }
  }
}

// ================= SERVO =================

void updateServoScan() {
  if (millis() - lastServoMove >= servoDelay) {
    lastServoMove = millis();

    servoAngle += servoDirection * servoStep;

    if (servoAngle >= 180) {
      servoAngle = 180;
      servoDirection = -1;
    }

    if (servoAngle <= 0) {
      servoAngle = 0;
      servoDirection = 1;
    }

    nozzleServo.write(servoAngle);
  }
}

// ================= BLUETOOTH COMMANDS =================

void handleCommand(char cmd) {
  command = cmd;

  if (cmd == 'F') {
    if (!pumpIsOn) {
      moveForward();
    }
  }

  else if (cmd == 'B') {
    if (!pumpIsOn) {
      moveBackward();
    }
  }

  else if (cmd == 'L') {
    if (!pumpIsOn) {
      turnLeft();
    }
  }

  else if (cmd == 'R') {
    if (!pumpIsOn) {
      turnRight();
    }
  }

  else if (cmd == 'S') {
    stopMotors();
  }

  else if (cmd == 'P') {
    pumpOffOverride = false;
    pumpOn();
    Serial.println("Pump ON");
  }

  else if (cmd == 'O') {
    pumpOffOverride = true;
    pumpOff();
    Serial.println("Pump OFF");
  }

  else if (cmd == 'V') {
    if (!pumpIsOn) {
      servoScanEnabled = true;
      Serial.println("Servo ON");

      nozzleServo.write(0);
      delay(500);
      nozzleServo.write(90);
      delay(500);
      nozzleServo.write(180);
      delay(500);

      servoAngle = 180;
      servoDirection = -1;
    } else {
      Serial.println("Servo blocked because pump is ON");
    }
  }

  else if (cmd == 'X') {
    servoScanEnabled = false;
    nozzleServo.write(90);
    servoAngle = 90;
    Serial.println("Servo OFF");
  }

  else if (cmd == 'D') {
    obstacleAlert = true;
    obstacleStartTime = millis();
    stopMotors();
    Serial.println("Obstacle alert");
  }

  else if (cmd == 'T') {
    totalWaterUsed_mL = 0;
    waterRemaining_mL = tankCapacity_mL;
    pumpOffOverride = false;
    Serial.println("Tank reset to full");
  }
}

// ================= MOTORS =================

void moveForward() {
  digitalWrite(ENA_ENB, HIGH);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(ENA_ENB, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(ENA_ENB, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(ENA_ENB, HIGH);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ================= LCD =================

void updateLCD() {
  lcd.clear();

  if (obstacleAlert) {
    lcd.setCursor(0, 0);
    lcd.print("OBSTACLE");
    lcd.setCursor(0, 1);
    lcd.print("DETECTED!");

    if (millis() - obstacleStartTime >= obstacleDisplayTime) {
      obstacleAlert = false;
    }

    return;
  }

  if (fireDetected && !pumpOffOverride) {
    lcd.setCursor(0, 0);
    lcd.print("FIRE DETECTED");
    lcd.setCursor(0, 1);
    lcd.print("Pump ON");
    return;
  }

  if (waterRemaining_mL <= lowWaterWarning_mL) {
    lcd.setCursor(0, 0);
    lcd.print("REFILL TANK!");
    lcd.setCursor(0, 1);
    lcd.print("Water:");
    lcd.print((int)waterRemaining_mL);
    lcd.print("mL");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("T:");

  if (isnan(temperature)) {
    lcd.print("--");
  } else {
    lcd.print((int)temperature);
  }

  lcd.print("C H:");

  if (isnan(humidity)) {
    lcd.print("--");
  } else {
    lcd.print((int)humidity);
  }

  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("W:");
  lcd.print((int)waterRemaining_mL);
  lcd.print(" ");

  if (pumpIsOn) {
    lcd.print("PumpON");
  } else {
    lcd.print("PumpOFF");
  }
}

// ================= SERIAL LOGGING =================

void logData() {
  Serial.print(millis());
  Serial.print(",");

  Serial.print(temperature);
  Serial.print(",");

  Serial.print(humidity);
  Serial.print(",");

  Serial.print(flameValue);
  Serial.print(",");

  Serial.print(fireDetected);
  Serial.print(",");

  Serial.print(pumpIsOn);
  Serial.print(",");

  Serial.print(flowRate_L_min);
  Serial.print(",");

  Serial.print(flowRate_mL_s);
  Serial.print(",");

  Serial.print(waterRemaining_mL);
  Serial.print(",");

  Serial.print(lineL);
  Serial.print(",");

  Serial.print(lineC);
  Serial.print(",");

  Serial.print(lineR);
  Serial.print(",");

  Serial.println(command);
}
