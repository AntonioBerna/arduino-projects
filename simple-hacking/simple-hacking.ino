#define OPTOCOUPLER_PIN 7
#define MS 500

unsigned long prev = 0;
bool state = false;

void toggle() {
  state = !state;
  digitalWrite(OPTOCOUPLER_PIN, state ? HIGH : LOW);
}

void setup() {
  pinMode(OPTOCOUPLER_PIN, OUTPUT);
}

void loop() {
  unsigned long curr = millis();
  
  if (curr - prev >= MS) {
    prev = curr;
    toggle();
  }
}
