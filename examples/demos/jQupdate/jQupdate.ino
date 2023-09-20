// jQuery обновление на странице

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    GP::GP.JQ_SUPPORT();
    GP::GP.JQ_UPDATE_BEGIN(2000);
    GP::GP.LED("L0", random(2));
    GP::GP.LED("L1", random(2));
    GP::GP.LED("L2", random(2));
    GP::GP.LED("L3", random(2));
    GP::GP.LED("L4", random(2));
    GP::GP.LED("L6", random(2));
    GP::GP.JQ_UPDATE_END();

    GP::GP.BUILD_END();
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while(WiFi.status() != WL_CONNECTED) {
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
