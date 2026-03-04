#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG 10
#define ECHO 11

LiquidCrystal_I2C lcd(0x27,16,2);

// Filter parameters
const int filterSize = 5;
float readings[filterSize];
int index = 0;
float total = 0;
float filteredDistance = 0;

void setup()
{
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  for(int i=0;i<filterSize;i++)
  readings[i] = 0;
}

void loop()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);

  float distance = duration * 0.034 / 2;

  // ----- Moving Average Filter -----
  total = total - readings[index];
  readings[index] = distance;
  total = total + readings[index];
  index = (index + 1) % filterSize;

  filteredDistance = total / filterSize;

  // ----- Display -----
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance:");

  lcd.setCursor(0,1);
  lcd.print(filteredDistance);
  lcd.print(" cm");

  Serial.print("Filtered Distance: ");
  Serial.println(filteredDistance);

  delay(500);
}