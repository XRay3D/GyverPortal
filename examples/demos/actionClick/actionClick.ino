// получаем клики со страницы и значения с компонентов

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

// переменные
bool valCheck;
bool valSwitch;
String valText;
int valNum;
char valPass[10];
float valSpin;
int valSlider;
GP::GPdate valDate;
GP::GPtime valTime;
GP::GPcolor valCol;
int valSelect;
int valRad;

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN(GP::DARK);

    GP::GP.TITLE("Title", "t1");
    GP::GP.HR();

    GP::GP.LABEL("Value: ");
    GP::GP.LABEL("NAN", "val");
    GP::GP.BREAK();
    GP::GP.LABEL("Check: ");
    GP::GP.CHECK("ch", valCheck);
    GP::GP.BREAK();
    GP::GP.LABEL("Switch: ");
    GP::GP.SWITCH("sw", valSwitch);
    GP::GP.BREAK();
    GP::GP.TEXT("txt", "text", valText);
    GP::GP.BREAK();
    GP::GP.NUMBER("num", "number", valNum);
    GP::GP.BREAK();
    GP::GP.PASS("pass", "pass", valPass);
    GP::GP.BREAK();
    GP::GP.SPINNER("spn", valSpin);
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
    GP::GP.BUTTON("btn", "Button");

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
    // был клик по компоненту
    if(ui.click()) {
        // проверяем компоненты и обновляем переменные

        // 1. переписали вручную
        if(ui.click("ch")) {
            valCheck = ui.getBool("ch");
            Serial.print("Check: ");
            Serial.println(valCheck);
        }

        // 2. автоматическое обновление переменной
        if(ui.clickBool("sw", valSwitch)) {
            Serial.print("Switch: ");
            Serial.println(valSwitch);
        }

        if(ui.clickString("txt", valText)) {
            Serial.print("Text: ");
            Serial.println(valText);
        }

        if(ui.clickInt("num", valNum)) {
            Serial.print("Number: ");
            Serial.println(valNum);
        }

        if(ui.clickStr("pass", valPass)) {
            Serial.print("Password: ");
            Serial.println(valPass);
        }

        if(ui.clickFloat("spn", valSpin)) {
            Serial.print("Spinner: ");
            Serial.println(valSpin);
        }

        if(ui.clickInt("sld", valSlider)) {
            Serial.print("Slider: ");
            Serial.println(valSlider);
        }

        if(ui.clickDate("date", valDate)) {
            Serial.print("Date: ");
            Serial.println(valDate.encode());
        }

        if(ui.clickTime("time", valTime)) {
            Serial.print("Time: ");
            Serial.println(valTime.encode());
        }

        if(ui.clickColor("col", valCol)) {
            Serial.print("Color: ");
            Serial.println(valCol.encode());
        }

        if(ui.clickInt("sel", valSelect)) {
            Serial.print("Select: ");
            Serial.println(valSelect);
        }
        if(ui.clickInt("rad", valRad)) {
            Serial.print("Radio: ");
            Serial.println(valRad);
        }

        if(ui.click("btn")) Serial.println("Button click");
    }
}

void loop() {
    ui.tick();
}
