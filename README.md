[![Foo](https://img.shields.io/badge/Version-1.7-brightgreen.svg?style=flat-square)](#versions)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)

# GyverPortal
### [English README!](/README_EN.md)  
> На ESP32 некорреткно работает режим обновлений, дублирует элементы. А также клики по элементам приходят асинхронно с обновлением страницы (как в примере menuTabs). Пока не уверен, что получится починить.  

Простой конструктор веб интерфейса для esp8266 и ESP32
- Простой конструктор - делаем страницы без знаний HTML и CSS
- Библиотека является обёрткой для стандартной ESP8266WebServer
- Позволяет быстро создать вебморду для управления и настройки своего девайса
- Компактный читаемый код в "скетче", никаких внешних обработчиков и лямбда-функций
- Конструктор использует стандартные HTML формы, CSS и javascript
- Элементы конструктора хранятся во Flash памяти
- Никаких глобальных буферов, всё генерируется на лету
- Приятный дизайн из коробки + тёмная тема
- Адаптировано под мобильные устройства и ПК
- Встроенные инструменты для удобного парсинга значений с формы
- Возможность настроить автоматическое обновление значений переменных по действию со страницы
- Встроенные жабаскрипты для AJAX, работа без обновления всей страницы:
  - Клики по компонентам, изменение их значений
  - Обновление компонентов по таймеру
  - График в реальном времени
  - Текстовое окно отладки (отправляем из программы)
- Компоненты конструктора:
  - Заголовок
  - Подпись
  - Разделитель
  - Перенос строки
  - Блок для объединения компонентов
  - Веб-форма (блок)
  - Кнопка submit (для форм)
  - Поле ввода текста
  - Многострочное поле ввода текста
  - Поле ввода пароля
  - Галочка (чекбокс)
  - Выключатель
  - Слайдер
  - Слайдер с подписью
  - Выбор времени
  - Выбор даты
  - Селектор (дропбокс)
  - Кнопка
  - Мини кнопка
  - "Светодиод" индикатор
  - Окно лога для отладки (веб Serial порт)
  - Несколько типов графиков (требуется интернет)

![demo](/docs/demoBig.png)  

### Возможности
Библиотека позволяет генерировать динамический веб интерфейс для управления своим электронным устройством из браузера (нужно зайти на IP адрес платы в строке адреса). Три основных способа взаимодействия с интерфейсом и сценарии использования:
- **Форма и кнопка отправить**: при нажатии на кнопку submit страница перезагружается, а в программу приходят данные со всех компонентов, входящих в форму (текст в поле ввода, положения слайдеров и чекбоксов, и так далее). Удобно для однократного ввода данных (настройки подключения и тому подобное).
- **Клик по компоненту**: при клике на почти любой компонент интерфейса (при изменении его состояния или значения) можно получить его актуальное значение без перезагрузки страницы. Удобно для управления и настройки (галочки, кнопки, слайдеры).
- **Обновление** значений и состояний компонентов в реальном времени без перезагрузки страницы. Удобно для индикации работы и получения текущих численных и текстовых значений из программы, вывод графиков в реальном времени.

### Совместимость
esp8266, esp32

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Документация](#docs)
- [Использование](#usage)
  - [Начало работы](#server)
  - [Конструктор](#build)
  - [Режим форм](#form)
  - [Режим кликов](#click)
  - [Режим обновлений](#update)
  - [Автообновление переменных](#auto)
  - [Графики](#plots)
  - [Лог](#log)
  - [Свои компоненты, API](#api)
- [Примеры](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **GyverPortal** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverPortal/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
`GyverPortal portal;`

<a id="docs"></a>
## Документация
<details>
<summary>Функции конструктора</summary>

```cpp
// создание страницы
BUILD_BEGIN(строка);                // начать построение. Добавляет начальный HTML код
BUILD_END();                        // завершить построение. Добавляет завершающий HTML код и открывает страницу

// темы
add.THEME(тема);                    // установить тему
GP_LIGHT    - светлая тема
GP_DARK     - тёмная тема

// создание формы
add.FORM_BEGIN(имя);                // начать форму с именем (имя)
add.FORM_END();                     // завершить форму

// пустая форма с одной кнопкой
add.FORM_SUBMIT(имя, текст);        // форма с именем (имя) кнопкой (текст)

// компоненты формы
add.BUTTON(имя, текст);             // кнопка
add.BUTTON(имя, текст, id);         // кнопка (id компонента, данные с которого кнопка отправит данные по click)
add.BUTTON_MINI(имя, текст);        // мини кнопка
add.BUTTON_MINI(имя, текст, id);    // мини кнопка (id компонента, данные с которого кнопка отправит данные по click)

add.NUMBER(имя, подсказка, число);  // поле ввода текста, число
add.TEXT(имя, подсказка, текст);    // поле ввода текста
add.PASS(имя, подсказка, текст);    // поле ввода пароля
add.CHECK(имя);                     // чекбокс, умолч. выключен
add.CHECK(имя, состояние);          // чекбокс
add.SWITCH(имя);                    // выключатель, умолч. выключен
add.SWITCH(имя, состояние);         // выключатель
add.DATE(имя);                      // ввод даты, умолч. пустой
add.DATE(имя, GPdate);              // ввод даты
add.TIME(имя);                      // ввод времени, умолч. пустой
add.TIME(имя, GPtime);              // ввод времени
add.SELECT(имя, список);            // селектор (дропбокс), элементы списка разделены запятой. Список может быть PSTR
add.SELECT(имя, список, активный);  // + текущий активный пункт
add.SLIDER(имя, число, мин, макс);                // слайдер
add.SLIDER(имя, число, мин, макс, шаг);           // слайдер
add.SLIDER(имя, подпись, число, мин, макс);       // слайдер с подписью
add.SLIDER(имя, подпись, число, мин, макс, шаг);  // слайдер с подписью
add.COLOR(имя);                     // выбор цвета, умолч. чёрный
add.COLOR(имя, число);              // выбор цвета
add.SUBMIT(текст);                  // кнопка отправки формы
add.LED_RED(имя);                   // красный светодиод-индикатор
add.LED_GREEN(имя);                 // зелёный светодиод-индикатор

// прочее для оформления
add.TITLE(текст);                   // заголовок
add.TITLE(текст, имя);              // + имя компонента (для update())
add.LABEL(текст);                   // подпись (для кнопок, полей, чекбоксов итд)
add.LABEL(текст, имя);              // + имя компонента (для update())
add.LABEL(число);                   // подпись, число
add.LABEL(число, имя);              // + имя компонента (для update())
add.AREA(имя, к-во строк, текст);   // большое поле ввода текста
add.AREA(имя, к-во строк);          // большое поле ввода текста
add.BREAK();                        // перенести строку
add.HR();                           // горизонтальный разделитель
add.BLOCK_BEGIN();                  // начать отрисовку блока
add.BLOCK_END();                    // завершить отрисовку блока

// прочее
add.AJAX_UPDATE(список, период);    // передать список обновления компонентов
// для списка можно использовать макрос PSTR

// большое поле для ввода текста
add.AREA(имя, к-во строк, текст);
add.AREA(имя, к-во строк);
add.AREA(имя);

// графики
// лёгкий статичный график без масштаба
add.PLOT<к-во осей, к-во данных>(имя, подписи, данные int16_t, int dec = 0)
add.PLOT_DARK<к-во осей, к-во данных>(имя, подписи, данные int16_t, int dec = 0)

// статический график с масштабом и привязкой ко времени
add.PLOT_STOCK<к-во осей, к-во данных>(имя, подписи, массив времён, массив данных, int dec = 0)
add.PLOT_STOCK_DARK<к-во осей, к-во данных>(имя, подписи, массив времён, массив данных, int dec = 0)

// динамический график, вызывает update
add.AJAX_PLOT(имя, к-во осей, к-во точек по Х, период update);
add.AJAX_PLOT_DARK(имя, к-во осей, к-во точек по Х, период update);

// создание кастомной страницы
GP_BUILD(строка);                   // запустить конструктор (можно вызывать где угодно)
GP_SHOW();                          // отобразить страницу (вызывать только внутри функции конструктора!)
add.PAGE_BEGIN();                   // начальный HTML код
add.PAGE_BLOCK_BEGIN()              // центральный div блок
add.PAGE_BLOCK_END()                // центральный div блок
add.PAGE_END();                     // завершающий HTML код
add.AJAX_CLICK();                   // обработчик кликов
```
</details>

<details>
<summary>Методы класса</summary>

```cpp
// система
void start();                       // запустить портал
void start(WIFI_AP);                // запустить портал с DNS сервером (для AP)
void stop();                        // остановить портал// показать свою страницу
void showPage(String&);             // показать свою страницу
void show();                        // вызвать конструктор и показать страницу

// подключение
void attachBuild(void*);            // подключить функцию-билдер страницы
void attachForm(void*);             // подключить функцию, которая вызывается при нажатии submit
void attachClick(void*);            // подключить функцию, которая вызывается при клике (кнопка, чекбокс, свитч, слайдер, селектор)
void attachUpdate(void*);           // подключить функцию, которая вызывается при AJAX обновлении со страницы

// опрос
bool tick();                        // тикер портала. Вернёт true, если портал запущен

bool form();                        // вернёт true, если было нажатие на любой submit
bool form(char* name);              // вернёт true, если был submit с указанной формы
String& formName();                 // получить имя теукщей submit формы

bool click();                       // вернёт true, если был клик по (кнопка, чекбокс, свитч, слайдер, селектор)
bool click(char* name);             // вернёт true, если был клик по указанному элементу
String& clickName();                // получить имя теукщего кликнутого компонента
const String& clickText();          // получить имя текст кликнутого компонента

bool update();                      // вернёт true, если было обновление
bool update(char* name);            // вернёт true, если было update с указанного компонента
String& updateName();               // вернёт имя обновлённого компонента

void answer(String& s);             // отправить ответ на обновление
void answer(int s);
void answer(char* s);
void answer(int16_t* v, int am);         // массив int размерностью am, для графика
void answer(int16_t* v, int am, int dec);// + делитель
void answer(GPcolor col);           // ответ с цветом
void answer(GPdate date);           // ответ с датой
void answer(GPtime time);           // ответ со временем

bool root();                        // вернёт true, если открыта главная страница (/)
String& uri();                      // адрес текущего запроса

// список автообновления
list.init(количество);              // инициализировать список, указать количество
list.clear();                       // очистить список
list.add(адрес, имя, тип);          // добавить переменную, указать имя компонента и тип
list.add(адрес, имя формы, имя, тип);   // добавить переменную, имя формы, указать имя компонента и тип

// типы для списка
T_CSTR      - массив char
T_STRING    - строка String
T_TIME      - время типа GPtime
T_DATE      - дата типа GPdate
T_CHECK     - boolean, для чекбокса
T_BYTE      - целое 1 байт
T_INT       - целое 4 байта
T_FLOAT     - float
T_COLOR     - целое 4 байта, для цвета

// парсеры
String getString(char* name);       // получить String строку с компонента name
char* getChars(char* name);         //получить char* строку с компонента name
void copyStr(char* name, char* dest);   // переписать char строку с компонента name к себе в dest
long getInt(char* name);            // получить целое число с компонента name
float getFloat(char* name);         // получить float число с компонента name
bool getCheck(char* name);          // получить состояние чекбокса с компонента name
GPdate getDate(char* name);         // получить дату с компонента name в формате GPdate
GPtime getTime(char* name);         // получить время с компонента name в формате GPtime
uint32_t getColor(char* name);      // получить цвет с компонента name
uint8_t getSelected(char* name, char* list);    // получить номер выбранного пункта в дроплисте list (может быть PSTR)
```
</details>

<details>
<summary>Хранение и изменение времени</summary>

```cpp
// структура для хранения даты
struct GPdate {
  int16_t year;
  uint8_t month, day;
};

// структура для хранения времени
struct GPtime {
  uint8_t hour, minute, second;
};

// получить unix время для графика
uint32_t GPunix(год, месяц, день, час, минута, секунда);
uint32_t GPunix(год, месяц, день, час, минута, секунда, gmt);
// gmt - часовой пояс, по умолч. 0 (пример: Москва gmt = 3)
// месяц и день начинаются с 1, не с 0!
```
</details>

<details>
<summary>Хранение и изменение цвета</summary>

```cpp
// см. пример gpcolor_demo

// структура для хранения цвета
struct GPcolor {
    uint8_t r, g, b;
};

// инициализация
GPcolor color;
GPcolor color(uint32_t color);
GPcolor color(byte r, byte g, byte b);

// к структуре могут быть применены методы
void setRGB(r, g, b);   // установить цвет побайтно
setHEX(uint32_t col);   // установить 24 бит цвет
uint32_t getHEX();      // получить 24 бит цвет

// к структуре можно присвоить uint32_t число
```
</details>

<details>
<summary>Утилиты</summary>

```cpp
char* splitList(char* str);             // разделить строку на подстроки. Цыганские фокусы
int8_t inList(char* name, char* list);  // получить номер, под которым name входит в list (вида "val1,val2,val3")
int8_t inList(String& name, char* list);

String encodeDate(GPdate& d);           // склеить дату в строку String
void encodeDate(char* str, GPdate& d);  // склеить дату в строку str[11]
String encodeDate(year, month, day);    // склеить дату в строку String
GPdate decodeDate(char* str);           // разобрать строковую дату[11] в структуру

String encodeTime(GPtime& t);           // склеить время в строку String
void encodeTime(char* str, GPtime& t);  // склеить время в строку str[9]
String encodeTime(hour, minute, second);// склеить время в строку String
GPtime decodeTime(char* str);           // разобрать строковое время[9] в структуру

String encodeColor(GPcolor color);      // собрать GPcolor цвет в String #rrggbb
String encodeColor(uint32_t color);     // собрать цвет в String #rrggbb
uint32_t decodeColor(char* hex);        // разобрать цвет #rrggbb в число

// добавить новое значение в массив с перемоткой (для графиков)
GPaddInt(int16_t val, int16_t* arr, uint8_t am);        // новое значение, массив, размер массива
GPaddUnix(uint32_t val, uint32_t* arr, uint8_t am);     // новое значение, массив, размер массива
GPaddUnixS(int16_t val, uint32_t* arr, uint8_t am);     // добавить секунды, массив, размер массива
```
</details>

### Таблица поддержки режимов работы компонентами

| Компонент/Вызов | form() | click() | update() |
| --------------- |:------:|:-------:|:--------:|
| TITLE           |        |         |    ✔    |
| LABEL           |        |         |    ✔    |
| BUTTON          |        |   ✔     |         |
| BUTTON_MINI     |        |   ✔     |         |
| NUMBER          |   ✔   |   ✔     |    ✔    |
| TEXT            |   ✔   |   ✔     |    ✔    |
| PASS            |   ✔   |   ✔     |    ✔    |
| AREA            |   ✔   |          |    ✔    |
| CHECK           |   ✔   |   ✔     |    ✔    |
| SWITCH          |   ✔   |   ✔     |    ✔    |
| DATE            |   ✔   |   ✔     |    ✔    |
| TIME            |   ✔   |   ✔     |    ✔    |
| SLIDER          |   ✔   |   ✔     |    ✔    |
| COLOR           |   ✔   |   ✔     |    ✔    |
| SELECT          |   ✔   |   ✔     |          |
| LED_RED         |   ✔   |          |    ✔    |
| LED_GREEN       |   ✔   |          |    ✔    |

<a id="usage"></a>
<a id="server"></a>
## 1. Сервер
Библиотека может работать как в локальной сети (esp подключается к роутеру), так и в режиме точки доступа (смартфон подключается к esp).
### 1.1 Подключение к роутеру
```cpp
  WiFi.mode(WIFI_STA);
  WiFi.begin("login", "pass");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  // ...
  portal.start();   // запускаем портал
```
Для подключения к порталу нужно зайти в браузере на IP адрес платы, который выдал ей роутер. В примере выше этот адрес выводится в монитор порта.

### 1.2 Создание точки
В этом режиме при запуске портала нужно передать **WIFI_AP** для запуска DNS сервера
```cpp
  WiFi.mode(WIFI_AP);
  WiFi.softAP("My Portal");
  portal.start(WIFI_AP);   // запускаем портал с настройкой на режим AP
```
На стандартных настройках IP адрес для подключения в этом режиме будет *192.168.4.1*

### 1.3 Тикер
В GyverPortal используется стандартная библиотека *ESP8266WebServer*, поэтому
для обеспечения работы сервера нужно вызывать `portal.tick()` в цикле программы. Возвращает `true`, если сервер запущен в данный момент.

### 1.4 Создаём GyverPortal глобально
```cpp
GyverPortal portal;
void build() {}

void setup() {
  // подключаемся к сети
  portal.attachBuild(build);
  portal.start();
}

void loop() {
  portal.tick();
  // опрос действий
}
```

### 1.5 Создаём GyverPortal локально
```cpp
void build() {}

void f() {
  GyverPortal portal;
  portal.attachBuild(build);
  portal.start();

  while (portal.tick()) {
    // опрос действий
  }
}
```
Для выхода из цикла можно вызвать `portal.stop()` по таймауту или сигналу с браузера.

<a id="build"></a>
## 2. Конструктор страниц
### 2.1. Создаём функцию конструктора
1. Создаём функцию вида: `void f()`. Далее в ней:
2. Создём пустую строку: `String s;`.
3. Запускаем конструктор: `BUILD_BEGIN(s)`. Передаём созданную строку. Здесь добавляется начальный HTML код.
4. (Опционально) применяем тему: `add.THEME(тема)`, GP_LIGHT/GP_DARK
5. Строим страницу, используя конструктор или прибавляя свои данные к строке.
6. Завершаем работу конструктора: `BUILD_END()`. Здесь добавляется завершающий HTML код и страница отправляется на сервер.

Шаблон функции конструктора:
```cpp
void build() {
  String s;
  BUILD_BEGIN(s);
  add.THEME(GP_LIGHT);
  // собираем страницу
  // ...
  BUILD_END();
}
```

### 2.2 Подключаем функцию конструктора
Передаём в библиотеку нашу функцию-конструктор страницы:
```cpp
portal.attachBuild(build);
```
Библиотека сама будет вызывать её, когда потребуется отобразить страницу. Функций-конструкторов (а следовательно и страниц) может быть несколько и их можно переключать.

<a id="form"></a>
## 3. Использование форм
### 3.1 Собираем страницу с формами
Основная суть использования форм:
- Форма имеет своё уникальное имя, должно начинаться с **/**
- Внутри формы может быть сколько угодно элементов, но только одна кнопка типа **SUBMIT**
- При нажатии на SUBMIT esp получает имя формы и данные из всех элементов внутри этой формы
- При нажатии на SUBMIT страница перезагружается, поэтому значения компонентов страницы нужно хранить в переменных и передавать при следующей сборке страницы

Пример с двумя формами, первая может передать текст из окна ввода, вторая - только факт нажатия кнопки:
```cpp
форма_1
    ввод текста
    кнопка submit
форма_1

форма_2
    кнопка submit
форма_2
```

В конструкторе GyverPortal это будет выглядеть так:
```cpp
void build() {
  String s;                     // создать строку
  BUILD_BEGIN(s);               // запустить конструктор
  add.THEME(GP_LIGHT);          // применить тему

  add.FORM_BEGIN("/login");     // начать форму, передать имя
  add.TEXT("txt", "Login", ""); // ввод текста, подсказка Login, текста нет
  add.BREAK();                  // перенос строки
  add.SUBMIT("Submit");         // кнопка Submit
  add.FORM_END();               // завершить форму

  add.FORM_BEGIN("/exit");      // начать форму, передать имя
  add.SUBMIT("Exit");           // кнопка Exit
  add.FORM_END();               // завершить форму

  BUILD_END();                  // завершить построение
}
```
Результат работы конструктора:  
![demo](/docs/demo.png)  
Все инструменты конструктора описаны в документации выше.

### 3.2 Опрос действий
- При нажатии любой кнопки типа *SUBMIT* в браузере функция `form()` вернёт `true`
- Функция должна опрашиваться после `tick()`
- Для поиска формы, с которой пришёл сигнал, используем `form(имя)` - вернёт `true`, если имя совпало
  - Лучше обернуть поиск в `if (form())`, чтобы не тратить процессорное время на сравнение строк
```cpp
portal.tick();
if (portal.form()) {
  Serial.print("Submit form: ");
  if (portal.form("/login")) Serial.println("Login");
  if (portal.form("/exit")) Serial.println("Exit");
}
```

### 3.3 Подключение обработчика
Вместо ручного опроса `form()` можно подключить свою функцию вида `void f(GyverPortal*)`, она будет вызвана при нажатии на любой SUBMIT.
Эту функцию нужно будет передать в `attachForm()`.
```cpp
void myAction(GyverPortal* p) {
  // имеем доступ к объекту портала, который отправил вызов
  if (p -> form("/exit")) Serial.println("exit");
}

void setup() {
  portal.attachForm(myAction);
}
```


### 3.4 Парсинг данных
В библиотеке реализованы готовые инструменты для полученя данных из компонентов формы (см. документацию выше). Например выведем в порт содержимое поля ввода текста:
```cpp
portal.tick();
if (portal.form("/login")) Serial.println(portal.getString("txt"));
// где "txt" - имя компонента
```

<a id="click"></a>
## 4. Использование кликов
### 4.1 Отличие от форм
В библиотеке реализован механизм, позволяющий обрабатывать действия на странице без её перезагрузки (как при использовании форм):
- Форма позволяет по нажатию одной кнопки получить значения с нескольких компонентов. Страница перезагрузится.
- Клик позволяет получить текущее (изменённое) значение только с кликнутого компонента. *Страница не перезагрузится*.

### 4.2 Опрос действий
- При клике по некоторым компонентам или изменении их значения (см. таблицу в документации) функция `click()` вернёт `true`
- Функция должна опрашиваться после `tick()`
- Для поиска компонента, с которого пришёл сигнал, используем `click(имя)` - вернёт `true`, если имя совпало
  - Лучше обернуть поиск в `if (click())`, чтобы не тратить процессорное время на сравнение строк
```cpp
portal.tick();
if (portal.click()) {
  if (portal.click("mybutton")) Serial.println("Click!");
}
```

### 4.3 Подключение обработчика
Вместо ручного опроса `click()` можно подключить свою функцию вида `void f(GyverPortal*)`, она будет вызвана при нажатиях на компоненты.
Эту функцию нужно будет передать в `attachClick()`.
```cpp
void myClick(GyverPortal* p) {
  // имеем доступ к объекту портала, который отправил вызов
  if (p -> click("mybutton")) Serial.println("Click!");
}

void setup() {
  portal.attachClick(myClick);
}
```

### 4.4 Парсинг данных
Парсинг данных от кликов можно производить при помощи тех же функций, что и для форм.

### 4.5 Подключение кнопки на другой компонент
Кнопку (BUTTON, BUTTON_MINI) можно "подключить" к другому компоненту: при клике по кнопке будет вызван сигнал `click` с именем кнопки и данными с указанного компонента. Для подключения нужно указать имя компонента третьим аргументом при добавлении кнопки:
```cpp
add.BUTTON(имя кнопки, текст кнопки, имя компонента);
add.BUTTON_MINI(имя кнопки, текст кнопки, имя компонента);
```
Пример, клик по кнопке отправляет текст из поля txt:
```cpp
add.TEXT("txt", "");
add.BUTTON_MINI("btn", "Send", "txt");
```

<a id="update"></a>
## 5. Использование обновлений
В библиотеке реализован механизм скриптовых запросов со страницы по таймеру. Это позволяет обновлять значения некоторых компонентов и надписей
(см. таблицу в документации) без обновления страницы в браузере.

### 5.1 Подключение обновлений
Для включения режима обновлений нужно добавить в начало страницы блок *AJAX_UPDATE*:
```cpp
void build() {
  String s;
  BUILD_BEGIN(s);
  add.AJAX_UPDATE("name1,name2,name3");
  // ...
  add.LABEL("NAN", "val");  // будем обновлять текст
  BUILD_END();
}
```

- Функция AJAX_UPDATE принимает список имён компонентов, разделённых запятой.
- ПРОБЕЛ ПОСЛЕ ЗАПЯТОЙ НЕ СТАВИМ.
- Также можно указать период запросов на обновления в миллисекундах `add.AJAX_UPDATE("name1,name2", 5000);`, по умолчанию - 1000 (1 секунда).
- Не все компоненты поддерживают режим обновлений (см. таблицу в документации).

### 5.2 Опрос обновлений
- При наступлении обновления функция `update()` вернёт `true`
- Функция должна опрашиваться после `tick()`
- Для поиска компонента, с которого пришёл сигнал, используем `update(имя)` - вернёт `true`, если имя совпало
  - Лучше обернуть поиск в `if (update())`, чтобы не тратить процессорное время на сравнение строк
- Нужно ответить на запрос обновления при помощи функции `answer()`. В неё передаётся актуальное значение для компонента
- Если не ответить на обновление до следующего вызова `tick()` - библиотека ответит пустым ответом, чтобы страница не зависла

```cpp
portal.tick();
if (portal.update()) {
  if (portal.update("val")) portal.answer(random(1000));
}
```

### 5.3 Подключение обработчика
Вместо ручного опроса `update()` можно подключить свою функцию вида `void f(GyverPortal*)`, она будет вызвана при обновлении любого компонента.
Эту функцию нужно будет передать в `attachUpdate()`.
```cpp
void myUpdate(GyverPortal* p) {
  if (p -> update("val")) p -> answer(random(1000));
}

void setup() {
  portal.attachUpdate(myUpdate);
}
```

<a id="auto"></a>
## 6. Автоматическое обновление переменных
**[См. примеры demoSubmitAuto и demoClickAuto]** Вместо ручного парсинга можно указать библиотеке переменные,
которые будут автоматически получать новые значения с указанных компонентов страницы. *Это работает как для форм, так и для кликов.*
- Инициализируем список, вызвав `.list.init(количество)`, передаём размер списка в количестве переменных.
- Добавляем переменную по её адресу:
  - `.list.add(&переменная, имя, тип)` - с указанием имени компонента и его типа
  - `.list.add(&переменная, форма, имя, тип)` - с указанием имени формы, имени компонента и типа

Указанные переменные обновят свои значения при действии с формы с указанным именем или при клике.
Если имя формы не указано - компонент будет парситься при действии с любой формы. *Для работы с кликами не нужно указывать имя формы.*

### 6.1 Поддержка фичи компонентами, связь с типами

| Тип данных    | Тип/Компонент | TEXT/NUMBER | PASS | CHECK | SWITCH | DATE | TIME | SLIDER | COLOR | SELECT | AREA |
| ------------- | ------------- |:-----------:|:----:|:-----:|:------:|:----:|:----:|:------:|:-----:|:------:|:----:|
| `char[]`      | T_CSTR        |  ✔         |  ✔   |       |        |  ✔  |  ✔  |   ✔   |   ✔   |   ✔   |      |
| `String`      | T_STRING      |  ✔         |  ✔   |       |        |  ✔  |  ✔  |   ✔   |   ✔   |   ✔   |      |
| `GPtime`      | T_TIME        |             |      |        |       |      |  ✔  |        |       |        |      |
| `GPdate`      | T_DATE        |             |      |        |       |  ✔  |      |        |       |        |      |
| `bool`        | T_CHECK       |             |      |  ✔    |   ✔   |      |      |        |       |        |      |
| `byte`, `char`| T_BYTE        |  ✔         |  ✔  |        |        |      |      |   ✔   |       |        |      |
| `int`, `long` | T_INT         |  ✔         |  ✔   |       |        |      |      |   ✔   |       |        |      |
| `float`       | T_FLOAT       |  ✔         |  ✔   |       |        |      |      |   ✔   |       |        |      |
| `uint32_t`    | T_COLOR       |             |      |       |        |      |      |        |   ✔   |        |      |

<a id="plots"></a>
## 7. Графики
### 7.1 Общие особенности
#### Совместимость
Графики **AJAX_PLOT** и **PLOT_STOCK** несовместимы в одном интерфейсе!

#### Вывод дробных данных
У всех трёх типов графиков есть аргумент `dec`, по умолчанию равен 0. Это делитель, на который (если отличен от 0) будут делиться значения точек графика и переводиться в `float`. 
Таким образом можно отображать данные с плавающей точкой и не хранить в памяти лишние 2 байта. Получили температуру 22.5 градусов, умножаем на 10 и сохраняем в массив. Вызываем график с `dec`, равным 10.  

#### Несколько осей
Все графики поддерживают вывод по нескольким осям (общая ось X).

#### Подписи
Подписи храним в массиве char, например так:
```cpp
const char *names[] = {"kek", "puk",};
```
#### Обновление статических графиков
Статические графики отображают данные при перезагрузке страницы. Таким образом в конструктор должен быть передан массив с актуальными значениями.  
В библиотеке реализованы функции для удобного добавления нового значения к массиву (с автоматической "перемоткой"):
```cpp
GPaddInt(int16_t val, int16_t* arr, uint8_t am);        // новое значение, массив, размер массива
GPaddUnix(uint32_t val, uint32_t* arr, uint8_t am);     // новое значение, массив, размер массива
GPaddUnixS(int16_t val, uint32_t* arr, uint8_t am);     // добавить секунды, массив, размер массива
```

Например, есть массив `int arr[2][20]` - хранит 20 значений для двух осей графика. Можно обновлять его и хранить в EEPROM, обеспечивая бесперерывную работу. Для добавления нового значения делаем по своему таймеру:
```cpp
GPaddInt(новое, arr[0], 20);
GPaddInt(новое, arr[1], 20);
```
В конструктор передаём как
```cpp
add.PLOT<2, 20>("table", names, arr);
```

#### Обновление динамических графиков
Динамический график вызывает `update`, отвечаем ему новыми значениями и он строит график в реальном времени. Для передачи значений по нескольким осям используем
`answer(массив, размер)` или `answer(массив, размер, dec)`, где dec имеет смысл делителя (см. выше).


### 7.2 График PLOT
Лёгкий статический график без масштаба  
**[См. пример staticPlot]**  
![demo](/docs/staticPlot.png)  
```cpp
add.PLOT<к-во осей, к-во данных>(имя, подписи, данные int16_t, int dec = 0)
add.PLOT_DARK<к-во осей, к-во данных>(имя, подписи, данные int16_t, int dec = 0)
```

### 7.3 График PLOT_STOCK
Статический график с масштабом и привязкой ко времени  
**[См. пример stockPlot]**  
![demo](/docs/stockPlot.png)  
```cpp
add.PLOT_STOCK<к-во осей, к-во данных>(имя, подписи, массив времён, массив данных, int dec = 0)
add.PLOT_STOCK_DARK<к-во осей, к-во данных>(имя, подписи, массив времён, массив данных, int dec = 0)
```
Данный график требует для отображения массив даты и времени типа `uint32_t`, содержащий время в формате unix.

### 7.4 График AJAX_PLOT
Динамический график, вызывает update по своему имени, требует ответа  
**[См. пример ajaxPlot]**  
![demo](/docs/ajaxPlot.png)  

```cpp
add.AJAX_PLOT(имя, к-во осей, к-во точек по Х, период update);
add.AJAX_PLOT_DARK(имя, к-во осей, к-во точек по Х, период update);
```

<a id="log"></a>
## 8. Лог
В библиотеке реализована возможность делать `print()` в специальное окно лога на странице:
- Окно лога можно создать только одно
- Обновление происходит автоматически, раз в секунду
- Страница не обновляется
- Можно отправлять любые данные, как Serial

### 8.1 Подключение окна лога
Добавляем `add.AREA_LOG(к-во строк)` в нужное место страницы

### 8.2 Запуск лога
Вызываем `log.start(размер буфера)`. Размер буфера по умолчанию 64 символа
- Примечание: это размер буфера *на стороне библиотеки*, то есть ограничение на количество символов на одну отправку на страницу (раз в секунду). У страницы браузера свой буфер для отображения текста!

### 8.3 Вывод данных
Просто вызываем `log.print()` или `log.println()` как у обычного Serial. См. пример **demoLog**.

<a id="api"></a>
## 9. Свои компоненты, API
### 9.1 Кастомный конструктор
Конструктор GyverPortal ничем не ограничивает построение страницы: достаточно прибавить к строке любой HTML код между запуском `GP_BUILD(s)` и завершением конструктора `GP_SHOW()`:
```cpp
void build() {
  String s;
  GP_BUILD(s);
  // собираем страницу
  // ...
  GP_SHOW();
}
```

Для справки:  
Стандартный `BUILD_BEGIN(String)` внутри состоит из:
```cpp
  GP_BUILD(s);
  add.PAGE_BEGIN();
  add.AJAX_CLICK();
  add.PAGE_BLOCK_BEGIN();
```

Стандартный `BUILD_END()` внутри состоит из:
```cpp
 add.PAGE_BLOCK_END();
 add.PAGE_END();
 GP_SHOW();
```

### 9.2 Свой код
Достаточно прибавить любой HTML код к строке, например:
```cpp
s += F("<input type=\"email\" class=\"myClass\">");
```
Можно обернуть в F macro, чтобы не занимать оперативку.

### 9.3 API
Для обеспечения работоспособности механизмов библиотеки в кастомных компонентах нужно соблюдать следующие моменты:
- Если нужна поддержка кликов - добавить в страницу `add.AJAX_CLICK()`
- У компонентов формы должен быть указан атрибут *name* для передачи данных через submit.
- У кликабельных компонентов должен быть указан атрибут *onclick* с параметром-функцией: `onclick="GP_click(this)"`. Библиотека сама перехватит вызов и направит в `click()`.
- У компонентов, с которых нужен сигнал `click()` по изменению данных, должен быть указан атрибут *onchange* с параметром-функцией: `onchange="GP_click(this)"`. Библиотека сама перехватит вызов и направит в `click()`.
- У компонентов, для которых нужны обновления `update()`, должен быть указан атрибут `id`. Его значение также нужно передать в `add.AJAX_UPDATE()`.
- Если нужен клик, который передаёт данные с другого компонента, указываем атрибут с функцией `onclick="GP_clickid(btn,tar)"`, где `btn` - имя (для библиотеки) кликающего компонента, а `tar` - атрибут `id` целевого компонента, с которого нужно передать данные.
- Для ручной передачи в библиотеку сигнала о клике нужно отправить http POST запрос вида `GP_click?имя=значение`
- Для ручной передачи в библиотеку сигнала об обновлении нужно отправить http GET запрос вида `GP_update?id_компонента`

<a id="example"></a>
## Примеры
```cpp
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - улучшил графики и стили
- v1.2
    - Блок NUMBER теперь тип number
    - Добавил большое текстовое поле AREA
    - Добавил GPunix
    - Улучшил парсинг
    - Добавил BUTTON_MINI
    - Кнопки могут передавать данные с других компонентов (кроме AREA и чекбоксов)
    - Добавил PLOT_STOCK - статический график с масштабом
    - Добавил AJAX_PLOT_DARK
    - Изменён синтаксис у старых графиков
    - Фичи GPaddUnix и GPaddInt для графиков
    - Убрал default тему
    - Подкрутил стили
    - Добавил окно лога AREA_LOG и функцию лога в целом
- v1.3 - переделал GPunix, мелкие фиксы, для списков можно использовать PSTR
- v1.4 - мелкие фиксы, клик по COLOR теперь отправляет цвет
- v1.5 - добавил блок "слайдер+подпись"
- v1.5.1 - мелкий фикс копирования строк
- v1.5.2 - добавлен *meta charset="utf-8"*, английский README (спасибо VerZsuT)
- v1.6 - добавлены инструменты для работы c цветом. Добавил answer() для даты, времени и цвета
- v1.7 - поддержка ESP32

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
Библиотека открыта для доработки и ваших **Pull Request**'ов!
