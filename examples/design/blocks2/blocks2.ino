// пример использования "блоков" для оформления

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN(600);
    GP::GP.THEME(GP::DARK);

    GP::GP.BLOCK_BEGIN(GP_DIV);
    GP::GP.LABEL("Block div");
    GP::GP.BLOCK_END();

    GP::GP.BLOCK_BEGIN(GP_DIV, "", "My div txt");
    GP::GP.LABEL("Block div text");
    GP::GP.BLOCK_END();

    GP::GP.BLOCK_BEGIN(GP_TAB);
    GP::GP.LABEL("Block tab");
    GP::GP.BLOCK_END();

    GP::GP.BLOCK_BEGIN(GP_TAB, "", "My tab txt");
    GP::GP.LABEL("Block tab text");
    GP::GP.BLOCK_END();

    GP::GP.BLOCK_BEGIN(GP_TAB, "", "My tab txt red ", GP_RED);
    GP::GP.LABEL("Block tab text red");
    GP::GP.BLOCK_END();

    GP::GP.BLOCK_BEGIN(GP_THIN);
    GP::GP.LABEL("Block thin");
    GP::GP.BLOCK_END();

    GP::GP.BLOCK_BEGIN(GP_THIN, "", "My thin txt");
    GP::GP.LABEL("Block thin text");
    GP::GP.BLOCK_END();

    GP::GP.BLOCK_BEGIN(GP_THIN, "", "My thin txt red", GP_RED);
    GP::GP.LABEL("Block thin text red");
    GP::GP.BLOCK_END();

    // то же самое макросами

    M_BLOCK(
        GP_DIV,
        GP::GP.LABEL("Block div"););

    M_BLOCK(
        GP_DIV, "", "My div txt",
        GP::GP.LABEL("Block div text"););

    M_BLOCK(
        GP_TAB,
        GP::GP.LABEL("Block tab"););

    M_BLOCK(
        GP_TAB, "", "My tab txt",
        GP::GP.LABEL("Block tab text"););

    M_BLOCK(
        GP_TAB, "", "My tab txt red ", GP_RED,
        GP::GP.LABEL("Block tab text red"););

    M_BLOCK(
        GP_THIN,
        GP::GP.LABEL("Block thin"););

    M_BLOCK(
        GP_THIN, "", "My thin txt",
        GP::GP.LABEL("Block thin text"););

    M_BLOCK(
        GP_THIN, "", "My thin txt red", GP_RED,
        GP::GP.LABEL("Block thin text red"););

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
