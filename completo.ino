#include <dht.h>


#include <LiquidCrystal.h>;
#include <dht.h>;
dht DHT;


const int sensorMin = 0;     
const int sensorMax = 1024;  

#define DHT11_PIN 8

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
   
  lcd.begin(20, 4);

}

void loop() {
  lcd.clear();
  int sensorReading = analogRead(A0);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0);
  lcd.print("Temperatura = ");
  lcd.print(DHT.temperature);
  lcd.setCursor(0,1);
  lcd.print("Umidita' = ");
  lcd.print(DHT.humidity);
  lcd.print("%");

  switch (range) {
    case 0:    
      lcd.setCursor(0,2);
      lcd.print("Diluvia");
      break;
   case 1:    
      lcd.setCursor(0,2);
      lcd.print("Piove");
      break;
   case 2:    
      lcd.setCursor(0,2);
      lcd.print("Non piove");
      break;
    }
  
  delay(2500);
}
