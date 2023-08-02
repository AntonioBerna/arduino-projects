class Led {
  private:
    int pin;
    bool state;
  public:
    Led(int led_pin);
    void toggle();
};

Led::Led(int led_pin) : pin(led_pin), state(false) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void Led::toggle() {
  state = !state;
  digitalWrite(pin, state ? HIGH : LOW);
}
