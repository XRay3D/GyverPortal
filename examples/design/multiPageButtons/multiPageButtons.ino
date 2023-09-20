// пример организации многостраничности на кнопках

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

// конструктор страницы
void build() {
    GP::GP.BUILD_BEGIN();
    GP::GP.THEME(GP::DARK);

    // ui.uri() возвращает текущий адрес страницы
    // используем его в конструкции с if для постройки страницы
    // переход на страницы может осуществляться в адресной строке браузера
    // или по кнопке-ссылке BUTTON_LINK

    // страница с формой
    // ВАЖНО: url страницы должен совпадать с именем формы!
    // чтобы форма отображалась в браузере после Submit
    if(ui.uri("/save")) {
        GP::GP.FORM_BEGIN("/save");
        GP::GP.TEXT("txt", "text", "");
        GP::GP.BREAK();
        GP::GP.SUBMIT("Submit");
        GP::GP.FORM_END();
        GP::GP.BUTTON_LINK("/", "Back");

        // страница с кнопкой, на которую можно кликнуть
    } else if(ui.uri("/clicks")) {
        GP::GP.BUTTON("btn", "Button");
        GP::GP.BUTTON_LINK("/", "Back");

        // страница с лампочкой, которая сама переключается
    } else if(ui.uri("/updates")) {
        GP::GP.UPDATE("led");
        GP::GP.LABEL("LED: ");
        GP::GP.LED_RED("led", 0);
        GP::GP.BREAK();
        GP::GP.BUTTON_LINK("/", "Back");

        // главная страница, корень, "/"
    } else {
        GP::GP.BUTTON_LINK("/save", "Form");
        GP::GP.BUTTON_LINK("/clicks", "Clicks");
        GP::GP.BUTTON_LINK("/updates", "Updates");
    }

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

bool led;
void action() {
    // имитация активности
    if(ui.form("/save")) Serial.println(ui.getString("txt"));
    if(ui.click("btn")) Serial.println("Button");
    if(ui.update("led")) ui.answer(led = !led);
}

void loop() {
    ui.tick();
}
