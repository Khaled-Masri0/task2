#include <LiquidCrystal.h>


 const int rs = 6;   
 const int en = 7;   
 const int d4 = 10;  
 const int d5 = 11;  
 const int d6 = 12;  
 const int d7 = 13;  


const int motorIn1 = 4;    
const int motorIn2 = 5;    
const int motorEnable = 9; 
const int potPin = A0;     


const int encoderC1 = 2; 
const int encoderC2 = 3; 


volatile int encoderTicks = 0; 
int lastEncoderStateC1 = LOW; 
unsigned long lastPrintTime = 0; 
 

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnable, OUTPUT);


  pinMode(encoderC1, INPUT_PULLUP);
  pinMode(encoderC2, INPUT_PULLUP);


  Serial.begin(9600);

 
  lcd.begin(16, 2);  
   lcd.print("Motor Controller");
   delay(2000); 
   lcd.clear();

 
  attachInterrupt(digitalPinToInterrupt(encoderC1), encoderISR, CHANGE);

 
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnable, 0);
  Serial.println("Motor is stopped.");
}

void loop() {
 
  int potValue = analogRead(potPin); // Read potentiometer value (0 to 1023)
  int motorSpeed = map(abs(potValue - 512), 0, 512, 0, 255); // Map to PWM range (0-255)

  
  if (potValue > 768) { 
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
    analogWrite(motorEnable, motorSpeed); 
    lcd.setCursor(0, 0);  
    lcd.print("Motor: Forward ");
    Serial.println("Motor spinning forward.");
  } else if (potValue < 256) { 
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);
    analogWrite(motorEnable, motorSpeed); 
    lcd.setCursor(0, 0);  
   lcd.print("Motor: Backward");
    Serial.println("Motor spinning backward.");
  } else { 
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    analogWrite(motorEnable, 0); 
    lcd.setCursor(0, 0);  
   lcd.print("Motor: Stopped ");
    Serial.println("Motor is stopped.");
  }

 
   lcd.setCursor(0, 1);  
   lcd.print("Encoder: ");
   lcd.print(encoderTicks);

 
  if (millis() - lastPrintTime >= 1000) {
    Serial.print("Encoder ticks: ");
    Serial.println(encoderTicks);
    encoderTicks = 0; 
    lastPrintTime = millis();
  }

  delay(100);
}


void encoderISR() {
  int c1State = digitalRead(encoderC1);
  int c2State = digitalRead(encoderC2);


  if (c1State != lastEncoderStateC1) {
    if (c2State != c1State) {
      encoderTicks++; 
    } else {
      encoderTicks--; 
    }
    lastEncoderStateC1 = c1State;
  }
}