#include <LiquidCrystal.h>

const int rs = 12, en = 10, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int ptm_pos = A0;
const int buttonPin = 8;

const int redPin = 13;
const int greenPin = 9;
const int bluePin = 6;
const int piezo = 7;


int ptm;
int theta;
int blueLevel;

int setTime = 0;
int timeLeft = 0;
bool isCounting = false;

unsigned long previousMillis = 0;
const long interval = 1000;

int lastPtm = -100;
int lastSetTime = -1;       
int lastButtonState = HIGH; 


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(piezo, OUTPUT);
//  lcd.print("test");
}

void loop() {
  // put your main code here, to run repeatedly:
  int currentButtonState = digitalRead(buttonPin);

  if (digitalRead(buttonPin) == LOW){
    delay(500);

    if (!isCounting && setTime > 0){
      isCounting = true;
      timeLeft = setTime*60;

      previousMillis = millis();

      setColor(0, 30, 0);

      tone(piezo, 1200, 100);

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

    int currentPtm = analogRead(ptm_pos);
    if (abs(currentPtm-lastPtm) > 4){
      lastPtm = currentPtm;
      blueLevel = map(currentPtm, 0, 1023, 156, 3);
      setTime = map(currentPtm,0, 1023, 60, 1);
      setColor(0, 0, blueLevel);

      if (setTime != lastSetTime){
        lcd.setCursor(0,0);
        lcd.print("Set Timer:");

        if (setTime < 10){
        lcd.print("0");
        }
        lcd.print(setTime);
        lcd.print(" min ");

        tone(piezo, 400, 10);

        lastSetTime = setTime;
      }
    }
  }

  if (isCounting){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      timeLeft--;

      int displayMins = timeLeft / 60;
      int displaySecs = timeLeft % 60;

      lcd.setCursor(0,0);
      lcd.print("Time Left: ");
      if (displayMins < 10) lcd.print("0");
      lcd.print(displayMins);
      lcd.print(":");
      if (displaySecs < 10) lcd.print("0");
      lcd.print(displaySecs);
      lcd.print(" ");

      if (timeLeft <= 0){
        isCounting = false;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Time Completed");
        setColor(255, 0, 0);


        tone(piezo, 600, 400); // Low tone
        delay(500);
        tone(piezo, 800, 400); // High tone
        delay(500);
        tone(piezo, 600, 400); // Low tone
        delay(500);
        tone(piezo, 800, 400); // High tone
        delay(500);

        lcd.clear();
        lastSetTime = -1;
      }
    }
  }
}

void setColor(int redValue, int blueValue, int greenValue){
  analogWrite(redPin, redValue);
  analogWrite(bluePin, blueValue);
  analogWrite(greenPin, greenValue);
}
