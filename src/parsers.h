#pragma once

// GP Parsers

#include "objects.h"
#include "utils.h"
#include <Arduino.h>

namespace GP {

class ArgParser {
public:
    virtual const String arg(const String& n) = 0; // value from name
    virtual int args() = 0;                        // amount
    virtual bool hasArg(const String& n) = 0;      // check
    virtual bool hasClick() = 0;

    const String& arg() { // value from 0
        return _argValPtr ? (*_argValPtr) : __empty_str__;
    }
    const String& argName() { // name from 0
        return _argNamePtr ? (*_argNamePtr) : __empty_str__;
    }

    // ==================== HOLD ===================
    // вернёт true, если статус удержания кнопки изменился (нажата/отпущена)
    bool hold() {
        return _holdF && args();
    }

    // вернёт true, если кнопка удерживается
    bool hold(const String& name) {
        return _hold.length() ? _hold.equals(name) : 0;
    }

    // вернёт имя удерживаемой кнопки
    String holdName() {
        return _hold.length() ? _hold : __empty_str__;
    }

    // вернёт часть имени hold компонента, находящейся под номером idx после разделителя /
    String holdNameSub(int idx = 1) {
        return _hold.length() ? (listIdx(_hold, idx, '/')) : __empty_str__;
    }

    // вернёт true, если кнопка удерживается и имя компонента начинается с указанного
    bool holdSub(const String& name) {
        return _hold.length() ? _hold.startsWith(name) : 0;
    }

    // вернёт true, если кнопка была нажата
    bool clickDown(const String& name) {
        return hold() ? (_holdF == 1 && argName().equals(name)) : 0;
    }
    // вернёт true, если кнопка была нажата и имя компонента начинается с указанного
    bool clickDownSub(const String& name) {
        return hold() ? (_holdF == 1 && argName().startsWith(name)) : 0;
    }

    // вернёт true, если кнопка была отпущена
    bool clickUp(const String& name) {
        return hold() ? (_holdF == 2 && argName().equals(name)) : 0;
    }
    // вернёт true, если кнопка была отпущена и имя компонента начинается с указанного
    bool clickUpSub(const String& name) {
        return hold() ? (_holdF == 2 && argName().startsWith(name)) : 0;
    }

    // ===================== CLICK OBJ ======================
    bool clickDown(GP::BUTTON& btn) {
        return clickDown(btn.name);
    }

    bool clickUp(GP::BUTTON& btn) {
        return clickUp(btn.name);
    }

    bool clickDown(GP::BUTTON_MINI& btn) {
        return clickDown(btn.name);
    }

    bool clickUp(GP::BUTTON_MINI& btn) {
        return clickUp(btn.name);
    }

    // ===================== CLICK =====================
    // вернёт true, если был клик по (кнопка, чекбокс, свитч, слайдер, селектор)
    bool click() {
        return hasClick() && args();
    }

    // вернёт true, если был клик по указанному элементу (кнопка, чекбокс, свитч, слайдер, селектор)
    bool click(const String& name) {
        return click() ? (argName().equals(name) && args() == 1) : 0;
    }

    // вернёт true, если имя кликнутого компонента начинается с name
    bool clickSub(const String& name) {
        return click() ? (argName().startsWith(name) && args() == 1) : 0;
    }

    // вернёт имя теукщего кликнутого компонента
    String clickName() {
        return click() ? argName() : __empty_str__;
    }

    // вернёт часть имени кликнутого компонента, находящейся под номером idx после разделителя /
    String clickNameSub(int idx = 1) {
        return click() ? (listIdx(argName(), idx, '/')) : __empty_str__;
    }

