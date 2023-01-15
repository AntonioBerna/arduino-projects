#include <RtttlPlayer.h>
#include <LiquidCrystal_I2C.h>

#define ENABLE_PIN 3
#define CHANGE_PIN 2
#define DEBOUNCE_DELAY 50
#define LED_DELAY 200
#define BUZZER_PIN 4
#define BACKLIGHT_PIN 5

int enable_state = LOW;
int enable_last_reading = LOW;
unsigned long enable_timestamp = 0;
bool enable = false;
int change_state = LOW;
int change_last_reading = LOW;
unsigned long change_timestamp = 0;
char *songs[] = {
	"We Wish You a |Merry Christmas:d=4,o=5,b=170:d,g,8g,8a,8g,8f#,e,e,e,a,8a,8b,8a,8g,f#,d,d,b,8b,8c6,8b,8a,g,e,d,e,a,f#,2g,d,g,8g,8a,8g,8f#,e,e,e,a,8a,8b,8a,8g,f#,d,d,b,8b,8c6,8b,8a,g,e,d,e,a,f#,1g,d,g,g,g,2f#,f#,g,f#,e,2d,a,b,8a,8a,8g,8g,d6,d,d,e,a,f#,2g,1p",
  	"Jingle Bells:d=4,o=5,b=170:b,b,b,p,b,b,b,p,b,d6,2g.,8a,2b.,8p,c6,c6,c6.,8c6,c6,b,b,8b,8b,b,a,a,b,2a,2d6,1p",
  	"Silent Night:d=4,o=4,b=125:f#.,8g#,f#,d#.,p.,f#.,8g#,f#,d#.,p.,2c#6,c#6,a#.,p.,2b,b,f#.,p.,2g#,g#,b.,8a#,g#,f#.,8g#,f#,d#.,p.,2g#,g#,b.,8a#,g#,f#.,8g#,f#,d#.,p.,2c#6,c#6,e.6,8c#6,a#,2b.,2d#6,p.,b,f#,d#,f#.,8e,c#,b.4,1p",
  	"Let it Snow:d=4,o=5,b=125:8c,8c,8c6,8c6,a#,a,g,f,2c,8c,16c,g.,8f,g.,8f,e,2c,d,8d6,8d6,c6,a#,a,2g.,8e.6,16d6,c6,8c.6,16a#,a,8a#.,16a,2f.,c,8c6,8c6,a#,a,g,f,2c,8c.,16c,g.,8f,g.,8f,e,2c,d,8d6,8d6,c6,a#,a,2g.,8e.6,16d6,c6,8c.6,16a#,a,8a.,16g,2f.1p"
};
int songs_number = sizeof(songs) / sizeof(char *);
int song_index = 0;
bool song_changed = true;
int led[] = {12, 11, 10, 9, 8};
int led_number = sizeof(led) / sizeof(int);
unsigned long led_timestamp = 0;

RtttlPlayer player(BUZZER_PIN);
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup() {
	pinMode(ENABLE_PIN, INPUT);
	pinMode(CHANGE_PIN, INPUT);

	for (int i = 0; i < led_number; i++) {
		pinMode(led[i], OUTPUT);
	}
	randomSeed(analogRead(A0));
	lcd.init();
	lcd.backlight();
	lcd.clear();
	pinMode(BACKLIGHT_PIN, OUTPUT);
}

void loop() {
  	int enable_input = digitalRead(ENABLE_PIN);
  
  	if (enable_input != enable_last_reading) {
    	enable_timestamp = millis();
  	}

  	if ( (millis() - enable_timestamp) > DEBOUNCE_DELAY ) {
    	if (enable_input != enable_state && enable_input == HIGH) {
      		enable = !enable;
      		song_index = 0;
      		song_changed = true;
      		for (int i = 0; i < led_number; i++) {
        		digitalWrite(led[i], LOW);
      		}
      		lcd.clear();
      		analogWrite(BACKLIGHT_PIN, 0);
    	}
    	enable_state = enable_input;
  	}
  	enable_last_reading = enable_input;

  	if (!enable) {
    	return;
  	}

  	int change_input = digitalRead(CHANGE_PIN);
  	if (change_input != change_last_reading){
    	change_timestamp = millis();
  	}
  	
	if ( (millis() - change_timestamp) > DEBOUNCE_DELAY ) {
    	if (change_input != change_state && change_input == HIGH) {
      		song_index = (song_index + 1) % songs_number;
      		song_changed = true;
    	}
    	change_state = change_input;
  	}
  	change_last_reading = change_input;

  	if ( (millis() - led_timestamp) > LED_DELAY) {
		int random_number = random(0, 31);
    	for (int i = 0; i < led_number; i++) {
    		digitalWrite(led[i], random_number & 1);
      		random_number >>= 1;
    	}
    	led_timestamp = millis();
  	}

  	if (song_changed) {
    	player.play(songs[song_index]);
    	song_changed = false;
    	lcd.clear();
    	analogWrite(BACKLIGHT_PIN, 10);
    	String song_title = player.getSongTitle();
    	int song_title_length = song_title.length();
    	int row = 0;
    	int column = 0;
    	
		for (int i = 0; i < song_title_length; i++) {
      		if (song_title[i] != '|') {
        		lcd.setCursor(column, row);
        		lcd.print(song_title[i]);
        		column++;
      		} else {
        		row++;
        		column = 0;
      		}
    	}
  	} else {
    	if (player.isPlaying()) {
      		player.update();
    	} else {
      		song_index = (song_index + 1) % songs_number;
      		song_changed = true;
    	}
  	}
  	delay(1);
}
