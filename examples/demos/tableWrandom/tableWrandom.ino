// таблица с рандомным содержимым

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN(600);
    // 4GP::GP.THEME(GP::DARK);
    static GP::Align arr[]{GP::Align::LEFT, GP::Align::LEFT};
    GP::GP.TABLE_BEGIN("", arr);
    //    GP::GP.TABLE_BORDER(true);
    *GP::tmpPageBuf += R"(<tr>
    <th>First name</th>
    <th>Last name</th>
  </tr>)";
    for(int i{}; i < 10; i++) {
        GP::GP.TR();
        GP::GP.TD();
        rndLabel();
        GP::GP.TD();
        rndComp();
    }
    GP::GP.TABLE_END();

    //    M_TABLE(
    //        "",
    //        GP_ALS(GP::Align::LEFT, GP::Align::LEFT),
    //        for (int i = 0; i < 10; i++) {
    //            M_TR(rndLabel(), rndComp());
    //        });

    GP::GP.BUILD_END();
}

void rndComp() {
    switch(random(4)) {
    case 0: GP::GP.SLIDER("sld"); break;
    case 1: GP::GP.SPINNER("spn"); break;
    case 2: GP::GP.SELECT("sel", "1,2,3"); break;
    case 3: GP::GP.SWITCH("sw"); break;
    }
}

void rndLabel() {
    String s;
    int len = random(5, 15);
    for(int i = 0; i < len; i++)
        s += (char)random(40, 127);
    GP::GP.LABEL(s);
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
