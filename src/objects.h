#pragma once

// GP Objects

#include "utils.h"

#define EMPTY_INT   INT32_MAX
#define EMPTY_FLOAT NAN

namespace GP {

String* tmpPageBuf;

struct Obj {
    void send() { }
};

struct AREA : Obj {
    AREA(const char* name = nullptr, int r = 1, const String& text = {}, const String& wrap = {}, bool disabled = false)
        : name{name}
        , rows{r}
        , text{text}
        , width{wrap}
        , disabled{disabled} { }
    const char* name;
    int rows;
    String text;
    String width;
    bool disabled;

    void obj(const String& name, int rows = 1, const String& value = "", const String& width = "", bool dis = false) {
        *tmpPageBuf += F("<textarea onchange='GP_click(this)' style='height:auto' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' rows='");
        *tmpPageBuf += rows;
        if (width.length()) {
            *tmpPageBuf += F("' style='width:");
            *tmpPageBuf += width;
        }
        *tmpPageBuf += "'";
        if (dis) *tmpPageBuf += F(" disabled");
        *tmpPageBuf += ">";
        if (value.length()) *tmpPageBuf += value;
        *tmpPageBuf += F("</textarea>\n");
        send();
    }
};

struct BUTTON : Obj {
    BUTTON(const char* name = nullptr, const String& text = {}, const String& target = {}, PGM_P style = GREEN, const String& width = {}, bool disabled = 0, bool reload = 0)
        : name{name}
        , text{text}
        , target{target}
        , style{style}
        , width{width}
        , disabled{disabled}
        , reload{reload} { }
    const char* name;
    String text;
    String target;
    PGM_P style;
    String width;
    bool disabled;
    bool reload;
};

struct BUTTON_MINI : Obj {
    BUTTON_MINI(const char* name = nullptr, const String& text = {}, const String& target = {}, PGM_P style = GREEN, const String& width = {}, bool disabled = 0, bool reload = 0)
        : name{name}
        , text{text}
        , target{target}
        , style{style}
        , width{width}
        , disabled{disabled}
        , reload{reload} { }
    const char* name;
    String text;
    String target;
    PGM_P style;
    String width;
    bool disabled;
    bool reload;
};

struct CHECK : Obj {
    CHECK(const char* name = nullptr, bool sta = 0, PGM_P style = GREEN, bool disabled = false)
        : name{name}
        , state{sta}
        , style{style}
        , disabled{disabled} { }
    const char* name;
    bool state;
    PGM_P style;
    bool disabled;

    void obj(const String& name, bool state = 0, PGM_P st = GREEN, bool dis = false) {
        if (st != GREEN) {
            *tmpPageBuf += F("<style>#__");
            *tmpPageBuf += name;
            *tmpPageBuf += F(" input:checked+span::before{border-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F(";background-color:");
            *tmpPageBuf += FPSTR(st);
            *tmpPageBuf += F("}</style>\n");
        }
        *tmpPageBuf += F("<label id='__");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' class='check_c'><input type='checkbox' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += "' ";
        if (state) *tmpPageBuf += F("checked ");
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onclick='GP_click(this)'><span></span></label>\n"
                         "<input type='hidden' value='0' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += "'>\n";
        send();
    }
};

struct COLOR : Obj {
    COLOR(const char* name = nullptr)
        : name{name} { }
    COLOR(const char* name, GPcolor c, bool disabled = false)
        : name{name}
        , color{c}
        , disabled{disabled} { }
    const char* name;
    GPcolor color;
    bool disabled = false;

    void obj(const String& name, uint32_t value = 0, bool dis = false) {
        *tmpPageBuf += F("<input type='color' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        GPcolor col(value);
        *tmpPageBuf += col.encode();
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this)'>\n");
        send();
    }

