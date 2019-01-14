const int sensorMin = 0;     
const int sensorMax = 1024;  


#include <LiquidCrystal.h>;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
  lcd.begin(20, 4);
}
void loop() {
	int sensorReading = analogRead(A0);
	int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

  lcd.setCursor(0,3);
  
  switch (range) {
  case 0:    
    lcd.clear();
    lcd.setCursor(0,2);
    lcd.print("Diluvia");
    break;
 case 1:    
    lcd.clear();
    lcd.setCursor(0,2);
    lcd.print("Piove");
    break;
 case 2:    
    lcd.clear();
    lcd.setCursor(0,2);
    lcd.print("Non piove");
    break;
  }
  delay(1000); 
}
