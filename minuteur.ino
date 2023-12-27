#include <LiquidCrystal.h>

#define TIME 100

#define LED A0
#define BUZZER A2


#define BPLUS 3
#define BFUNC A3
#define BMOINS A1

const int rs = 10, en = 9, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

static int time_button_ms = 0;
static int time_button_s = 0;
static int time_button_choose = 0;

static int time_timer = 0;

static int last_time_button_ms = 0;
static int last_time_button_s = 0;
static int last_time_button_choose = 0;

static uint16_t flag_begin = 0;

static uint16_t minutes = 0;
static uint16_t secondes = 0;

static bool mode = false;  // false = settings; true = counter
#define SETTINGS false
#define COUNTER true



void shut_led() {
  digitalWrite(LED, LOW);
}

void light_led() {
  digitalWrite(LED, HIGH);
}

void use_led() {
  light_led();
  delay(100);
  shut_led();
}

void up_buzzer() {
  digitalWrite(BUZZER, HIGH);
}
void off_buzzer() {
  digitalWrite(BUZZER, LOW);
}
void use_buzzer() {
  up_buzzer();
  delay(10);
  off_buzzer();
}
void alarm() {
  for (int i = 0; i < 5; i++) {
    use_buzzer();
    delay(300);
    use_buzzer();
    delay(300);
    use_buzzer();
    delay(300);
    use_buzzer();
    delay(300);
    use_buzzer();
    delay(1000);
  }
}

String padStart(const String& s, unsigned int totalLength, char pad) {
  if (totalLength <= s.length()) return s;
  unsigned int paddingLength = totalLength - s.length();

  String result;
  result.reserve(totalLength);
  while (paddingLength--) {
    result += pad;
  }
  result += s;
  return result;
}

void display_time() {
  lcd.clear();
  lcd.print(padStart(String(minutes), 2, '0') + "min:" + padStart(String(secondes), 2, '0') + "sec");
}



void setup() {
  Serial.begin(9600);
  // Initialisation des boutons
  pinMode(BPLUS, INPUT_PULLUP);
  pinMode(BFUNC, INPUT_PULLUP);
  pinMode(BMOINS, INPUT_PULLUP);

  // Initilisation de la LED
  pinMode(LED, OUTPUT);

  // Initialisation du buzzer
  pinMode(BUZZER, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  display_time();

  // Vérification sonore et visuelle que tout marche
  light_led();
  use_buzzer();
}

uint8_t delay_between_pression = 10;
void loop() {
  if (mode == SETTINGS) {
    if (digitalRead(BPLUS) == LOW) {
      minutes++;
      if (minutes > 59) {
        minutes = 59;
      }
      display_time();
      use_led();
      use_buzzer();
      delay(delay_between_pression);
    } else if (digitalRead(BFUNC) == LOW) {
      mode = COUNTER;
      delay(delay_between_pression);
    } else if (digitalRead(BMOINS) == LOW) {
      if (minutes > 0) {
        minutes--;
      }
      display_time();
      use_led();
      use_buzzer();
      delay(delay_between_pression);
    }
  } else {
    if (digitalRead(BFUNC) == LOW && secondes != 0) {
      mode = SETTINGS;
      secondes = 0;
      minutes = 1;
      delay(delay_between_pression);
    } else {
      if (secondes < 1 && minutes < 1) {
        alarm();
        mode = SETTINGS;

      } else if (secondes < 1) {
        minutes--;
        secondes = 59;
      } else {
        secondes--;
      }
      display_time();
      delay(1000);
    }
  }

}
