// ручное скачивание файлов
// поднимает флаг запроса файла с запроса, содержащего точку

#define AP_SSID ""
#define AP_PASS ""

#include <LittleFS.h>

#include <GyverPortal.h>
GP::GyverPortal ui;

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    GP::GP.EMBED("/test.txt");
    GP::GP.BUTTON_LINK("/test.txt", "Открыть");
    GP::GP.BUTTON_DOWNLOAD("/test.txt", "Скачать");

    GP::GP.BUILD_END();
}

void setup() {
    startup();

    if(!LittleFS.begin()) Serial.println("FS Error");

    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
    ui.downloadAuto(0); // отключить авто скачивание
}

void action() {
    if(ui.download()) {
        Serial.println(ui.uri());

        // открыть файл любым способом и передать в библиотеку
        // путь к файлу - uri
        ui.sendFile(LittleFS.open(ui.uri(), "r"));
    }
}

void loop() {
    ui.tick();
}

void startup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP());
}