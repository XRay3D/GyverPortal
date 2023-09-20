// использование BOX для вёрстки интерфейса

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
 GP::GyverPortal ui;

void build() {
  GP::GP.BUILD_BEGIN();
  GP::GP.THEME( GP::DARK);

  GP::GP.BOX_BEGIN(Align::JUSTIFY);
  GP::GP.LABEL("Switch");
  GP::GP.BOX_BEGIN(Align::RIGHT);
  GP::GP.LED("");
  GP::GP.SWITCH("");
  GP::GP.LED("");
  GP::GP.BOX_END();
  GP::GP.BOX_END();

  GP::GP.HR();

  GP::GP.BOX_BEGIN(Align::CENTER);
  GP::GP.SPINNER("sp1");
  GP::GP.SPINNER("sp2");
  GP::GP.BOX_END();

  GP::GP.HR();

  GP::GP.BOX_BEGIN(Align::JUSTIFY);
  GP::GP.LABEL("Slider");
  GP::GP.SLIDER("sld");
  GP::GP.BOX_END();

  GP::GP.HR();

  GP::GP.BOX_BEGIN(Align::JUSTIFY);
  GP::GP.LABEL("Setting");
  GP::GP.BOX_BEGIN(Align::RIGHT);
  GP::GP.CHECK("ch1");
  GP::GP.CHECK("ch2");
  GP::GP.CHECK("ch3");
  GP::GP.BOX_END();
  GP::GP.BOX_END();
  
  GP::GP.HR();

  GP::GP.BOX_BEGIN(Align::JUSTIFY);
  GP::GP.LABEL("Buttons");
  GP::GP.BOX_BEGIN(Align::RIGHT);
  GP::GP.BUTTON_MINI("b1", "Kek", "", GP_RED);
  GP::GP.BUTTON_MINI("b1", "Puk");
  GP::GP.BOX_END();
  GP::GP.BOX_END();

  GP::GP.HR();

  GP::GP.BOX_BEGIN(Align::JUSTIFY);
  GP::GP.LABEL("Upload settings");
  GP::GP.FILE_UPLOAD("file");
  GP::GP.BOX_END();

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
  ui.attach(action);
  ui.start();
}

void action() {
}

void loop() {
  ui.tick();
}
