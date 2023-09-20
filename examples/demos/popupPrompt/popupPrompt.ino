// всплывающее окно prompt
// активация по событию из скетча и по кнопке в браузере
// ответ на update будет текстом в поле ввода текста
// ответ должен иметь ненулевую длину! пустой ответ или "" нельзя

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

int val;

void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    // запрос на обновление каждую секунду
    GP::GP.UPDATE("prm");
    GP::GP.PROMPT("prm", "Random number");

    // вызов по кнопке
    GP::GP.PROMPT("prmT", "val variable:");
    GP::GP.BUTTON("btnP", "Prompt");
    GP::GP.UPDATE_CLICK("prmT", "btnP");

    GP::GP.BUILD_END();
}

bool flag;

void action() {
    if(ui.update()) {
        if(flag && ui.update("prm")) {
            flag = 0;
            ui.answer(random(1000));
        }

        if(ui.update("prmT"))
            ui.answer(val);
    }

    if(ui.click()) {
        if(ui.click("prm")) Serial.println("Prm: " + ui.getString());
        if(ui.click("prmT")) {
            Serial.println("PrmT: " + ui.getString());
            val = ui.getInt();
        }
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

    // "вызываем" раз в 10 секунд
    static uint32_t tmr;
    if(millis() - tmr >= 10000) {
        tmr = millis();
        flag = 1;
    }
}