    // ===================== CLICK AUTO =====================
    // нулевой аргумент (для вызова в условии)
    bool clickStr(char* t, uint16_t len = 0) {
        return click() ? copyStr(t, len) : 0;
    }
    bool clickString(String& t) {
        return click() ? copyString(t) : 0;
    }
    template <typename T>
    bool clickInt(T& t) {
        return click() ? copyInt(t) : 0;
    }
    template <typename T>
    bool clickFloat(T& t) {
        return click() ? copyFloat(t) : 0;
    }
    template <typename T>
    bool clickBool(T& t) {
        return click() ? copyBool(t) : 0;
    }
    bool clickDate(GPdate& t) {
        return click() ? copyDate(t) : 0;
    }
    bool clickTime(GPtime& t) {
        return click() ? copyTime(t) : 0;
    }
    bool clickWeek(GPweek& t) {
        return click() ? copyWeek(t) : 0;
    }
    bool clickFlags(GPflags& t) {
        return click() ? copyFlags(t) : 0;
    }
    bool clickColor(GPcolor& t) {
        return click() ? copyColor(t) : 0;
    }

    // с указанием имени компонента
    bool clickStr(const String& n, char* t, uint16_t len = 0) {
        return click() ? copyStr(n, t, len) : 0;
    }
    bool clickString(const String& n, String& t) {
        return click() ? copyString(n, t) : 0;
    }
    template <typename T>
    bool clickInt(const String& n, T& t) {
        return click() ? copyInt(n, t) : 0;
    }
    template <typename T>
    bool clickFloat(const String& n, T& t) {
        return click() ? copyFloat(n, t) : 0;
    }
    template <typename T>
    bool clickBool(const String& n, T& t) {
        return click() ? copyBool(n, t) : 0;
    }
    bool clickDate(const String& n, GPdate& t) {
        return click() ? copyDate(n, t) : 0;
    }
    bool clickTime(const String& n, GPtime& t) {
        return click() ? copyTime(n, t) : 0;
    }
    bool clickWeek(const String& n, GPweek& t) {
        return click() ? copyWeek(n, t) : 0;
    }
    bool clickFlags(const String& n, GPflags& t) {
        return click() ? copyFlags(n, t) : 0;
    }
    bool clickColor(const String& n, GPcolor& t) {
        return click() ? copyColor(n, t) : 0;
    }

    // ======================= ПАРСЕРЫ =======================
    // ОПАСНЫЕ ФУНКЦИИ (не проверяют есть ли запрос). Конвертируют и возвращают значение
    // получить String строку с компонента
    //    template <typename T>
    //    T get(const String& n) {
    //        if constexpr (is_same_v<T, GPcolor>) return {};
    //        if constexpr (is_same_v<T, GPdate>) return {};
    //        if constexpr (is_same_v<T, GPtime>) return {};
    //        if constexpr (is_same_v<T, String>) return arg(n);
    //        if constexpr (is_same_v<T, bool>) return {};
    //        if constexpr (is_same_v<T, char*>) return {};
    //        if constexpr (is_same_v<T, float>) return {};
    //        if constexpr (is_same_v<T, int8_t>) return {};
    //        if constexpr (is_same_v<T, long>) return {};
    //    }

    String getString(const String& n) {
        return arg(n);
    }
    String getString() {
        return arg();
    }

    // получить число с компонента
    int getInt(const String& n) {
        return getIntUniv(arg(n));
    }
    int getInt() {
        return getIntUniv(arg());
    }

    // получить float с компонента
    float getFloat(const String& n) {
        return arg(n).toFloat();
    }
    float getFloat() {
        return arg().toFloat();
    }

    // получить состояние чекбокса
    bool getBool(const String& n) {
        String s = arg(n);
        return (s[0] == '1' || s[0] == 'o' || s[0] == 't');
    }
    bool getBool() {
        return (arg()[0] == '1' || arg()[0] == 'o' || arg()[0] == 't');
    }

    // получить дату с компонента
    GPdate getDate(const String& n) {
        return GPdate(arg(n));
    }
    GPdate getDate() {
        return GPdate(arg());
    }

    // получить время с компонента
    GPtime getTime(const String& n) {
        return GPtime(arg(n));
    }
    GPtime getTime() {
        return GPtime(arg());
    }

    // получить неделю с компонента
    GPweek getWeek(const String& n) {
        return GPweek(arg(n));
    }
    GPweek getWeek() {
        return GPweek(arg());
    }

