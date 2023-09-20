// кастомные кнопки OTA прошивки

#define AP_SSID ""
#define AP_PASS ""

// подключить библиотеку файловой системы (до #include GP::GyverPortal)
#include <LittleFS.h>

#include <GyverPortal.h>
 GP::GyverPortal ui(&LittleFS);  // передать ссылку на fs (SPIFFS/LittleFS)

// конструктор страницы
void build() {
  GP::GP.BUILD_BEGIN();
  GP::GP.THEME( GP::DARK);

  GP::GP.LABEL("v1");

  GP::GP.OTA_FIRMWARE();
  GP::GP.OTA_FILESYSTEM();

  GP::GP.BUILD_END();
}

void setup() {
  startup();

  if (!LittleFS.begin()) Serial.println("FS Error");

  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
  ui.enableOTA();
}

void action() {
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
}
