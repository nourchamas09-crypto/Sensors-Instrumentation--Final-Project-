/*
  5-Channel Line Tracking Sensor Test
  Autonomous Firefighting and Rescue Robot

  Purpose:
  Tests the line tracking sensor outputs before integration.

  Final prototype uses:
  OUT2 -> Left
  OUT3 -> Center
  OUT4 -> Right
*/

#define LINE_LEFT 35      // OUT2
#define LINE_CENTER 32    // OUT3
#define LINE_RIGHT 13     // OUT4

int lineL = 0;
int lineC = 0;
int lineR = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LINE_LEFT, INPUT);
  pinMode(LINE_CENTER, INPUT);
  pinMode(LINE_RIGHT, INPUT);

  Serial.println("Line Tracking Sensor Test");
  Serial.println("time_ms,left_OUT2,center_OUT3,right_OUT4");
}

void loop() {
  lineL = digitalRead(LINE_LEFT);
  lineC = digitalRead(LINE_CENTER);
  lineR = digitalRead(LINE_RIGHT);

  Serial.print(millis());
  Serial.print(",");
  Serial.print(lineL);
  Serial.print(",");
  Serial.print(lineC);
  Serial.print(",");
  Serial.println(lineR);

  delay(300);
}
