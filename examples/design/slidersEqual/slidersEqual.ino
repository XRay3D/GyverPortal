// как сделать слайдеры одной ширины с подписью

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
 GP::GyverPortal ui;

void build() {
  GP::GP.BUILD_BEGIN();
  GP::GP.THEME( GP::DARK);

  M_BLOCK(
    M_BOX(
      GP::GP.LABEL("Velocity");
      M_BOX(
        Align::RIGHT,
        "220px",
        GP::GP.SLIDER("vel");
      );
    );

    M_BOX(
      GP::GP.LABEL("Accel.");
      M_BOX(
        Align::RIGHT,
        "220px",
        GP::GP.SLIDER("acc");
      );
    );
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
