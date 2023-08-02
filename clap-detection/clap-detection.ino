#include "sensor.h"
#include "led.h"

#define SENSOR_PIN 7
#define LED_PIN 8

Sensor sensor(SENSOR_PIN);
Led led(LED_PIN);

void setup() {
  // nothing to do
}

void loop() {
  if (sensor.is_triggered()) {
    led.toggle();
  }
}