    void obj(const String& name, GPcolor col, bool dis = false) {
        *tmpPageBuf += F("<input type='color' name='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' id='");
        *tmpPageBuf += name;
        *tmpPageBuf += F("' value='");
        *tmpPageBuf += col.encode();
        *tmpPageBuf += "' ";
        if (dis) *tmpPageBuf += F("disabled ");
        *tmpPageBuf += F("onchange='GP_click(this)'>\n");
        send();
    }
};

struct DATE : Obj {
    DATE(const char* name = nullptr)
        : name{name} { }
    DATE(const char* name, GP::Date d, bool disabled = false)
        : name{name}
        , date{d}
        , disabled{disabled} { }
    const char* name;
    GP::Date date;
    bool disabled = false;
};

struct HasText : Obj { };

struct LABEL : HasText {
    LABEL(const char* name = nullptr, const String& text = {}, PGM_P style = __DEFAULT__, int size = 0, bool bold = 0, bool wrap = 0)
        : name{name}
        , text{text}
        , style{style}
        , size{size}
        , bold{bold}
        , wrap{wrap} { }
    const char* name;
    String text;
    PGM_P style;
    int size;
    bool bold;
    bool wrap;
};

struct LABEL_BLOCK : Obj {
    LABEL_BLOCK(const char* name = nullptr, const String& text = {}, PGM_P style = GREEN, int size = 0, bool bold = 0)
        : name{name}
        , text{text}
        , style{style}
        , size{size}
        , bold{bold} { }
    const char* name;
    String text;
    PGM_P style;
    int size;
    bool bold;
};

struct LED : Obj {
    LED(const char* name = nullptr, bool state = 0)
        : name{name}
        , state{state} { }
    const char* name;
    bool state;
};

struct LED_GREEN : Obj {
    LED_GREEN(const char* name = nullptr, bool state = 0)
        : name{name}
        , state{state} { }
    const char* name;
    bool state;
};

struct LED_RED : Obj {
    LED_RED(const char* name = nullptr, bool state = 0)
        : name{name}
        , state{state} { }
    const char* name;
    bool state;
};

struct NUMBER : Obj {
    NUMBER(const char* name = nullptr, const String& placeholder = {}, int value = EMPTY_INT, const String& wrap = {}, bool disabled = false)
        : name{name}
        , placeholder{placeholder}
        , value{value}
        , width{wrap}
        , disabled{disabled} { }
    const char* name;
    String placeholder;
    int value;
    String width;
    String pattern;
    String min = {};
    String max = {};
    bool disabled;
};

struct NUMBER_F : Obj {
    NUMBER_F(const char* name = nullptr, const String& placeholder = {}, float value = EMPTY_FLOAT, uint8_t decimals = 2, const String& wrap = {}, bool disabled = false)
        : name{name}
        , placeholder{placeholder}
        , value{value}
        , decimals{decimals}
        , width{wrap}
        , disabled{disabled} { }
    const char* name;
    String placeholder;
    float value;
    uint8_t decimals;
    String width;
    String pattern;
    String min = {};
    String max = {};
    bool disabled;
};

struct PASS : Obj {
    PASS(const char* name = nullptr, const String& placeholder = {}, const String& txt = {}, const String& wrap = {}, int maxlen = 0, const String& pattern = {}, bool disabled = false, bool e = false)
        : name{name}
        , placeholder{placeholder}
        , text{txt}
        , width{wrap}
        , maxlen{maxlen}
        , pattern{pattern}
        , disabled{disabled}
        , eye{e} { }
    const char* name;
    String placeholder;
    String text;
    String width;
    int maxlen;
    String pattern;
    bool disabled;
    bool eye;
};

struct RADIO : Obj {
    RADIO(const char* name = nullptr, int num = 0, int value = -1, PGM_P style = __DEFAULT__, bool disabled = 0)
        : name{name}
        , num{num}
        , value{value}
        , style{style}
        , disabled{disabled} { }
    const char* name;
    int num;
    int value;
    PGM_P style;
    bool disabled;
};

struct SELECT : Obj {
    SELECT(const char* name = nullptr, const String& list = {}, int selected = 0, bool numbers = 0, bool disabled = false, bool reload = 0)
        : name{name}
        , list{list}
        , selected{selected}
        , numbers{numbers}
        , disabled{disabled}
        , reload{reload} { }
    const char* name;
    String list;
    int selected;
    bool numbers;
    bool disabled;
    bool reload;
    String getValue() { return listIdx(list, selected); }
};

struct SLIDER : Obj {
    SLIDER(const char* name = nullptr, float value = 0, float min = 0, float max = 10, float step = 1, uint8_t decimals = 0, PGM_P style = GREEN, bool disabled = false, bool oninput = 0)
        : name{name}
        , value{value}
        , min{min}
        , max{max}
        , step{step}
        , decimals{decimals}
        , style{style}
        , disabled{disabled}
        , oninput{oninput} { }
    const char* name;
    float value = 0;
    float min = 0;
    float max = 10;
    float step = 1;
    uint8_t decimals;
    PGM_P style;
    bool disabled;
    bool oninput;
};

struct SPINNER : Obj {
    SPINNER(const char* name = nullptr, float value = 0, float min = NAN, float max = NAN, float step = 1, uint8_t decimals = 0, PGM_P style = GREEN, const String& wrap = {}, bool disabled = 0)
        : name{name}
        , value{value}
        , min{min}
        , max{max}
        , step{step}
        , decimals{decimals}
        , style{style}
        , width{wrap}
        , disabled{disabled} { }
    const char* name;
    float value;
    float min;
    float max;
    float step;
    uint8_t decimals;
    PGM_P style;
    String width;
    bool disabled;
};

struct SWITCH : Obj {
    SWITCH(const char* name = nullptr, bool sta = 0, PGM_P style = GREEN, bool disabled = false)
        : name{name}
        , state{sta}
        , style{style}
        , disabled{disabled} { }
    const char* name;
    bool state;
    PGM_P style;
    bool disabled;
};

struct TD {
} : Obj;

struct TEXT : Obj {
    TEXT(const char* name = nullptr, const String& placeholder = {}, const String& txt = {}, const String& wrap = {}, int maxlen = 0, const String& pattern = {}, bool disabled = false)
        : name{name}
        , placeholder{placeholder}
        , text{txt}
        , width{wrap}
        , maxlen{maxlen}
        , pattern{pattern}
        , disabled{disabled} { }
    const char* name;
    String placeholder;
    String text;
    String width;
    int maxlen;
    String pattern;
    bool disabled;
};

struct TIME : Obj {
    TIME(const char* name = nullptr)
        : name{name} { }
    TIME(const char* name, GP::Time text, bool disabled = false)
        : name{name}
        , time{text}
        , disabled{disabled} { }
    const char* name;
    GP::Time time;
    bool disabled = false;
};

struct TITLE : HasText : Obj {
    TITLE(const char* name = nullptr, const String& text = {}, PGM_P style = __DEFAULT__, int size = 0, bool bold = 0)
        : name{name}
        , text{text}
        , style{style}
        , size{size}
        , bold{bold} { }
    const char* name;
    String text;
    PGM_P style;
    int size;
    bool bold;
};

struct TR {
} : Obj;

} // namespace GP
