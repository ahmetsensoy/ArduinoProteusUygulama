#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

String text;
String lcdSicaklik;
bool yak = false;
bool sondur = false;
bool yuruyen = false;
bool motor = false;
bool msj = false;
int ledler[7] = {4, 5, 6, 7, 8, 9, 10};
int sure = 500;
int mtr = 13;

SoftwareSerial virtualTerminal(2, 3);
void setup() {
  for (int i = 2; i < 11; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(mtr, OUTPUT);
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  virtualTerminal.begin(9600);
  digitalWrite(mtr, 1);
  int deger = analogRead(A0);
  double sicaklik = Termistor(deger);
  lcdSicaklik = String(sicaklik) + "°C";
  lcd.setCursor(0, 0);
  lcd.print(lcdSicaklik);
}

double Termistor(int analogOkuma) {

  double sicaklik;
  sicaklik = log(((10240000 / analogOkuma) - 10000));
  sicaklik = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik * sicaklik)) * sicaklik);
  sicaklik = sicaklik - 273.15;
  return sicaklik;
}

void loop() {

  while (Serial.available()) {
    delay(10);
    text = Serial.readString();
  }

  if (text.length() > 0)
  {
    Serial.println();


    if (yuruyen) {
      if (text.indexOf("basla") > -1) {
        Serial.println("Yuruyen Isik Basladi");
        virtualTerminal.println("Yuruyen Isik Basladi");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(lcdSicaklik);
        lcd.setCursor(0, 1);
        lcd.print("YuruyenI.Basladi");
        for (int i = 0; i < 7; i++) {
          digitalWrite(ledler[i], HIGH);
          delay(sure);
          digitalWrite(ledler[i], LOW);
        }

        for (int i = 5; i > 0; i--) {
          digitalWrite(ledler[i], HIGH);
          delay(sure);
          digitalWrite(ledler[i], LOW);
        }
        return;
      }
      else if (text.indexOf("dur") > -1) {
        for (int i = 0; i < 7; i++) {
          digitalWrite(ledler[i], LOW);
        }
        Serial.println("Yuruyen Isik Durdu");
        virtualTerminal.println("Yuruyen Isik Durdu");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(lcdSicaklik);
        lcd.setCursor(0, 1);
        lcd.print("Yuruyen I.Durdu");
        text = "";
        yuruyen = false;
      }
      else {
        text = "";
      }
    }

    if (motor) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      if (text.indexOf("calistir") > -1) {
        digitalWrite(mtr, 0);
        Serial.println("Motor Calisti");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(lcdSicaklik);
        lcd.setCursor(0, 1);
        lcd.print("Motor Calisti");
        virtualTerminal.println("Motor Calisti");

      }
      else if (text.indexOf("durdur") > -1) {
        digitalWrite(mtr, 1);
        Serial.println("Motor Duruyor");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(lcdSicaklik);
        lcd.setCursor(0, 1);
        lcd.print("Motor Duruyor");
        virtualTerminal.println("Motor Duruyor");
        text = "";
        motor = false;
      }
      else {
        text = "";
      }
    }
    if (msj) {
      Serial.println(text); // Hem Terminal hem de LCD
      virtualTerminal.println(text);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);
      lcd.print(text);
      text = "";
      msj = false;
    }


    if (text.indexOf("yuruyenisik") > -1)
    {
      yuruyen = true;
      yak = false;
      sondur = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);
      lcd.print("yuruyen isik");
      text = "";
    }
    else if (text.indexOf("motor") > -1)
    {
      motor = true;
      yak = false;
      sondur = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);
      lcd.print("motor");
      text = "";
    }
    else if (text.indexOf("mesaj") > -1)
    {
      msj = true;
      yak = false;
      sondur = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);
      lcd.print("mesaj");
      text = "";
    }
    else if (text.indexOf("sicaklik") > -1)
    {

      yak = false;
      sondur = false;
      int deger = analogRead(A0);
      double sicaklik = Termistor(deger);
      lcdSicaklik = String(sicaklik) + "°C";
      Serial.println(lcdSicaklik);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);
      lcd.print("sicaklik");

      virtualTerminal.println(String(sicaklik) + "°C");
      text = "";
    }

    else if (yak == true && text.indexOf("ledsondur") < 0)
    { int ledNo = text.toInt() - 1; // dizilerde indis 0 dan basliyor o yuzden -1 eklendi
      if (ledNo < 0 || ledNo > 6) {
        Serial.println("Lutfen 1 den 7 ye kadar olan bir rakam giriniz!");
        text = "";
        return;
      }

      digitalWrite(ledler[ledNo], HIGH);
      String txt = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);

      switch (ledNo) {
        case 0:
          txt = "1.Led yandi";
          break;
        case 1:
          txt = "2.Led yandi";
          break;
        case 2:
          txt = "3.Led yandi";
          break;
        case 3:
          txt = "4.Led yandi";
          break;
        case 4:
          txt = "5.Led yandi";
          break;
        case 5:
          txt = "6.Led yandi";
          break;
        case 6:
          txt = "7.Led yandi";
          break;
      }
      Serial.println(txt);
      virtualTerminal.println(txt);
      lcd.print(txt);
      text = "";
    }
    else if (sondur && text.indexOf("ledyak") < 0)
    { int ledNo = text.toInt() - 1;
      if (ledNo < 0 || ledNo > 6) {
        Serial.println("Lutfen 1 den 7 ye kadar olan bir rakam giriniz!");
        text = "";
        return;
      }

      digitalWrite(ledler[ledNo], LOW); // dizilerde indis 0 dan basliyor o yuzden -1 eklendi
      String txt = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);

      switch (ledNo) {
        case 0:
          txt = "1.Led sondu";
          break;
        case 1:
          txt = "2.Led sondu";
          break;
        case 2:
          txt = "3.Led sondu";
          break;
        case 3:
          txt = "4.Led sondu";
          break;
        case 4:
          txt = "5.Led sondu";
          break;
        case 5:
          txt = "6.Led sondu";
          break;
        case 6:
          txt = "7.Led sondu";
          break;
      }
      Serial.println(txt);
      virtualTerminal.println(txt);
      lcd.print(txt);
      text = "";

    }
    else if (text.indexOf("ledyak") > -1)
    {
      yak = true;
      sondur = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);
      lcd.print("Led Yak");
      text = "";
    }
    else if (text.indexOf("ledsondur") > -1)
    {
      sondur = true;
      yak = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lcdSicaklik);
      lcd.setCursor(0, 1);
      lcd.print("Led Sondur");
      text = "";
    }

    else {
      text = "";
    }
  }
}
