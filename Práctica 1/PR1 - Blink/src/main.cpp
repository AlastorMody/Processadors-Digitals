#include <Arduino.h>

int PIN_RED_LED = 22;
int PIN_BLUE_LED = 19;
int PIN_YELLOW_LED = 17;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_BLUE_LED, OUTPUT);
  pinMode(PIN_YELLOW_LED, OUTPUT);
}

void loop() {

  digitalWrite(PIN_RED_LED, HIGH);
  Serial.println("LED RED ON");
  delay(500);

  digitalWrite(PIN_BLUE_LED, HIGH);
  Serial.println("LED BLUE ON");
  delay(500);

  digitalWrite(PIN_YELLOW_LED, HIGH);
  Serial.println("LED YELLOW ON");
  delay(500);

  digitalWrite(PIN_RED_LED, LOW);
  Serial.println("LED RED OFF");
  delay(500);

  digitalWrite(PIN_BLUE_LED, LOW);
  Serial.println("LED BLUE OFF");
  delay(500);

  digitalWrite(PIN_YELLOW_LED, LOW);
  Serial.println("LED YELLOW OFF");
  delay(500);

}