#pragma once

// Switch Anti-Bounce
#define LED_PIN 13
#define SWITCH_PIN 7
#define DEBOUNCE_DELAY 50

// IR
#include <IRremote.hpp>
#define IR_RECEIVE_PIN 11
#define IR_TURN_ON 0x40 // key: OK
#define IR_TURN_OFF 0x52 // key: 0

// Bluetooth (set by Android App)
#define BLUE_TURN_ON 97 // 'a'
#define BLUE_TURN_OFF 98 // 'b'
