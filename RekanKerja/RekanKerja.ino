#include "esp_camera.h"
#include "mario.h"
#include "fd_forward.h"
#include "BluetoothSerial.h"
#include <base64.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();
#include <SPI.h>


BluetoothSerial SerialBT;
int toggle = 0;
int readPir = 0;
int idPengirim;
int pesanTampil = 0;
int toggleGambar = 0;
int PIR = 0;
int vibrate = 16;
int buzz = 4;
String pesanTerima ="";
String pengirim = "";
int waktu = 5;
int buzzer = 0;
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
int faceState = 0;

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
 
bool initCamera() {
 
  camera_config_t config;
 
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;
 
  esp_err_t result = esp_camera_init(&config);
 
  if (result != ESP_OK) {
    return false;
  }
 
  return true;
}
 
mtmn_config_t mtmn_config = {0};
int detections = 0;

void initBT(){
  if(!SerialBT.begin("camesp")){
    Serial.println("An error occurred initializing Bluetooth");
    ESP.restart();
  }else{
    Serial.println("Bluetooth initialized");
  }
}


void notifOn(){
  digitalWrite(buzz,HIGH);
  digitalWrite(vibrate,HIGH);
}

void notifOff(){
  digitalWrite(buzz,LOW);
  digitalWrite(vibrate,LOW);
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
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


void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);
  testdrawtext("Preparing Teman Kerja...",TFT_BLACK);
  delay(2000);
  if (!initCamera()) {
 
    Serial.printf("Failed to initialize camera...");
    return;
  }
  pinMode(PIR,INPUT);
  pinMode(vibrate,OUTPUT);
  pinMode(buzz, OUTPUT);
  initBT();
 
  mtmn_config = mtmn_init_config();
  tft.fillScreen(TFT_WHITE);
}

void capture(){
  camera_fb_t * frame;
  frame = esp_camera_fb_get();
 
  dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, frame->width, frame->height, 3);
  fmt2rgb888(frame->buf, frame->len, frame->format, image_matrix->item);
 
  esp_camera_fb_return(frame);
 
  box_array_t *boxes = face_detect(image_matrix, &mtmn_config);
 
  if (boxes != NULL) {
      faceState = 1;
   }
   else{
        faceState = 0;
   }
 
  dl_matrix3du_free(image_matrix);

}
 
void loop() {
  readPir = digitalRead(PIR);
  lnow = millis();
  if(lnow - last >= interval){
    last = lnow;
    capture();
    String Kirim = "";
    Kirim += "{'type':1,'motion':'";
    Kirim += readPir; 
    Kirim += "','image':";
    Kirim += faceState;
    Kirim += "}";
    SerialBT.print(Kirim);
    Serial.println(Kirim);
  }
  
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
  Serial.println(nowbuzz - lastbuzz);
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
     }
     else if(toggleGambar == 1){
      tft.pushImage(35, 15,animation_width  , animation_height, mario[0]);
      toggleGambar = 2;
     }
     else{
      tft.pushImage(35, 15,animation_width  , animation_height, mario[1]);
      toggleGambar = 1;
     }
   }
 }
}
