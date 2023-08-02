#define ANALOG_PIN_A0 A0

const int led_pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int number_of_leds = sizeof(led_pins) / sizeof(int);

void led_bar(int led_level) {
    for (int i = 0; i < number_of_leds; i++) {
        if (i < led_level) {
            digitalWrite(led_pins[i], HIGH);
        } else {
            digitalWrite(led_pins[i], LOW);
        }
    }
}

void setup() {
    for (int i = 0; i < number_of_leds; i++) {
        pinMode(led_pins[i], OUTPUT);
    }
}

void loop() {
    int potentiometer = analogRead(ANALOG_PIN_A0);
    int led_level = map(potentiometer, 0, 1023, 0, number_of_leds);
    led_bar(led_level);
}
