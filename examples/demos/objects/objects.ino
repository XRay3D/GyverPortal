// работа с "объектами" компонентов
// их можно создавать, настраивать, отправлять на страницу и опрашивать в action

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

GP::LABEL_BLOCK lbl("lbl");
GP::BUTTON btn("btn", "Button");
GP::TEXT txt("txt");
GP::SELECT sel("selo");

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN(GP::DARK);

    GP::GP.LABEL("Random text:");

    // лейбл с апдейтом
    GP::GP.UPDATE(lbl.name);
    GP::GP.LABEL_BLOCK(lbl);
    GP::GP.BREAK();

    GP::GP.BUTTON(btn);
    GP::GP.BREAK();
    txt.width = GP::getAlign(GP::Align::JUSTIFY);
    GP::GP.TEXT(txt);
    GP::GP.BREAK();
    GP::GP.SELECT("sel", "Sel 1,Sel 2,Sel 3,Sel 4");
    GP::GP.SELECT(sel);

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

    // настройка селекта
    sel.list = F("Sel 1,Sel 2,Sel 3,Sel 4");
    sel.numbers = true;
}

void action() {
    if(ui.update())
        ui.update(lbl); // автоматически обновить лейбл на странице

    if(ui.click()) {
        if(ui.click(btn)) Serial.println("Click");
        if(ui.click(txt)) Serial.println(txt.text);
        if(ui.click(sel)) Serial.println(sel.getValue());
    }
}

void loop() {
    ui.tick();

    // имитируем изменение строки в программе
    static uint32_t tmr;
    if(millis() - tmr >= 500) {
        tmr = millis();
        lbl.text = "";
        for(int i = 0; i < 5; i++) lbl.text += (char)random(48, 126);
    }
}
