// локальная работа портала

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>

// конструктор страницы
void build(GP::GyverPortal& p) {
    GP::GP.BUILD_BEGIN(GP::DARK);

    GP::GP.BUTTON("clk", "Click");
    GP::GP.BREAK();
    GP::GP.BUTTON("ext", "Exit");
    GP::GP.BREAK();
    // GP::GP.FILE_UPLOAD("test.txt"); Требует предварительного монтирования файловой системы
    GP::GP.BUILD_END();
}

// Обработчик событий
void action(GP::GyverPortal& p) {
    if(p.click("clk")) Serial.println("Button click");
    if(p.click("ext")) {
        Serial.println("Exit portal");
        p.stop();
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

    GP::GyverPortal ui;
    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
    Serial.println("Portal run");
    while(ui.tick())
        ;

    Serial.println("Setup end");
}

void loop() {
}
