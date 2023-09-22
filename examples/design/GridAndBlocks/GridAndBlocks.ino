// пример использования "блоков" и сетки для оформления

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN(600);
    GP::GP.THEME(GP::DARK);

    // позволяет "отключить" таблицу при ширине экрана меньше 700px
    GP::GP.GRID_RESPONSIVE(700);

    M_GRID(
        M_BLOCK_TAB(
            "MOTOR CONFIG",
            M_BOX(GP::GP.LABEL("Velocity"); GP::GP.SLIDER("vel"););
            M_BOX(GP::GP.LABEL("Accel."); GP::GP.SLIDER("acc");););
        M_BLOCK_TAB(
            "MANUAL",
            M_BOX(GP::GP.LABEL("Duty"); GP::GP.SLIDER("spd"););
            M_BOX(GP::GP.BUTTON("bkw", "◄"); GP::GP.BUTTON("frw", "►"););););

    M_GRID(
        M_BLOCK_TAB(
            "PID CONFIG",
            M_BOX(GP::GP.LABEL("Kp"); GP::GP.SPINNER("kp"););
            M_BOX(GP::GP.LABEL("Ki"); GP::GP.SPINNER("ki"););
            M_BOX(GP::GP.LABEL("Kd"); GP::GP.SPINNER("kd");););

        M_BLOCK_TAB(
            "SPEED",
            GP::GP.SWITCH("spdOn");
            GP::GP.BREAK();
            M_BOX(GP::GP.LABEL("Speed"); GP::GP.SLIDER("spdPid"););
            M_BOX(GP::GP.BUTTON("bkwPid", "◄"); GP::GP.BUTTON("frwPid", "►"););););

    M_BLOCK_TAB(
        "PLANNER",
        GP::GP.SWITCH("planOn");
        GP::GP.BREAK();
        M_BOX(GP::GP.LABEL("Target"); GP::GP.SLIDER("tar"););
        M_BOX(GP::GP.LABEL("Stop zone"); GP::GP.SLIDER("deadz");););

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
    ui.start();
}

void loop() {
    ui.tick();
}
