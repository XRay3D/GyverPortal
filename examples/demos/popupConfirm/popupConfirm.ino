// всплывающее окно confirm
// активация по событию из скетча и по кнопке в браузере

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    // запрос на обновление каждую секунду
    GP::GP.UPDATE("cfm");
    GP::GP.CONFIRM("cfm");

    // вызов по кнопке
    GP::GP.CONFIRM("cfmT", "Confirm Text");
    GP::GP.BUTTON("btnC", "Confirm");
    GP::GP.UPDATE_CLICK("cfmT", "btnC");

    GP::GP.BUILD_END();
}

bool flag;

void action() {
    if(ui.update()) {
        // вызов с таймера
        if(flag && ui.update("cfm")) {
            flag = 0;
            ui.answer(String("Confirm! millis(): ") + millis());
        }

        // вызов с кнопки
        if(ui.update("cfmT"))
            ui.answer(1);
    }

    if(ui.click()) {
        if(ui.click("cfm")) Serial.println(String("cfm: ") + ui.getBool());
        if(ui.click("cfmT")) Serial.println(String("cfmT: ") + ui.getBool());
    }
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

    // "вызываем" раз в 5 секунд
    static uint32_t tmr;
    if(millis() - tmr >= 5000) {
        tmr = millis();
        flag = 1;
    }
}
