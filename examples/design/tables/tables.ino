// пример использования таблиц для оформления

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>

GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN(600);
    GP::GP.THEME(GP::DARK);

    // включаем границы таблиц (ДЛЯ НАГЛЯДНОСТИ)
    GP::GP.TABLE_BORDER(true);

    // делаем таблицу вручную
    GP::GP.TITLE("Manual Table");
    GP::GP.TABLE_BEGIN();

    GP::GP.TR();
    GP::GP.TD();
    GP::GP.LABEL("Row 1");
    GP::GP.TD();
    GP::GP.CHECK("");

    GP::GP.TR();
    GP::GP.TD();
    GP::GP.LABEL("Row 2");
    GP::GP.TD();
    GP::GP.CHECK("");

    GP::GP.TABLE_END();

    // делаем таблицу с объединением ячеек
    GP::GP.TITLE("Span Table");
    GP::GP.TABLE_BEGIN();

    GP::GP.TR();
    GP::GP.TD(GP::Align::CENTER, 2);
    GP::GP.LABEL("Big table cell");

    GP::GP.TR();
    GP::GP.TD();
    GP::GP.LABEL("First");
    GP::GP.TD();
    GP::GP.LABEL("Second");

    GP::GP.TABLE_END();

    // авто таблица 1
    GP::GP.TITLE("Auto Table 1");
    GP::M_TABLE(GP::FOR(5, // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
                        GP::M_TR(
                            GP::LABEL("", String("Switch #") + I), GP::SWITCH(""))))
        .build();

    // авто таблица 2
    // с указанием размеров ячеек
    GP::GP.TITLE("Auto Table 2");
    // M_TABLE(        "30%,20%,50%",
    for (int i = 0; i < 5; i++) {
        GP::M_TR(
            GP::LABEL("", String("Label #") + i),
            GP::CHECK(""),
            GP::BUTTON("", "Btn"))
            .build();
    } //);

    // авто таблица 3
    // с указанием ориентации ячеек
    GP::GP.TITLE("Auto Table 3");
    // M_TABLE(        "",
    // GP_ALS(GP::Align::LEFT, GP::Align::CENTER, GP::Align::RIGHT),
    for (int i = 0; i < 3; i++) {
        GP::M_TR(GP::LABEL("", String(i)), GP::CHECK(""), GP::LED(""))
            .build();
    } //);

    GP::GP.BUILD_END();
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while (WiFi.status() != WL_CONNECTED) {
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
