#include <DHT.h>

#include "DHT.h"

#define LED_LOUNDLY 13
#define LED_DETERGENT 12
#define LED_WASH 4
#define POWER_LOUNDLY 11
#define POWER_DETERGENT 5
#define DHTPIN A0
#define DHTTYPE DHT11 
#define FAN 2

int cnt = 0;
DHT dht(DHTPIN, DHTTYPE);

int cnt_debug = 0;
int cntLED = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode( POWER_LOUNDLY, INPUT);
  pinMode(LED_LOUNDLY, OUTPUT);
  pinMode(POWER_DETERGENT, INPUT);
  pinMode(LED_DETERGENT, OUTPUT);
  pinMode(LED_WASH, OUTPUT);
  pinMode(FAN, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if( isnan(t) || isnan(h) ){
    Serial.println("Failed to read from DHT");
  }else{
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t ");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
  }
/* debug */
/*
   if(digitalRead(POWER_LOUNDLY) == HIGH){
    Serial.println(analogRead(POWER_LOUNDLY), DEC);
    digitalWrite(LED_LOUNDLY, LOW);
    delay(1000);
    
   }else{
    Serial.println("NO POWER");
    digitalWrite(LED_LOUNDLY, HIGH);
    delay(1000);
   }
*/
  while(1){
  // debug
  //if(cnt_debug > 100){
    delay(10000);
     digitalWrite(LED_LOUNDLY, HIGH);
     digitalWrite(LED_DETERGENT, HIGH);
     break;
  //}
    
  if( (digitalRead(POWER_LOUNDLY) == HIGH) && (digitalRead(POWER_DETERGENT) == HIGH) ){
    Serial.println("Loop 1");
    
    digitalWrite(LED_LOUNDLY, HIGH);
    Serial.println("Amount of clothes");
    Serial.println(analogRead(POWER_LOUNDLY), DEC);

    
    digitalWrite(LED_DETERGENT, HIGH);
    Serial.println("Amount of detergent");
    Serial.println(analogRead(POWER_DETERGENT), DEC);

    delay(1000);

    break;
    
  }else if( (digitalRead(POWER_LOUNDLY) == LOW) && (digitalRead(POWER_DETERGENT) == HIGH ) ){
    Serial.println("Loop 2");
    
    digitalWrite(LED_LOUNDLY, LOW);
    

    digitalWrite(LED_DETERGENT, HIGH);
    Serial.println("Amount of detergent");
    Serial.println(analogRead(POWER_DETERGENT), DEC);

    delay(1000);
    
  }else if( (digitalRead(POWER_LOUNDLY) == HIGH) && (digitalRead(POWER_DETERGENT) == LOW ) ){
    Serial.println("Loop 3");

    digitalWrite(LED_LOUNDLY, HIGH);
    Serial.println("Amount of detergent");
    Serial.println(analogRead(POWER_LOUNDLY), DEC);

    digitalWrite(LED_DETERGENT, LOW);

    delay(1000);
    
  }else if( (digitalRead(POWER_LOUNDLY) == LOW) && (digitalRead(POWER_DETERGENT) == LOW ) ){
    Serial.println("Loop 4");

    digitalWrite(LED_LOUNDLY, LOW);
    digitalWrite(LED_DETERGENT, LOW);

    delay(1000);
    
  }
    cnt_debug++;
  }

  // 洗濯開始
  delay(1000
  );
  digitalWrite(LED_WASH, HIGH);
  delay(10000);


  // すすぎ
  while(cntLED <= 5){

    cntLED++;
  
  digitalWrite(LED_LOUNDLY, HIGH);
  digitalWrite(LED_DETERGENT, HIGH);
  digitalWrite(LED_WASH, HIGH);

    delay(1000);

  digitalWrite(LED_LOUNDLY, LOW);
  digitalWrite(LED_DETERGENT, LOW);
  digitalWrite(LED_WASH, LOW);

  delay(1000);
  
  }

  cnt_debug = 0;
  // 乾燥
  while(1){
  if(h > 35){
    Serial.println("start FAN");
    digitalWrite(FAN, HIGH);
    delay(5000);
    cnt_debug++;

    }
  }

  // end
  while(1){
    digitalWrite(LED_LOUNDLY, LOW);
  digitalWrite(LED_DETERGENT, LOW);
  digitalWrite(LED_WASH, LOW);
  delay(1000);
  }
  
}
