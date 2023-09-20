// почти все компоненты

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
 GP::GyverPortal ui;

void action();

void build() {
  GP::GP.BUILD_BEGIN();
  GP::GP.THEME( GP::DARK);
  //GP.THEME(GP_LIGHT);

  GP::GP.TITLE("GyverPortal");
  GP::GP.HR();

  GP::GP.NAV_TABS_LINKS("/,/home,/sett,/kek", "Home,Settings,Kek");
  
  M_SPOILER(
    "Spoiler",
    GP::GP.LABEL("Hello!");
  );

  M_BLOCK(
    GP::GP.LABEL("Checks & LED");
    GP::GP.BREAK();
    GP::GP.LABEL_BLOCK("label block");
    GP::GP.LED("");
    GP::GP.CHECK("");
    GP::GP.SWITCH("");
  );

  M_BLOCK_TAB(
    "Block Tab",
    GP::GP.LABEL("Inputs");
    M_BOX(GP.LABEL("Number");   GP::GP.NUMBER("", "", 123);     );
    M_BOX(GP.LABEL("Float");    GP::GP.NUMBER_F("", "", 3.14);  );
    M_BOX(GP.LABEL("Text");     GP::GP.TEXT("", "", "Hello");   );
    M_BOX(GP.LABEL("Password"); GP::GP.PASS("", "", "Pass");    );
    GP::GP.AREA("", 3, "Text area");
  );

  M_BLOCK_THIN(
    M_BOX(GP.LABEL("Date");   GP::GP.DATE("");  );
    M_BOX(GP.LABEL("Time");   GP::GP.TIME("");  );
    M_BOX(GP.LABEL("Color");  GP::GP.COLOR(""); );
  );

  M_BLOCK_THIN_TAB(
    "Thin Tab",
    GP::GP.LABEL("Upload File/Folder");
    M_BOX(
      Align::CENTER,
      GP::GP.FILE_UPLOAD("");
      GP::GP.FOLDER_UPLOAD("");
    );
  );

  M_BOX(GP.LABEL("Select");   GP::GP.SELECT("", "Some,Drop,List");  );
  M_BOX(GP.LABEL("Slider");   GP::GP.SLIDER("");  );
  M_BOX(GP.LABEL("Spinner");  GP::GP.SPINNER(""); );

  GP::GP.BUTTON("", "Button");
  GP::GP.BUTTON_MINI("", "Btn Mini");

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

  // подключаем конструктор и запускаем
  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
}

void action() {
}

void loop() {
  ui.tick();
}
