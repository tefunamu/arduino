#include <Time.h>
#include <TimeLib.h>

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal.h>
#include <Time.h>

// MACアドレス
byte mac[] = { 
  0x00, 0x50, 0xc2, 0x97, 0x22, 0xc3 };

// UDPローカルポート番号
unsigned int localPort = 8888;

// NTPタイムサーバIPアドレス(ntp.nict.jp NTP server)
IPAddress timeServer(133, 243, 238, 164);

// NTPパケットバッファサイズ
const int NTP_PACKET_SIZE= 48;

// NTP送受信用パケットバッファ
byte packetBuffer[NTP_PACKET_SIZE];

// Udpクラス
EthernetUDP Udp;

// 最後にパケットを送信した時間（ミリ秒）
unsigned long lastSendPacketTime = 0;

// キャラクタLCDクラス（RS=>7, E=>6, D4=>5, D5=>4, D4=>3, D3=>2)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Attempting to obtain a DHCP lease...");

  lcd.begin(20, 4);
  lcd.print("Starts sync...");

  if ( Ethernet.begin(mac) == 0 ) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }

  Serial.println("A DHCP lease has been obtained.");

  Serial.print("My IP address is ");
  Serial.println(Ethernet.localIP());

  Serial.print("Gateway IP address is ");
  Serial.println(Ethernet.gatewayIP());

  Serial.print("DNS IP address is ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.println();

  Udp.begin(localPort);

  // 最初の時刻リクエストを送信
  sendNTPpacket(timeServer);
  lastSendPacketTime = millis();
}

void loop()
{
  if ( millis() - lastSendPacketTime > 180000 ){
    // NTPサーバへ時刻リクエストを送信
    sendNTPpacket(timeServer);
    // 時間を更新
    lastSendPacketTime = millis();
  }

  // NTPサーバからのパケット受信
  if ( Udp.parsePacket() ) {
    // バッファに受信データを読み込む
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    // 時刻情報はパケットの40バイト目からはじまる4バイトのデータ
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

    // NTPタイムスタンプは64ビットの符号無し固定小数点数（整数部32ビット、小数部32ビット）
    // 1900年1月1日0時との相対的な差を秒単位で表している
    // 小数部は切り捨てて、秒を求めている
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // NTPタイムスタンプをUNIXタイムに変換する
    // UNITタイムは1970年1月1日0時からはじまる
    // 1900年から1970年の70年を秒で表すと2208988800秒になる
    const unsigned long seventyYears = 2208988800UL;
    // NTPタイムスタンプから70年分の秒を引くとUNIXタイムが得られる
    unsigned long epoch = secsSince1900 - seventyYears; 
    Serial.print("Unix time = ");
    Serial.println(epoch);

    // Timeライブラリに時間を設定(UNIXタイム)
    // 日本標準時にあわせるために＋9時間しておく
    setTime(epoch + (9 * 60 * 60));

    Serial.print("JST is ");
    Serial.print(year());
    Serial.print('/');
    Serial.print(month());
    Serial.print('/');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(hour());
    Serial.print(':'); 
    Serial.print(minute());
    Serial.print(':'); 
    Serial.println(second());
    Serial.println();

    // LCDクリア
    lcd.clear();
    // 年/月/日を表示
    lcd.print(year());
    lcd.print('/');
    lcd.print(month());
    lcd.print('/');
    lcd.print(day());
    // 2行目３文字目にカーソルを移動
    lcd.setCursor(2, 1);
    // 時:分:秒を表示
    lcd.print(hour());
    lcd.print(':'); 
    lcd.print(minute());
    lcd.print(':'); 
    lcd.print(second());
  }
}

// send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  // NTP requests are to port 123
  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
