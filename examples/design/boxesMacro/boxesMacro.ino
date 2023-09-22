// использование BOX для вёрстки интерфейса

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    M_BOX(
        GP::Align::JUSTIFY,
        GP::GP.LABEL("Switch");
        M_BOX(
           GP:: Align::RIGHT,
            GP::GP.LED("");
            GP::GP.SWITCH("");
            GP::GP.LED("");););

    GP::GP.HR();

    M_BOX(
        GP::Align::CENTER,
        GP::GP.SPINNER("sp1");
        GP::GP.SPINNER("sp2"););

    GP::GP.HR();

    M_BOX(
        GP::Align::JUSTIFY,
        GP::GP.LABEL("Slider");
        GP::GP.SLIDER("sld"););

    GP::GP.HR();

    M_BOX(
        GP::Align::JUSTIFY,
        GP::GP.LABEL("Setting");
        M_BOX(
            GP::Align::RIGHT,
            GP::GP.CHECK("ch1");
            GP::GP.CHECK("ch2");
            GP::GP.CHECK("ch3");););

    GP::GP.HR();

    M_BOX(
        GP::Align::JUSTIFY,
        GP::GP.LABEL("Buttons");
        M_BOX(
            GP::Align::RIGHT,
            GP::GP.BUTTON_MINI("b1", "Kek", "", GP::RED);
            GP::GP.BUTTON_MINI("b1", "Puk");););

    GP::GP.HR();

    M_BOX(
        GP::Align::JUSTIFY,
        GP::GP.LABEL("Upload settings");
        GP::GP.FILE_UPLOAD("file"););

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

    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
}

void action() {
}

void loop() {
    ui.tick();
}