    // получить флаги с компонента
    GPflags getFlags(const String& n) {
        return GPflags(arg(n));
    }
    GPflags getFlags() {
        return GPflags(arg());
    }

    // получить цвет с компонента
    GPcolor getColor(const String& n) {
        return GPcolor(arg(n));
    }
    GPcolor getColor() {
        return GPcolor(arg());
    }

    // ===================== COPY-ПАРСЕРЫ =====================
    // ОПАСНЫЕ парсеры (не проверяют запрос). Использовать только в условии!
    bool copyStr(char* t, uint16_t len = 0) {
        // return (args() && (!len || arg().length() < len)) ? (strcpy(t, arg().c_str()), 1) : 0;
        if (!args()) return 0;
        if (!len || arg().length() < len) strcpy(t, arg().c_str());
        else {
            strncpy(t, arg().c_str(), len - 1);
            t[len - 1] = '\0';
        }
        return 1;
    }
    bool copyString(String& t) {
        return args() ? (t = arg(), 1) : 0;
    }
    template <typename T>
    bool copyInt(T& t) {
        return args() ? (t = getInt(), 1) : 0;
    }
    template <typename T>
    bool copyFloat(T& t) {
        return args() ? (t = getFloat(), 1) : 0;
    }
    template <typename T>
    bool copyBool(T& t) {
        return args() ? (t = getBool(), 1) : 0;
    }
    bool copyDate(GPdate& t) {
        return args() ? (t = getDate(), 1) : 0;
    }
    bool copyTime(GPtime& t) {
        return args() ? (t = getTime(), 1) : 0;
    }
    bool copyWeek(GPweek& t) {
        return args() ? (t = getWeek(), 1) : 0;
    }
    bool copyFlags(GPflags& t) {
        return args() ? (t = getFlags(), 1) : 0;
    }
    bool copyColor(GPcolor& t) {
        return args() ? (t = getColor(), 1) : 0;
    }

    // БЕЗОПАСНЫЕ парсеры (проверяют запрос). Копируют данные из запроса в переменную
    bool copyStr(const String& n, char* t, uint16_t len = 0) {
        return (hasArg(n) && (!len || arg(n).length() < len)) ? (strcpy(t, arg(n).c_str()), 1) : 0;
    }
    bool copyString(const String& n, String& t) {
        return hasArg(n) ? (t = arg(n), 1) : 0;
    }
    template <typename T>
    bool copyInt(const String& n, T& t) {
        return hasArg(n) ? (t = getInt(n), 1) : 0;
    }
    template <typename T>
    bool copyFloat(const String& n, T& t) {
        return hasArg(n) ? (t = getFloat(n), 1) : 0;
    }
    template <typename T>
    bool copyBool(const String& n, T& t) {
        return hasArg(n) ? (t = getBool(n), 1) : 0;
    }
    bool copyDate(const String& n, GPdate& t) {
        return hasArg(n) ? (t = getDate(n), 1) : 0;
    }
    bool copyTime(const String& n, GPtime& t) {
        return hasArg(n) ? (t = getTime(n), 1) : 0;
    }
    bool copyWeek(const String& n, GPweek& t) {
        return hasArg(n) ? (t = getWeek(n), 1) : 0;
    }
    bool copyFlags(const String& n, GPflags& t) {
        return hasArg(n) ? (t = getFlags(n), 1) : 0;
    }
    bool copyColor(const String& n, GPcolor& t) {
        return hasArg(n) ? (t = getColor(n), 1) : 0;
    }

    // ===================== COPY OBJ =====================
    bool copy(GP::NUMBER& num) {
        return copyInt(num.name, num.value);
    }
    bool copy(GP::NUMBER_F& num) {
        return copyFloat(num.name, num.value);
    }

    bool copy(GP::TEXT& txt) {
        return copyString(txt.name, txt.text);
    }
    bool copy(GP::PASS& pas) {
        return copyString(pas.name, pas.text);
    }

    bool copy(GP::AREA& ar) {
        return copyString(ar.name, ar.text);
    }

