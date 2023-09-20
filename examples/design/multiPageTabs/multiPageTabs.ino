// виртуальная мультистраничность (на одной странице) со вкладками

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
 GP::GyverPortal ui;

// конструктор страницы
void build() {
  GP::GP.BUILD_BEGIN();
  GP::GP.THEME( GP::DARK);

  // первый блок навигации
  GP::GP.NAV_TABS("Home,Settings,Keks,End");

  GP::GP.NAV_BLOCK_BEGIN();
  GP::GP.LABEL("Tab1");
  GP::GP.NAV_BLOCK_END();

  GP::GP.NAV_BLOCK_BEGIN();
  GP::GP.LABEL("Tab2");
  GP::GP.NAV_BLOCK_END();

  GP::GP.NAV_BLOCK_BEGIN();
  GP::GP.LABEL("Tab3");
  GP::GP.NAV_BLOCK_END();

  GP::GP.NAV_BLOCK_BEGIN();
  GP::GP.LABEL("Tab4");
  GP::GP.NAV_BLOCK_END();

  // второй блок навигации
  GP::GP.NAV_TABS("KEK,PUK,4EBUREK", GP_RED);

  // эти блоки сделаем макросами
  M_NAV_BLOCK(
    GP::GP.LABEL("block1");
    GP::GP.BUTTON("", "Btn");
  );

  M_NAV_BLOCK(
    GP::GP.LABEL("block2");
    GP::GP.LED("");
  );

  M_NAV_BLOCK(
    GP::GP.LABEL("block3");
    GP::GP.BREAK();
    GP::GP.TEXT("");
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

  // подключаем конструктор и запускаем
  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
}

void action() {
}

void loop() {
  ui.tick();
}
