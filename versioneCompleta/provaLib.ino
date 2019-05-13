#include <dht.h>
#include "BMP280.h"
#define P0 1013.25 // pressione standard, a livello del mare
#include <LiquidCrystal.h>;

//dati DHT
dht DHT;

//dati BMP
BMP280 bmp;
double T = 0;
double P = 0;
double A = 0; 
char risultatomisura = 0; 
int correttore = 0;
int alt = 0;

//dati sensore di pioggia
int rangePioggia = 0;

//dati per la temporizzazione
unsigned long time;
unsigned long last_time_b;
unsigned long last_time_s;


//dati per il bottone
int statoButon = LOW;
int statoSchermo = HIGH;

//inizializzazione LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//valore fotoresistore
const int analogInPin = A1;

//metodo per sapere quanto piove
long Map(long x, long in_min, long in_max, long out_min, long out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//invio dati
String stringaInvio="";


void setup() {
  //impostazione modalita' dei pin(mancano quelli dello schermo)
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(8,INPUT);
  pinMode(7,INPUT);
  pinMode(9,OUTPUT);
  //accensione dello schermo
  digitalWrite(9,HIGH);
  lcd.begin(20, 4);
  //accensione BMP
  bmp.begin();
  bmp.setOversampling(4);

  last_time_b = 0;
  last_time_s = 0;

  Serial.begin(115200);
}

//motodo che legge le misure dai sensori
void leggiMisure(){
  //lettura DHT
  int chk = DHT.read11(8);

  //lettura sensore di pioggia
  int sensorReading = analogRead(A0);
  rangePioggia = Map(sensorReading, 0, 1023, 0, 2);

  //lettora BMP
  risultatomisura = bmp.startMeasurment();
  if (risultatomisura != 0){
   risultatomisura = bmp.getTemperatureAndPressure(T, P);
   if (risultatomisura != 0){
     A = bmp.altitude(P, P0); 
     alt = A + correttore;
   }
  } 
}

//metodo che stampa le misure sullo schermo
void stampa(){
    //stampa temperatura da BMP
    lcd.setCursor(0,0);
    lcd.print("Temperatura: ");
    lcd.print(T);
    //Serial.println(T, 2); 
    stringaInvio=stringaInvio+(T)+";";

    stringaInvio=stringaInvio+(analogRead(analogInPin))+";";

    //stampa umidità da DHT
    lcd.setCursor(0,1);
    lcd.print("Umidita': ");
    lcd.print(DHT.humidity);
    lcd.print("%");
    //Serial.println(DHT.humidity, 2);
    stringaInvio=stringaInvio+(DHT.humidity)+";";


     //stampa pressione da BMP
    lcd.setCursor(9,2);
    lcd.print("Pres:");
    lcd.print (P);
    //Serial.println(P, 2);
    stringaInvio=stringaInvio+(P)+";";

    //stampa altituidine da BMP
    lcd.setCursor(0,2);    
    lcd.print ("Alt:");
    lcd.print (alt);
    lcd.print("m");
    //Serial.println(alt, 1);

   

    //stampa stato della pioggia
     switch (rangePioggia) {
       case 0:    
          lcd.setCursor(0,3);
          lcd.print("Diluvia");
          stringaInvio=stringaInvio+("2")+";";
          break;
       case 1:    
          lcd.setCursor(0,3);
          lcd.print("Piove");
          stringaInvio=stringaInvio+("1")+";";
          break;
       case 2:    
          lcd.setCursor(0,3);
          lcd.print("Non piove");
          stringaInvio=stringaInvio+("0")+";";
          break;
     }
      lcd.setCursor(11,3);
      lcd.print(analogRead(analogInPin));

    
     Serial.println(stringaInvio);
      
     stringaInvio="";
}


void loop() {

  time = millis();

  int lettura = digitalRead(7);
  if(time > last_time_b + 10){
    if(lettura==HIGH && lettura!= statoButon){
      statoSchermo = !statoSchermo;
      digitalWrite(9,statoSchermo);
    }
    statoButon = lettura;
    last_time_b = time;
  }

  if(time > last_time_s + 5000 || last_time_s == 0){
    leggiMisure();
    lcd.clear();
    stampa();
    last_time_s = time;
  }

}
