int cnt = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  cnt++;
  Serial.print(cnt,DEC);
  Serial.print("sec.elapsed\n");
  delay(1000);
}
