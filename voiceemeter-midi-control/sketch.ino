#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 13, 2, 3, 4, 5);
int diff = 15;
int lastPotVal1 = 0;
int potVal1 = 0;
int currentChannel1 = 0;
int currentChannel2 = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
  pinMode(A2, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Channel: ");
  updateLcd(currentChannel1);
  lcd.setCursor(0,1);
  lcd.print("Volume: ");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(2));
  //Serial.println(digitalRead(4)); 
  //Serial.println("---------------");
  potVal1 = analogRead(A2);
  int potVal1diff = potVal1 - lastPotVal1;

  if (abs(potVal1diff) > diff) // execute only if new and old values differ enough
  {
    MIDImessage(0xB0, currentChannel1, map(potVal1, 0, 1023, 0, 127)); // map sensor range to MIDI range
    lastPotVal1 = potVal1; // reset old value with new reading
    lcd.setCursor(8,1);
    lcd.print(map(potVal1, 0, 1023, -60, 12));
    lcd.print("dB  ");
  }
  delay(10);

  if (digitalRead(8) == 1) {
    changeMidiChannel();
    updateLcd(currentChannel1);
    delay(200);
  }
  if (digitalRead(9) == 1) {
    MIDImessage(0xB0, currentChannel2, 1);
    delay(200);
  }
}

void MIDImessage(byte status, byte data1, byte data2) {
  Serial.write(status);
  Serial.write(data1);
  Serial.write(data2);
}

void changeMidiChannel() {
  if (currentChannel1 < 7) {
    currentChannel1++;
  } else {
    currentChannel1 = 0;
  }
  if (currentChannel2 < 15) {
    currentChannel2++;
  } else {
    currentChannel2 = 8;
  }
}

int updateLcd(int cc) {
  lcd.setCursor(9,0);
  String channelList[8] = {"Mic    ", "Games   ", "Spotify", "Chrome ", "Discord", "Other  ", "A1     ", "A2     "};
  lcd.print(channelList[cc]);
  return cc;
}