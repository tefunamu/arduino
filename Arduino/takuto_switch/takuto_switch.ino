#define LED 13
#define SW  2

void setup() {
  // put your setup code here, to run once:
  pinMode(SW, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(SW) == HIGH){
    digitalWrite(LED, LOW);
  }else{
    digitalWrite(LED, HIGH);
  }
}