    bool copy(GP::CHECK& ch) {
        return copyBool(ch.name, ch.state);
    }
    bool copy(GP::SWITCH& sw) {
        return copyBool(sw.name, sw.state);
    }

    bool copy(GP::DATE& d) {
        return copyDate(d.name, d.date);
    }
    bool copy(GP::TIME& t) {
        return copyTime(t.name, t.time);
    }
    bool copy(GP::COLOR& c) {
        return copyColor(c.name, c.color);
    }

    bool copy(GP::SPINNER& s) {
        return copyFloat(s.name, s.value);
    }
    bool copy(GP::SLIDER& s) {
        return copyFloat(s.name, s.value);
    }

    bool copy(GP::SELECT& s) {
        return copyInt(s.name, s.selected);
    }

    // ===================== CLICK OBJ =====================
    bool click(GP::BUTTON& btn) {
        return click(btn.name);
    }
    bool click(GP::BUTTON_MINI& btn) {
        return click(btn.name);
    }

    bool click(GP::NUMBER& num) {
        return click() ? copy(num) : 0;
    }
    bool click(GP::NUMBER_F& num) {
        return click() ? copy(num) : 0;
    }

    bool click(GP::TEXT& txt) {
        return click() ? copy(txt) : 0;
    }
    bool click(GP::PASS& pas) {
        return click() ? copy(pas) : 0;
    }

    bool click(GP::AREA& ar) {
        return click() ? copy(ar) : 0;
    }

    bool click(GP::CHECK& ch) {
        return click() ? copy(ch) : 0;
    }
    bool click(GP::SWITCH& sw) {
        return click() ? copy(sw) : 0;
    }

    bool click(GP::DATE& d) {
        return click() ? copy(d) : 0;
    }
    bool click(GP::TIME& t) {
        return click() ? copy(t) : 0;
    }
    bool click(GP::COLOR& c) {
        return click() ? copy(c) : 0;
    }

    bool click(GP::SPINNER& s) {
        return click() ? copy(s) : 0;
    }
    bool click(GP::SLIDER& s) {
        return click() ? copy(s) : 0;
    }

    bool click(GP::SELECT& s) {
        return click() ? copy(s) : 0;
    }

    // отправить ответ на обновление
    bool answer(const String& s) {
        if (_answPtr) *_answPtr += s;
        return (bool)_answPtr;
    }
    bool answer(int v) {
        if (_answPtr) *_answPtr += v;
        return (bool)_answPtr;
    }

    // для float/double
    template <typename T>
    bool answer(T v, uint8_t dec) {
        return answer(String(v, (uint16_t)dec));
    }
    bool answer(int* v, int am, int dec = 0) {
        String s;
        s.reserve(am * 4);
        for (int i = 0; i < am; i++) {
            if (dec) s += (float)v[i] / dec;
            else s += v[i];
            if (i != am - 1) s += ',';
        }
        return answer(s);
    }

    bool answer(GPcolor col) {
        return answer(col.encode());
    }
    bool answer(GPdate date) {
        return answer(date.encode());
    }
    bool answer(GPtime time) {
        return answer(time.encode());
    }
    bool answer(GPweek week) {
        return answer(week.encode());
    }
    bool answer(GPflags flags) {
        return answer(flags.encode());
    }
    bool answer(Canvas& cv) {
        return answer(cv._read());
    }

    // ==================== UPDATE AUTO =====================
    // автоматическое обновление. Отправит значение из указанной переменной
    // Вернёт true в момент обновления
    bool updateString(const String& n, String& f) {
        return update(n) ? (answer(f), 1) : 0;
    }
    template <typename T>
    bool updateInt(const String& n, T f) {
        return update(n) ? (answer((int)f), 1) : 0;
    }
    template <typename T>
    bool updateFloat(const String& n, T f, int dec = 2) {
        return update(n) ? (answer(f, dec), 1) : 0;
    }
    bool updateBool(const String& n, bool f) {
        return update(n) ? (answer(f), 1) : 0;
    }
    bool updateDate(const String& n, GPdate f) {
        return update(n) ? (answer(f), 1) : 0;
    }
    bool updateTime(const String& n, GPtime f) {
        return update(n) ? (answer(f), 1) : 0;
    }
    bool updateWeek(const String& n, GPweek f) {
        return update(n) ? (answer(f), 1) : 0;
    }
    bool updateFlags(const String& n, GPflags f) {
        return update(n) ? (answer(f), 1) : 0;
    }
    bool updateColor(const String& n, GPcolor f) {
        return update(n) ? (answer(f), 1) : 0;
    }

