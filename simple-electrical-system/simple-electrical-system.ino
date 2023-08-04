#include "configs.hpp"

// Switch Anti-Bounce
uint8_t led_state = LOW;
uint8_t switch_state = LOW;
uint8_t last_read = LOW;
unsigned long last_time = 0;

// Bluetooth
int incoming_byte = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(LED_PIN, led_state);

  IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop() {
  // IR
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);

    IrReceiver.resume();
    if (IrReceiver.decodedIRData.command == IR_TURN_ON) {
      digitalWrite(LED_PIN, HIGH);
    } else if (IrReceiver.decodedIRData.command == IR_TURN_OFF) {
      digitalWrite(LED_PIN, LOW);
    }
  }

  // Bluetooth
  if (Serial.available() > 0) {
    incoming_byte = Serial.read();

    Serial.println(incoming_byte);
    if (incoming_byte == BLUE_TURN_ON) {
      digitalWrite(LED_PIN, HIGH);
    } else if (incoming_byte == BLUE_TURN_OFF) {
      digitalWrite(LED_PIN, LOW);
    }
  }

  // Switch Anti-Bounce
  int new_read = digitalRead(SWITCH_PIN);
  if (new_read != last_read) {
    last_time = millis();
  }

  if ((millis() - last_time) > DEBOUNCE_DELAY) {
    if (new_read != switch_state && new_read == HIGH) {
      led_state = !led_state;
      digitalWrite(LED_PIN, led_state);
    }
    switch_state = new_read;
  }
  last_read = new_read;
}
