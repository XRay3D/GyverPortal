// изменение имени окна

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    // однократно изменить название окна
    // GP::GP.PAGE_TITLE("MyPortal");

    // изменить и подключить обновление (1 в сек)
    GP::GP.PAGE_TITLE("MyPortal", "tit");
    GP::GP.UPDATE("tit");

    GP::GP.BUILD_END();
}

void action() {
    // менять при обновлении
    if(ui.update("tit"))
        ui.answer(String("MyPortal: ") + random(100));
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

    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
}

void loop() {
    ui.tick();
}
