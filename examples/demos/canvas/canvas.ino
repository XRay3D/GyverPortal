// пример с канвасом

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

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
}

void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    // поддержка канваса
    GP::GP.CANVAS_SUPPORT();

    // создать канвас и сразу вывести графику
    GP::GP.CANVAS_BEGIN("cnv", 350, 350);
    GP::Canvas cv; // начать рисовать (после вызова begin)
    cv.background("white");
    GP::GP.CANVAS_END(); // закончить рисовать и отправить результат

    // обновлять канвас по таймеру
    GP::GP.UPDATE("cnv", 300);

    // создать "пустой" канвас
    GP::GP.CANVAS("cnv2", 350, 350);

    GP::GP.BUTTON_MINI("btn", "Draw!"); // кнопка
    GP::GP.REDRAW_CLICK("cnv2", "btn"); // которая обновляет канвас

    GP::GP.BUILD_END();
}

void action() {
    // рисуем линии на первом (по таймеру)
    if(ui.update("cnv")) {
        GP::Canvas cv;
        cv.stroke("black");
        cv.line(0, 0, random(10, 300), random(10, 300));
        ui.answer(cv);
    }

    // рисуем шарики на втором (по кнопке)
    if(ui.update("cnv2")) {
        GP::Canvas cv;
        cv.fill("red");
        cv.noStroke();
        cv.circle(random(0, 300), random(0, 300), 10);
        ui.answer(cv);
    }
}

void loop() {
    ui.tick();
}
