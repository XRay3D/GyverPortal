// тест графика PLOT_STOCK

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

#define PLOT_SIZE 100
int16_t arr[2][PLOT_SIZE];
uint32_t dates[PLOT_SIZE];
const char* names[] = {
    "kek",
    "puk",
};

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    GP::GP.PLOT_STOCK_DARK<2, PLOT_SIZE>("plot", names, dates, arr);
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

    // ставим последний элемент массива на текущую дату
    // gmt 3 для Москвы
    dates[PLOT_SIZE - 1] = GP::GPunix(2022, 1, 22, 21, 59, 0, 3);

    for(int i = 0; i < PLOT_SIZE; i++) {
        GP::GPaddInt(i * 2, arr[0], PLOT_SIZE);
        GP::GPaddInt(i * 5, arr[1], PLOT_SIZE);

        // добавляем значения графика по 5 секунд
        GP::GPaddUnixS(5, dates, PLOT_SIZE);
    }

    // подключаем конструктор и запускаем
    ui.attachBuild(build);
    ui.start();
}

void loop() {
    ui.tick();

    static uint32_t tmr;
    if(millis() - tmr >= 5000) {
        tmr = millis();
        GP::GPaddInt(random(100), arr[0], PLOT_SIZE);
        GP::GPaddInt(random(100), arr[1], PLOT_SIZE);
        GP::GPaddUnixS(5, dates, PLOT_SIZE);
    }
}
