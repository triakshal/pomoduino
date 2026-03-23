#include <LiquidCrystal.h>

const int rs = 12, en = 10, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int ptm_pos = A0;
const int buttonPin = 8;


int ptm;
int theta;

int setTime = 0;
int timeLeft = 0;
bool isCounting = false;

unsigned long previousMillis = 0;
const long interval = 1000;

int lastSetTime = -1;       
int lastButtonState = HIGH; 


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);

  pinMode(buttonPin, INPUT_PULLUP);
//  lcd.print("test");
}

void loop() {
  // put your main code here, to run repeatedly:
  int currentButtonState = digitalRead(buttonPin);

  if (digitalRead(buttonPin) == LOW){
    delay(500);

    if (!isCounting && setTime > 0){
      isCounting = true;
      timeLeft = setTime;

      previousMillis = millis();

      lcd.clear();
    }
      else if (isCounting){
      isCounting = false;
      lcd.clear();
      lastSetTime = -1;
    }
  }

lastButtonState = currentButtonState;

  if (!isCounting){
    ptm = analogRead(ptm_pos);
    setTime = map(ptm,0,1023,100,1);

    if (setTime != lastSetTime){
      lcd.setCursor(0,0);
      lcd.print("Set Timer: ");
      lcd.print(setTime);
      lcd.print(" ");

      lastSetTime = setTime;
    }
  }

  if (isCounting){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      timeLeft--;


      lcd.setCursor(0,0);
      lcd.print("Time Left: ");
      lcd.print(timeLeft);
      lcd.print(" ");

      if (timeLeft <= 0){
        isCounting = false;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Time Completed");
        delay(2000);
        lcd.clear();
        lastSetTime = -1;
      }
    }
  }
}

