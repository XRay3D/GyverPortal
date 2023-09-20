// почти все компоненты

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN(700); // ширина колонки

    GP::GP.THEME(GP::DARK);
    // GP.THEME(GP_LIGHT);

    // при ширине меньше 600 GRID отключится (блоки пойдут по вертикали)
    GP::GP.GRID_RESPONSIVE(600);

    GP::GP.TITLE("Desktop Dashboard Style");
    GP::GP.HR();

    M_GRID(
        M_BLOCK_TAB(
            "Settings",
            M_BOX(GP.LABEL("Some check 1"); GP::GP.CHECK(""););
            M_BOX(GP.LABEL("Some check 2"); GP::GP.CHECK(""););
            M_BOX(GP.LABEL("Some Switch 1"); GP::GP.SWITCH(""););
            M_BOX(GP.LABEL("Some Switch 2"); GP::GP.SWITCH("");););

        M_BLOCK_TAB(
            "WiFi config",
            M_BOX(GP.LABEL("SSID"); GP::GP.TEXT(""););
            M_BOX(GP.LABEL("Password"); GP::GP.TEXT(""););
            M_BOX(GP.LABEL("Host"); GP::GP.TEXT(""););););

    M_GRID(
        M_BLOCK_TAB(
            "Upload",
            M_BOX(GP.LABEL("Upload settings"); GP::GP.FILE_UPLOAD(""););
            M_BOX(GP.LABEL("Upload photo"); GP::GP.FILE_UPLOAD(""););
            M_BOX(GP.LABEL("Upload data"); GP::GP.FILE_UPLOAD("");););

        M_BLOCK_TAB(
            "Color",
            M_BOX(GP.LABEL("Color 1"); GP::GP.SLIDER(""););
            M_BOX(GP.LABEL("Color 2"); GP::GP.SLIDER(""););
            M_BOX(GP.LABEL("Color 3"); GP::GP.SLIDER(""););););

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
