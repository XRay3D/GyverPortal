// пример использования таблиц для оформления

#define AP_SSID ""
#define AP_PASS ""

#include "GyverPortal.h"

/* //macro
. Variables and constants in RAM (global, static), used 29436 / 80192 bytes (36%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ DATA     1532     initialized variables
╠══ RODATA   1304     constants
╚══ BSS      26600    zeroed variables
. Instruction RAM (IRAM_ATTR, ICACHE_RAM_ATTR), used 60223 / 65536 bytes (91%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ ICACHE   32768    reserved space for flash instruction cache
╚══ IRAM     27455    code in IRAM
. Code in flash (default, ICACHE_FLASH_ATTR), used 340196 / 1048576 bytes (32%)
║   SEGMENT  BYTES    DESCRIPTION
╚══ IROM     340196   code in flash
*/

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
#if 0
  // авто таблица 1
  GP::GP.TITLE("Auto Table 1");
  M_TABLE(for (int i{}; i < 5; i++) {
    // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
    M_TR(
      GP::GP.LABEL(String("Switch #") + i),
      GP::GP.SWITCH(""));
  });


  // авто таблица 2
  // с указанием размеров ячеек
  GP::GP.TITLE("Auto Table 2");
  M_TABLE(
    "30%,20%,50%",
    for (int i{}; i < 5; i++) {
      M_TR(
        GP::GP.LABEL(String("Label #") + i),
        GP::GP.CHECK(""),
        GP::GP.BUTTON("", "Btn"));
    });

  // авто таблица 3
  // с указанием ориентации ячеек
  GP::GP.TITLE("Auto Table 3");
  M_TABLE(
    "",
    GP_ALS(GP::Align::LEFT, GP::Align::CENTER, GP::Align::RIGHT),
    for (int i{}; i < 3; i++) {
      M_TR(GP::GP.LABEL(String(i)), GP::GP.CHECK(""), GP::GP.LED(""));
    });

#else

  // авто таблица 1
  GP::GP.TITLE("Auto Table 1");
  GP::_TABLE(
    GP::FOR{
      5,  // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
      GP::_TR(
        GP::LABEL("", String("Switch #")),
        GP::SWITCH("")) }).build();

  // авто таблица 2
  // с указанием размеров ячеек
  GP::GP.TITLE("Auto Table 2");
  GP::_TABLE(
    "30%,20%,50%",
    GP::FOR{
      5,
      GP::_TR(
        GP::LABEL("", String("Label #")),
        GP::CHECK(""),
        GP::BUTTON("", "Btn")) }).build();

  // авто таблица 3
  // с указанием ориентации ячеек
  GP::GP.TITLE("Auto Table 3");
  GP::_TABLE(
    "", std::array{ GP::Align::LEFT, GP::Align::CENTER, GP::Align::RIGHT },
    GP::FOR{
      3,
      GP::_TR(GP::LABEL("", String("#")), GP::CHECK(""), GP::LED("")) }).build();

#endif
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
