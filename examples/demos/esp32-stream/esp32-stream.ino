// пример вывода стрима с камеры
// + управление фонариком

#define AP_SSID ""
#define AP_PASS ""

#define FLASH_PIN 4

#include "camera.h"
#include <CamStream.h>  // входит в GP::GyverPortal

#include <GyverPortal.h>
 GP::GyverPortal ui;

void build() {
  // ширина колонки 500 пикс
  GP::GP.BUILD_BEGIN( GP::DARK, 500);
  
  // вывести окно стрима шириной 500 пикс
  GP::GP.CAM_STREAM(500);
  
  GP::GP.LABEL("Flash:");
  GP::GP.SWITCH("flash");
  GP::GP.BUILD_END();
}

void action() {
  if (ui.click("flash")) digitalWrite(FLASH_PIN, ui.getBool());
}

void setup() {
  startup();

  // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  if (cam_init(FRAMESIZE_VGA)) Serial.println("Camera OK");
  else Serial.println("Camera Error");

  // начать стрим
  cam_stream_begin();

  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
}

void loop() {
  ui.tick();
}

void startup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  pinMode(FLASH_PIN, OUTPUT);
}
