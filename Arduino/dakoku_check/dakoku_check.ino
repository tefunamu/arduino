// #include <SPI.h>
// #include <SdFat.h>
// #include <SdFatUtil.h>
// #include <SFEMP3Shield.h>

// SdFat sd;
// SFEMP3Shield MP3player;

int lightValue = 0;
int mode = 0;
int ledValue = 0;
int intervalTime = 0;

int ledPin = 13;
int lightPin = A0;
int lightGND = 18;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(lightGND, OUTPUT);

  digitalWrite(lightGND, HIGH);
  Serial.begin(9600);

  // sd.begin(SD_SEL, SPI_HALF_SPEED);
  // MP3player.begin();
}

// the loop function runs over and over again forever
void loop() {
  lightValue = analogRead(lightPin); // read pin's data

  Serial.println(lightValue);
  Serial.println(ledValue);
  // 暗さが950を超えたら再生モードに移る
  if (lightValue > 950 && mode == 0) {
    mode = 1;
    ledValue = 255;
   //  getWildandtough();
  }
  // 再生モード時、だんだんLEDを暗くする
  if (mode == 1) {
    analogWrite(ledPin, ledValue);
    if (ledValue > 0) {
      ledValue -= 10;
    } else {
      ledValue = 0;
    }
    intervalTime += 1;
  }
  delay(1000);
  // 再生開始から300秒経ったら通常モードに戻る
  if (intervalTime > 300) {
    intervalTime = 0;
    mode = 0;
  }
}
