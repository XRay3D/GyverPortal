// пример использования "блоков" для оформления

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
 GP::GyverPortal ui;

void build() {
  GP::GP.BUILD_BEGIN(600);
  GP::GP.THEME( GP::DARK);

  // создаём блок вручную
  GP::GP.BLOCK_TAB_BEGIN("MOTOR CONFIG");
  M_BOX(GP.LABEL("Velocity"); GP::GP.SLIDER("vel"););
  M_BOX(GP.LABEL("Accel."); GP::GP.SLIDER("acc"););
  GP::GP.BLOCK_END();

  // через макрос
  M_BLOCK_TAB(
    "MANUAL",
    M_BOX(GP.LABEL("Duty"); GP::GP.SLIDER("spd"););
    M_BOX(GP.BUTTON("bkw", "◄"); GP::GP.BUTTON("frw", "►"););
  );

  GP::GP.BUILD_END();
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
  ui.start();
}

void loop() {
  ui.tick();
}
