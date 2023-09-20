// автоматическая загрузка файлов
// также включены: автоматическое скачивание/удаление/переименование для работы менеджера файлов

#define AP_SSID ""
#define AP_PASS ""

// подключить библиотеку файловой системы (до #include GP::GyverPortal)
#include <LittleFS.h>

#include <GyverPortal.h>
 GP::GyverPortal ui(&LittleFS);  // передать ссылку на fs (SPIFFS/LittleFS)

// конструктор страницы
void build() {
  GP::GP.BUILD_BEGIN();
  GP::GP.THEME( GP::DARK);

  GP::GP.FILE_UPLOAD("file_upl");    // кнопка загрузки
  GP::GP.FOLDER_UPLOAD("folder_upl");// кнопка загрузки
  GP::GP.FILE_MANAGER(&LittleFS);    // файловый менеджер

  GP::GP.BUILD_END();
}

void setup() {
  startup();

  if (!LittleFS.begin()) Serial.println("FS Error");

  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
}

void action() {
  // СИГНАЛ об успешном окончании загрузки
  if (ui.uploadEnd()) {
    Serial.print("Uploaded file: ");
    Serial.print(ui.fileName());      // имя файла
    Serial.print(", from: ");
    Serial.println(ui.uploadName());  // имя формы загрузки
    // файл сохранится В КОРЕНЬ, С ИМЕНЕМ fileName()
    // или с сохранением пути вложенных папок
  }
}

void loop() {
  ui.tick();
}

void startup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}
