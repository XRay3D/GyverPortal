// AJAX обновление на странице

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

// переменные
int valNum;
String valPass;
int valSlider;
float valSpin;
GP::Date valDate;
GP::Time valTime;
GP::GPcolor valCol;
int valSelect;
int valRad;

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    // список имён компонентов на обновление
    GP::GP.UPDATE("t1,lb,lbb,ch,led,sw,txt,num,pass,sld,date,time,spn,col,sel,rad");

    // обновление случайным числом
    GP::GP.TITLE("Title", "t1");
    GP::GP.HR();
    GP::GP.LABEL("Label: ");
    GP::GP.LABEL("NAN", "lb");
    GP::GP.BREAK();
    GP::GP.LABEL("Label Block: ");
    GP::GP.LABEL_BLOCK("NAN", "lbb");
    GP::GP.BREAK();
    GP::GP.LABEL("Check: ");
    GP::GP.CHECK("ch");
    GP::GP.BREAK();
    GP::GP.LABEL("Led: ");
    GP::GP.LED("led");
    GP::GP.BREAK();
    GP::GP.LABEL("Switch: ");
    GP::GP.SWITCH("sw");
    GP::GP.BREAK();
    GP::GP.TEXT("txt", "text");
    GP::GP.BREAK();

    // обновление из переменной (храним значение)
    GP::GP.NUMBER("num", "number", valNum);
    GP::GP.BREAK();
    GP::GP.PASS("pass", "pass", valPass);
    GP::GP.BREAK();
    GP::GP.SPINNER("spn");
    GP::GP.SLIDER("sld", valSlider, 0, 10);
    GP::GP.BREAK();
    GP::GP.DATE("date", valDate);
    GP::GP.BREAK();
    GP::GP.TIME("time", valTime);
    GP::GP.BREAK();
    GP::GP.COLOR("col", valCol);
    GP::GP.BREAK();
    GP::GP.SELECT("sel", "val 1,val 2,val 3", valSelect);
    GP::GP.BREAK();
    GP::GP.RADIO("rad", 0, valRad);
    GP::GP.LABEL("Value 0");
    GP::GP.BREAK();
    GP::GP.RADIO("rad", 1, valRad);
    GP::GP.LABEL("Value 1");
    GP::GP.BREAK();
    GP::GP.RADIO("rad", 2, valRad);
    GP::GP.LABEL("Value 2");
    GP::GP.BREAK();
    GP::GP.RADIO("rad", 3, valRad);
    GP::GP.LABEL("Value 3");
    GP::GP.BREAK();
    GP::GP.BREAK();
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
}

void action() {
    // было обновление
    if(ui.update()) {
        // 1. ищем, какой компонент запрашивает обновление
        // и вручную отправляем рандомное значение
        if(ui.update("t1")) ui.answer(random(100));
        if(ui.update("lb")) ui.answer(random(1000));
        if(ui.update("lbb")) ui.answer(random(1000));
        if(ui.update("ch")) ui.answer(random(2));
        if(ui.update("led")) ui.answer(random(2));
        if(ui.update("sw")) ui.answer(random(2));
        if(ui.update("txt")) ui.answer(random(1000));

        // 2. автоматическое обновление из переменной
        ui.updateInt("num", valNum);
        ui.updateString("pass", valPass);
        ui.updateInt("spn", valSpin);
        ui.updateInt("sld", valSlider);
        ui.updateDate("date", valDate);
        ui.updateTime("time", valTime);
        ui.updateColor("col", valCol);
        ui.updateInt("sel", valSelect);
        ui.updateInt("rad", valRad);
    }
}

void loop() {
    ui.tick();

    // имитируем изменение переменных "откуда то из программы"
    static uint32_t tmr;
    if(millis() - tmr >= 500) {
        tmr = millis();
        valNum = random(1000);
        valPass = random(1000);
        valSlider = random(10);
        valSpin = random(50);
        valDate.set(random(2000, 2030), random(13), random(13));
        valTime.set(random(24), random(60), random(60));
        valCol.setHEX(random(0xffffff));
        valSelect = random(3);
        valRad = random(4);
    }
}
