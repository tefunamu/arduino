#define LED  13
#define Cds A0

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(analogRead(Cds) > 800){
    digitalWrite(LED, LOW);
  }else{
    digitalWrite(LED, HIGH);
  }
}
