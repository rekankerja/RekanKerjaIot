#include <SPI.h>
#include <TFT_eSPI.h>
#include "kny.h"
#include "BluetoothSerial.h"
#include <ArduinoJson.h>


TFT_eSPI tft = TFT_eSPI();
BluetoothSerial SerialBT;

int idPengirim;
int pesanTampil = 0;
String pesanTerima ="";
String pengirim = "";
int waktu = 5;
int buzzer = 0;
int toggle = 0;
int readPir = 0;
int toggleGambar = 0;


int PIR = 37;
int buzz = 38;


unsigned long intervalBuzz = 1000;
unsigned long intervalBeep = 300;
unsigned long interval = 5000;
unsigned lastImage= millis();
unsigned nowImage;
unsigned lastBeep;
unsigned nowbeep;
unsigned lastbuzz = millis();
unsigned nowbuzz;
unsigned last = millis();
unsigned lnow;




void initBT(){
  if(!SerialBT.begin("camesp")){
    Serial.println("An error occurred initializing Bluetooth");
    ESP.restart();
  }else{
    Serial.println("Bluetooth initialized");
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.fillRect(0,0,240,10,TFT_WHITE);
  tft.setCursor(80, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void writepesan(char *pengirim, char *pesan, uint16_t color) {
  tft.fillScreen(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print("Pengirim : ");
  tft.setCursor(0,10);
  tft.print(pengirim);
  tft.setCursor(0, 20);
  tft.print("Pesan : ");
  tft.setCursor(0,30);
  tft.print(pesan);
}


void notifOn(){
  digitalWrite(buzz,HIGH);
}

void notifOff(){
  digitalWrite(buzz,LOW);
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
//    Serial.println("connected");
    testdrawtext("Connected ",TFT_BLACK);
  }
  if(event == ESP_SPP_CLOSE_EVT){
//    Serial.println("disconnected");
    testdrawtext("disconnected ",TFT_BLACK);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIR,INPUT);
  pinMode(buzz, OUTPUT);
  tft.init();
  tft.setRotation(1);
  
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE);
  testdrawtext("Preparing Teman Kerja...",TFT_BLACK);
//  tft.pushImage(0, 10,animation_width  , animation_height, kny[0]);
  delay(2000);
  testdrawtext("disconnected ",TFT_BLACK);
  initBT();
  SerialBT.register_callback(callback);
}



void loop() {
  // put your main code here, to run repeatedly:
//readPir = digitalRead(PIR);
readPir = 0;
  lnow = millis();
  if(lnow - last >= interval){
//     tft.fillScreen(TFT_WHITE);
    last = lnow;
    String Kirim = "";
    Kirim += "{'type':1,'motion':'";
    Kirim += readPir; 
    Kirim += "','image':";
    Kirim += '0';
    Kirim += "}";
    SerialBT.print(Kirim);
    Serial.println(Kirim);
  }
//
  if (SerialBT.available()) 
  {
    StaticJsonDocument<600> doc;
    DeserializationError err = deserializeJson(doc, SerialBT);
    if (err == DeserializationError::Ok) 
    {
      delay(20);
      lastBeep = millis();
      idPengirim = doc["idPesanPenerima"].as<int>();
      buzzer = doc["buzzer"].as<int>();
      pengirim = doc["namaPengirim"].as<String>();
      pesanTerima  = doc["pesan"].as<String>();
      waktu = doc["waktu"].as<int>();
      lastbuzz = millis();
      Serial.println(lastbuzz);
      Serial.println(idPengirim);
      Serial.println(pengirim);
      Serial.println(pesanTerima);
      Serial.println(waktu);
      String dataBalas = "{'type':0,'id':'";
      dataBalas += idPengirim;
      dataBalas += "','sukses':1}";
      SerialBT.print(dataBalas);
      int pengirimLen = pengirim.length()+1;
      int pesanTerimaLen = pesanTerima.length()+1;
      char lcdPengirim[pengirimLen];
      char lcdPesan[pesanTerimaLen];
      pengirim.toCharArray(lcdPengirim,pengirimLen);
      pesanTerima.toCharArray(lcdPesan,pesanTerimaLen);
      writepesan(lcdPengirim,lcdPesan,TFT_BLACK);
      pesanTampil = 1;
      toggleGambar = 0;
    } 
    else 
    {
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      while (SerialBT.available() > 0)
        SerialBT.read();
    }
  }
  nowbuzz = millis();
  if(buzzer == 1 && ((nowbuzz - lastbuzz) <= (waktu*intervalBuzz))){
    Serial.println("masuk buz");
    nowbeep = millis();
    if (nowbeep - lastBeep >= intervalBeep){
      Serial.println("masuk beep");
      lastBeep = nowbeep;
      if (toggle == 1){
        notifOn();
        toggle = 0;
      }
      else{
        notifOff();
        toggle = 1;
      }
    }
  }
  else{
    buzzer = 0;
    notifOff();
    toggle=1;
  }
  nowImage = millis();
 if(pesanTampil == 1 && ((nowbuzz - lastbuzz) <= (60000)) ){
   Serial.println("pesan");
 }
  else{
   
   if(nowImage - lastImage >= 500){
    lastImage = nowImage;
     if(toggleGambar == 0){
      tft.fillScreen(TFT_WHITE);
      toggleGambar = 1;
//      Serial.println("masuk putih");
     }
     else if(toggleGambar == 1){
     tft.pushImage(0, 10,animation_width  , animation_height, kny[0]);
      toggleGambar = 2;
//      Serial.println("masuk gambar");
     }
     else{
      tft.pushImage(0, 10,animation_width  , animation_height, kny[1]);
      toggleGambar = 1;
//      Serial.println("masuk gamba2");
     }
   }
 }
 
}
