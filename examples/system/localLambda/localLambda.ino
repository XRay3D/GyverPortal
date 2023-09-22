// локальная работа портала
// используем лямбда функции

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP());

    GP::GyverPortal ui;
    ui.start();
    ui.attachBuild([]() {
        GP::GP.BUILD_BEGIN();
        GP::GP.THEME(GP::DARK);

        GP::GP.BUTTON("clk", "Click");
        GP::GP.BUTTON("ext", "Exit");

        GP::GP.BUILD_END();
    });

    ui.attach([](GP::GyverPortal& p) {
        if(p.click("clk"))
            Serial.println("Button click");
        if(p.click("ext")) {
            Serial.println("Exit portal");
            p.stop();
        }
    });

    Serial.println("Portal run");
    while(ui.tick())
        ;

    Serial.println("Setup end");
}

void loop() {
}
