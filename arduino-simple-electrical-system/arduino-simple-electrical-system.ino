#include <IRremote.h>

int ledPin = 13;
int pulsantePin = 7;
int ledStato = LOW;
int pulsanteStato = LOW;

int attesaDebounce = 50;
unsigned long ultimoTempoDebounce = 0;
int ultimaLettura = LOW;

String inputString = "";
char junk;

int receiver = 11;
IRrecv irrecv(receiver);
decode_results results;

void setup() {
	Serial.begin(9600);
	irrecv.enableIRIn();
	pinMode(ledPin, OUTPUT);
	pinMode(pulsantePin, INPUT);
	digitalWrite(ledPin, ledStato);
}

void loop() {
	// Ricevitore IR
	if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX);
		irrecv.resume();
	}

	if (results.value == 0xFF02FD) { // tasto OK sul telecomando
		digitalWrite(ledPin, HIGH); // set the LED on
	} else if (results.value == 0xFF4AB5) { // tasto 0 sul telecomando
		digitalWrite(ledPin, LOW); // set the LED off
	}

	// Bluetooth
	if (Serial.available()) {
		while (Serial.available()) {
			char inChar = (char)Serial.read();
			inputString += inChar;
		}

		Serial.println(inputString);
		
		while (Serial.available() > 0) {
			junk = Serial.read();
		}

		if (inputString == "a") {
			digitalWrite(ledPin, HIGH);
		} else if (inputString == "b") {
			digitalWrite(ledPin, LOW);
		}

		inputString = "";
	}

	// Bottone Antirimbalzo
	int lettura = digitalRead(pulsantePin); // (int lettura = Variabile locale) leggo lo stato del pulsante
	if (lettura != ultimaLettura) {	        // se lo stato del pin è il contrario dell`ultima lettura
		ultimoTempoDebounce = millis();     // assegno il conteggio in millisecondi
	}

	if ((millis() - ultimoTempoDebounce) > attesaDebounce) { // se il conteggio è superiore a 50 millisecondi
		if (lettura != pulsanteStato and lettura == HIGH) { // se il pin7 è diverso dallo stato del pulsante e il pulsante è premuto
			ledStato = !ledStato;			// cambio lo stato del pin 13
			digitalWrite(ledPin, ledStato); // Imposto al pin13 lo stato di ledStato
		}
		pulsanteStato = lettura; // assegno lo stato del pulsante al pin7
	}

	ultimaLettura = lettura; // assegno l`ultima lettura allo stato del pulsante
	delay(10);				 // Ritardo 10 millis
}
