// выводим файлы в браузере

#define AP_SSID ""
#define AP_PASS ""

#include <LittleFS.h>

#include <GyverPortal.h>
GP::GyverPortal ui(&LittleFS);

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    GP::GP.IMAGE("/test.png");
    GP::GP.BREAK();
    GP::GP.IMAGE("/test.gif");
    GP::GP.BREAK();
    GP::GP.EMBED("/test.txt");
    GP::GP.BREAK();
    GP::GP.VIDEO("/test.mp4");

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

    if(!LittleFS.begin()) Serial.println("FS Error");

    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
}

void action() {
}

void loop() {
    ui.tick();
}
