// работа с компонентами в виде объектов (beta)

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

GP::TITLE tit("t1");
GP::LABEL lab("l1");
GP::LABEL_BLOCK labb("lb1");

GP::LED led1("led1");
GP::LED_RED led2("led2");
GP::LED_GREEN led3("led3");

GP::BUTTON btn("b1");
GP::BUTTON_MINI btnm("b2");

GP::NUMBER num("num");
GP::NUMBER_F numf("numf");

GP::TEXT txt("txt");
GP::PASS pas("pas");

GP::AREA ar("ar");

GP::CHECK ch("ch");
GP::SWITCH sw("sw");

GP::DATE dat("dat");
GP::TIME tm("tm");
GP::COLOR col("col");

GP::SPINNER sp("sp");
GP::SLIDER sl("sl");

GP::SELECT sel("sel");

void build() {
    GP::GP.BUILD_BEGIN(GP::DARK);
    // GP::GP.UPDATE("t1,l1,lb1,led1,led2,led3,num,numf,txt,pas,ar,ch,sw,dat,tm,col,sp,sl");

    GP::GP.PUT_OBJ(tit);
    GP::GP.PUT_OBJ(lab);
    GP::GP.PUT_OBJ(labb);

    GP::GP.PUT_OBJ(led1);
    GP::GP.PUT_OBJ(led2);
    GP::GP.PUT_OBJ(led3);

    GP::GP.PUT_OBJ(btn);
    GP::GP.PUT_OBJ(btnm);

    GP::GP.FORM_BEGIN("/save");
    GP::GP.PUT_OBJ(num);
    GP::GP.PUT_OBJ(numf);

    GP::GP.PUT_OBJ(txt);
    GP::GP.PUT_OBJ(pas);

    GP::GP.PUT_OBJ(ar);
    GP::GP.BREAK();

    GP::GP.PUT_OBJ(ch);
    GP::GP.PUT_OBJ(sw);

    GP::GP.PUT_OBJ(dat);
    GP::GP.PUT_OBJ(tm);
    GP::GP.PUT_OBJ(col);

    GP::GP.PUT_OBJ(sp);
    GP::GP.PUT_OBJ(sl);

    GP::GP.PUT_OBJ(sel);

    GP::GP.SUBMIT("Submit");
    GP::GP.FORM_END();

    GP::GP.BUILD_END();
}

void action() {
    if(ui.form("/save")) {
        ui.copy(num);
        ui.copy(numf);
        Serial.println(num.value);
        Serial.println(numf.value);

        ui.copy(txt);
        ui.copy(pas);
        Serial.println(txt.text);
        Serial.println(pas.text);

        ui.copy(ar);
        Serial.println(ar.text);

        ui.copy(ch);
        ui.copy(sw);
        Serial.println(ch.state);
        Serial.println(sw.state);

        ui.copy(dat);
        ui.copy(tm);
        Serial.println(dat.date.encode());
        Serial.println(tm.time.encode());

        ui.copy(col);
        Serial.println(col.color.encode());

        ui.copy(sp);
        Serial.println(sp.value);

        ui.copy(sl);
        Serial.println(sl.value);

        ui.copy(sel);
        Serial.println(sel.selected);
        Serial.println(sel.getValue());
    }

    if(ui.click()) {
        if(ui.clickUp(btn)) Serial.println("down");
        if(ui.clickDown(btn)) Serial.println("up");

        if(ui.click(btn)) Serial.println("btn");
        if(ui.click(btnm)) Serial.println("btn mini");

        if(ui.click(num)) Serial.println(num.value);
        if(ui.click(numf)) Serial.println(numf.value);

        if(ui.click(txt)) Serial.println(txt.text);
        if(ui.click(pas)) Serial.println(pas.text);

        if(ui.click(ar)) Serial.println(ar.text);

        if(ui.click(ch)) Serial.println(ch.state);
        if(ui.click(sw)) Serial.println(sw.state);

        if(ui.click(dat)) Serial.println(dat.date.encode());
        if(ui.click(tm)) Serial.println(tm.time.encode());
        if(ui.click(col)) Serial.println(col.color.encode());

        if(ui.click(sp)) Serial.println(sp.value);
        if(ui.click(sl)) Serial.println(sl.value);

        if(ui.click(sel)) {
            Serial.println(sel.selected);
            Serial.println(sel.getValue());
        }
    }

    if(ui.update()) {
        tit.text = random(10);
        lab.text = random(10);
        labb.text = random(10);
        ui.update(tit);
        ui.update(lab);
        ui.update(labb);

        led1.state = random(2);
        led2.state = random(2);
        led3.state = random(2);
        ui.update(led1);
        ui.update(led2);
        ui.update(led3);

        num.value = random(10);
        numf.value = random(100) / 10.0;
        ui.update(num);
        ui.update(numf);

        txt.text = random(100);
        pas.text = random(100);
        ui.update(txt);
        ui.update(pas);

        ar.text = random(10000);
        ui.update(ar);

        ch.state = random(2);
        sw.state = random(2);
        ui.update(ch);
        ui.update(sw);

        dat.date = {
            (uint16_t)random(2000, 2030), (uint8_t)random(13), (uint8_t)random(13)};
        tm.time = {
            (uint8_t)random(24), (uint8_t)random(60), (uint8_t)random(60)};
        ui.update(dat);
        ui.update(tm);

        col.color = random(0xffffff);
        ui.update(col);

        sp.value = random(50);
        ui.update(sp);

        sl.value = random(10);
        ui.update(sl);
    }
}

void setup() {
    btn.text = "button 1";
    btnm.text = "button 2";
    numf.decimals = 3;
    sp.decimals = 0;
    sl.decimals = 2;
    sl.step = 0.5;

    sel.list = "val 1,val 2,val 3";

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

void loop() {
    ui.tick();
}
