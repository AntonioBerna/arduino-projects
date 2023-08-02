#define DEBOUNCE_DELAY 25

class Sensor {
  private:
    int pin;
    unsigned long last_reading = 0;
  public:
    Sensor(int sensor_pin);
    bool is_triggered();
};

Sensor::Sensor(int sensor_pin) : pin(sensor_pin) {
  pinMode(pin, INPUT);
}

bool Sensor::is_triggered() {
  int sensor_data = digitalRead(pin);
  if (sensor_data == LOW && (millis() - last_reading) > DEBOUNCE_DELAY) {
    last_reading = millis();
    return true;
  }
  return false;
}
