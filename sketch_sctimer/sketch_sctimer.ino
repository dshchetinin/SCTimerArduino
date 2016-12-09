#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10,  9,  8);
#define RED_LED 7
#define GREEN_LED 6
const int resetButton = 5;
const int leftHandButton = 4;
const int rightHandButton = 3;

int leftButtonState = 0;
int rightButtonState = 0;
int resetButtonState = 0;
int running = 0;
int count = 0;
unsigned long average = 0;
unsigned long oldTime = 0;

void(* resetFunc) (void) = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("*SCTimerArduino*");
  pinMode (RED_LED, OUTPUT);
  pinMode (GREEN_LED, OUTPUT);
}
     
void loop() {
  lcd.setCursor(4,0);
  
  leftButtonState = digitalRead(leftHandButton);
  rightButtonState = digitalRead(rightHandButton);
  resetButtonState = digitalRead(resetButton);
  
  unsigned long currentTime = millis();
  unsigned long now = (currentTime - oldTime);
  unsigned long minutes = (now/1000)/60;
  unsigned long seconds = (now/1000)%60;
  unsigned long milliseconds = now%1000;
  
  // Reset timer
  if (resetButtonState) {
    resetFunc();
  }
  
  if (running) {
    print_time(minutes, seconds, milliseconds);
    digitalWrite(RED_LED, HIGH);
    delay(15);
    digitalWrite(GREEN_LED, LOW);
    delay(15);
    digitalWrite(RED_LED, LOW);
    delay(15);
    digitalWrite(GREEN_LED, HIGH);
  }
  
  // Start-Stop timer
  if (leftButtonState && rightButtonState) {
    if (running) {
      count += 1;
      average = (average+now)/count;
      stop_timer(minutes, seconds, milliseconds);
      print_average(average);
      running = 0;
    } else {
      while(leftButtonState && rightButtonState) {
          digitalWrite(RED_LED, HIGH);
          delay(500);
          leftButtonState = digitalRead(leftHandButton); 
          rightButtonState = digitalRead(rightHandButton);
          lcd.setCursor(0,0);
          lcd.print("     Ready?     ");
          lcd.setCursor(0,1);
          lcd.print("                 ");
          digitalWrite(GREEN_LED, HIGH);  
      }
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      running = 1;
      unsigned long currentTime = millis();
      oldTime = currentTime;
    }
  }
}

void stop_timer(unsigned long minutes, unsigned long seconds, unsigned long milliseconds) {
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  clear_lcd(minutes, seconds, milliseconds);
  delay(1000);
  unsigned long currentTime = millis();
  oldTime = currentTime;
}

void clear_lcd(unsigned long minutes, unsigned long seconds, unsigned long milliseconds) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ScTime:");
  lcd.setCursor(7,0);
  print_time(minutes, seconds, milliseconds);
}

void print_average(unsigned long average) {
  unsigned long minutes = (average/1000)/60;
  unsigned long seconds = (average/1000)%60;
  unsigned long milliseconds = average%1000;
  lcd.setCursor(0,1);
  char lcd_buffer[16];
  sprintf(lcd_buffer, "AvTime:%02lu:%02lu:%03lu", minutes, seconds, milliseconds);
  lcd.print(lcd_buffer);
}

void print_time(unsigned long minutes, unsigned long seconds, unsigned long milliseconds) {
  char lcd_buffer[9];
  sprintf(lcd_buffer, "%02lu:%02lu:%03lu", minutes, seconds, milliseconds);
  lcd.print(lcd_buffer);
}