    bool updateLog(GPlog& log) {
        return update(log.name) ? (answer(log.read()), 1) : 0;
    }

    // ================== UPDATE AUTO OBJ ===================
    bool update(GP::TITLE& title) {
        return (update(title.name) ? answer(title.text) : 0);
    }
    bool update(GP::LABEL& label) {
        return (update(label.name) ? answer(label.text) : 0);
    }
    bool update(GP::LABEL_BLOCK& label) {
        return (update(label.name) ? answer(label.text) : 0);
    }

    bool update(GP::LED& led) {
        return (update(led.name) ? answer(led.state) : 0);
    }
    bool update(GP::LED_RED& led) {
        return (update(led.name) ? answer(led.state) : 0);
    }
    bool update(GP::LED_GREEN& led) {
        return (update(led.name) ? answer(led.state) : 0);
    }

    bool update(GP::NUMBER& num) {
        return (update(num.name) ? answer(num.value) : 0);
    }
    bool update(GP::NUMBER_F& num) {
        return (update(num.name) ? answer(num.value, num.decimals) : 0);
    }

    bool update(GP::TEXT& txt) {
        return (update(txt.name) ? answer(txt.text) : 0);
    }
    bool update(GP::PASS& pas) {
        return (update(pas.name) ? answer(pas.text) : 0);
    }

    bool update(GP::AREA& ar) {
        return (update(ar.name) ? answer(ar.text) : 0);
    }

    bool update(GP::CHECK& ch) {
        return (update(ch.name) ? answer(ch.state) : 0);
    }
    bool update(GP::SWITCH& sw) {
        return (update(sw.name) ? answer(sw.state) : 0);
    }

    bool update(GP::DATE& d) {
        return (update(d.name) ? answer(d.date) : 0);
    }
    bool update(GP::TIME& t) {
        return (update(t.name) ? answer(t.time) : 0);
    }
    bool update(GP::COLOR& c) {
        return (update(c.name) ? answer(c.color) : 0);
    }

    bool update(GP::SPINNER& s) {
        return (update(s.name) ? answer(s.value, s.decimals) : 0);
    }
    bool update(GP::SLIDER& s) {
        return (update(s.name) ? answer(s.value, s.decimals) : 0);
    }

    // вернёт true, если было обновление
    bool update() {
        return (bool)_updPtr;
    }

    // вернёт true, если было update с указанного компонента
    bool update(const String& name) {
        return update() ? _updPtr->equals(name) : 0;
    }

    // вернёт true, если имя обновляемого компонента НАЧИНАЕТСЯ с указанного
    bool updateSub(const String& name) {
        return update() ? _updPtr->startsWith(name) : 0;
    }

    // вернёт имя обновлённого компонента
    String updateName() {
        return update() ? (*_updPtr) : __empty_str__;
    }

    // вернёт часть имени обновляемого компонента, находящейся под номером idx после разделителя /
    String updateNameSub(int idx = 1) {
        return update() ? (listIdx(*_updPtr, idx, '/')) : __empty_str__;
    }

    String* _answPtr = nullptr;
    String* _updPtr = nullptr;
    const String* _argValPtr = nullptr;
    const String* _argNamePtr = nullptr;
    String _hold;
    uint8_t _holdF = 0;

private:
    int getIntUniv(const String& s) {
        if (s[0] == '#') {
            GPcolor col(s);
            return col.getHEX();
        } else return s.toInt();
    }
};

} // namespace GP
