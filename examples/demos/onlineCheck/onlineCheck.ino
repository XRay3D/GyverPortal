// изменение имени окна

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
 GP::GyverPortal ui;

void build() {
  GP::GP.BUILD_BEGIN();
  GP::GP.THEME( GP::DARK);

  // проверять статус платы
  // зажми reset и смотри на название вкладки браузера
  GP::GP.ONLINE_CHECK();
  
  GP::GP.TITLE("MyPortal");
  
  GP::GP.BUILD_END();
}

void action() {
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
}

void loop() {
  ui.tick();
}
