#pragma once

// GP Parsers

#include "canvas.h"
#include "log.h"
#include "objects.h"
#include "utils.h"
#include <Arduino.h>

namespace GP {

// clang-format off

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
    bool clickDown(const String& name) { return hold() ? (_holdF == 1 && argName().equals(name)) : 0; }
    // вернёт true, если кнопка была нажата и имя компонента начинается с указанного
    bool clickDownSub(const String& name) { return hold() ? (_holdF == 1 && argName().startsWith(name)) : 0; }

    // вернёт true, если кнопка была отпущена
    bool clickUp(const String& name) { return hold() ? (_holdF == 2 && argName().equals(name)) : 0; }
    // вернёт true, если кнопка была отпущена и имя компонента начинается с указанного
    bool clickUpSub(const String& name) { return hold() ? (_holdF == 2 && argName().startsWith(name)) : 0; }

    // ===================== CLICK OBJ ======================
    bool clickDown(GP::BUTTON& btn) { return clickDown(btn.name); }

    bool clickUp(GP::BUTTON& btn) { return clickUp(btn.name); }

    bool clickDown(GP::BUTTON_MINI& btn) { return clickDown(btn.name); }

    bool clickUp(GP::BUTTON_MINI& btn) { return clickUp(btn.name); }

    // ===================== CLICK =====================
    // вернёт true, если был клик по (кнопка, чекбокс, свитч, слайдер, селектор)
    bool click() { return hasClick() && args(); }

    // вернёт true, если был клик по указанному элементу (кнопка, чекбокс, свитч, слайдер, селектор)
    bool click(const String& name) { return click() ? (argName().equals(name) && args() == 1) : 0; }

    // вернёт true, если имя кликнутого компонента начинается с name
    bool clickSub(const String& name) { return click() ? (argName().startsWith(name) && args() == 1) : 0; }

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
    bool clickStr(char* t, uint16_t len = 0) { return click() ? copyStr(t, len) : 0; }
    bool clickString(String& t) { return click() ? copyString(t) : 0; }
    template <typename T>
    bool clickInt(T& t) { return click() ? copyInt(t) : 0; }
    template <typename T>
    bool clickFloat(T& t) { return click() ? copyFloat(t) : 0; }
    template <typename T>
    bool clickBool(T& t) { return click() ? copyBool(t) : 0; }
    bool clickDate(GP::Date& t) { return click() ? copyDate(t) : 0; }
    bool clickTime(GP::Time& t) { return click() ? copyTime(t) : 0; }
    bool clickWeek(GPweek& t) { return click() ? copyWeek(t) : 0; }
    bool clickFlags(GP::Flags& t) { return click() ? copyFlags(t) : 0; }
    bool clickColor(GPcolor& t) { return click() ? copyColor(t) : 0; }

    // с указанием имени компонента
    bool clickStr(const String& n, char* t, uint16_t len = 0) { return click() ? copyStr(n, t, len) : 0; }
    bool clickString(const String& n, String& t) { return click() ? copyString(n, t) : 0; }
    template <typename T>
    bool clickInt(const String& n, T& t) { return click() ? copyInt(n, t) : 0; }
    template <typename T>
    bool clickFloat(const String& n, T& t) { return click() ? copyFloat(n, t) : 0; }
    template <typename T>
    bool clickBool(const String& n, T& t) { return click() ? copyBool(n, t) : 0; }
    bool clickDate(const String& n, GP::Date& t) { return click() ? copyDate(n, t) : 0; }
    bool clickTime(const String& n, GP::Time& t) { return click() ? copyTime(n, t) : 0; }
    bool clickWeek(const String& n, GPweek& t) { return click() ? copyWeek(n, t) : 0; }
    bool clickFlags(const String& n, GP::Flags& t) { return click() ? copyFlags(n, t) : 0; }
    bool clickColor(const String& n, GPcolor& t) { return click() ? copyColor(n, t) : 0; }

    // ======================= ПАРСЕРЫ =======================
    // ОПАСНЫЕ ФУНКЦИИ (не проверяют есть ли запрос). Конвертируют и возвращают значение
    // получить String строку с компонента
    //    template <typename T>
    //    T get(const String& n) {
    //        if constexpr (is_same_v<T, GPcolor>) return {};
    //        if constexpr (is_same_v<T, GP::Date>) return {};
    //        if constexpr (is_same_v<T, GP::Time>) return {};
    //        if constexpr (is_same_v<T, String>) return arg(n);
    //        if constexpr (is_same_v<T, bool>) return {};
    //        if constexpr (is_same_v<T, char*>) return {};
    //        if constexpr (is_same_v<T, float>) return {};
    //        if constexpr (is_same_v<T, int8_t>) return {};
    //        if constexpr (is_same_v<T, long>) return {};
    //    }

    String getString(const String& n) { return arg(n); }
    String getString()                { return arg(); }

    // получить число с компонента
    int getInt(const String& n) { return getIntUniv(arg(n)); }
    int getInt()                { return getIntUniv(arg()); }

    // получить float с компонента
    float getFloat(const String& n) { return arg(n).toFloat(); }
    float getFloat()                { return arg().toFloat(); }

    // получить состояние чекбокса
    bool getBool(const String& n) {
        String s = arg(n);
        return (s[0] == '1' || s[0] == 'o' || s[0] == 't');
    }
    bool getBool() {
        return (arg()[0] == '1' || arg()[0] == 'o' || arg()[0] == 't');
    }

    // получить дату с компонента
    GP::Date getDate(const String& n) { return GP::Date(arg(n)); }
    GP::Date getDate()                { return GP::Date(arg()); }

    // получить время с компонента
    GP::Time getTime(const String& n) { return GP::Time(arg(n)); }
    GP::Time getTime()                { return GP::Time(arg()); }

    // получить неделю с компонента
    GPweek getWeek(const String& n) { return GPweek(arg(n)); }
    GPweek getWeek()                { return GPweek(arg()); }

    // получить флаги с компонента
    GP::Flags getFlags(const String& n) { return GP::Flags(arg(n)); }
    GP::Flags getFlags()                { return GP::Flags(arg()); }

    // получить цвет с компонента
    GPcolor getColor(const String& n) { return GPcolor(arg(n)); }
    GPcolor getColor()                { return GPcolor(arg()); }

    // ===================== COPY-ПАРСЕРЫ =====================
    // ОПАСНЫЕ парсеры (не проверяют запрос). Использовать только в условии!
    bool copyStr(char* t, uint16_t len = 0) {
        // return (args() && (!len || arg().length() < len)) ? (strcpy(t, arg().c_str()), 1) : 0;
        if(!args()) return 0;
        if(!len || arg().length() < len) strcpy(t, arg().c_str());
        else {
            strncpy(t, arg().c_str(), len - 1);
            t[len - 1] = '\0';
        }
        return 1;
    }
    bool copyString(String& t) { return args() ? (t = arg(), 1) : 0; }
    template <typename T>
    bool copyInt(T& t) {         return args() ? (t = getInt(), 1) : 0; }
    template <typename T>
    bool copyFloat(T& t) {       return args() ? (t = getFloat(), 1) : 0; }
    template <typename T>
    bool copyBool(T& t) {        return args() ? (t = getBool(), 1) : 0; }
    bool copyDate(GP::Date& t) {   return args() ? (t = getDate(), 1) : 0; }
    bool copyTime(GP::Time& t) {   return args() ? (t = getTime(), 1) : 0; }
    bool copyWeek(GPweek& t) {   return args() ? (t = getWeek(), 1) : 0; }
    bool copyFlags(GP::Flags& t) { return args() ? (t = getFlags(), 1) : 0; }
    bool copyColor(GPcolor& t) { return args() ? (t = getColor(), 1) : 0; }

    // БЕЗОПАСНЫЕ парсеры (проверяют запрос). Копируют данные из запроса в переменную
    bool copyStr(const String& n, char* t, uint16_t len = 0) {
        return (hasArg(n) && (!len || arg(n).length() < len)) ? (strcpy(t, arg(n).c_str()), 1) : 0;
    }
    bool copyString(const String& n, String& t) {        return hasArg(n) ? (t = arg(n), 1) : 0;    }
    template <typename T>
    bool copyInt(const String& n, T& t) {        return hasArg(n) ? (t = getInt(n), 1) : 0;    }
    template <typename T>
    bool copyFloat(const String& n, T& t) {        return hasArg(n) ? (t = getFloat(n), 1) : 0;    }
    template <typename T>
    bool copyBool(const String& n, T& t) {        return hasArg(n) ? (t = getBool(n), 1) : 0;    }
    bool copyDate(const String& n, GP::Date& t) {        return hasArg(n) ? (t = getDate(n), 1) : 0;    }
    bool copyTime(const String& n, GP::Time& t) {        return hasArg(n) ? (t = getTime(n), 1) : 0;    }
    bool copyWeek(const String& n, GPweek& t) {        return hasArg(n) ? (t = getWeek(n), 1) : 0;    }
    bool copyFlags(const String& n, GP::Flags& t) {        return hasArg(n) ? (t = getFlags(n), 1) : 0;    }
    bool copyColor(const String& n, GPcolor& t) {        return hasArg(n) ? (t = getColor(n), 1) : 0;    }

    // ===================== COPY OBJ =====================
    bool copy(GP::AREA& obj)     { return copyString( obj.name, obj.text); }
    bool copy(GP::CHECK& obj)    { return copyBool(   obj.name, obj.state); }
    bool copy(GP::COLOR& obj)    { return copyColor(  obj.name, obj.color); }
    bool copy(GP::DATE& obj)     { return copyDate(   obj.name, obj.date); }
    bool copy(GP::NUMBER& obj)   { return copyInt(    obj.name, obj.value); }
    bool copy(GP::NUMBER_F& obj) { return copyFloat(  obj.name, obj.value); }
    bool copy(GP::PASS& obj)     { return copyString( obj.name, obj.text); }
    bool copy(GP::SELECT& obj)   { return copyInt(    obj.name, obj.selected); }
    bool copy(GP::SLIDER& obj)   { return copyFloat(  obj.name, obj.value); }
    bool copy(GP::SPINNER& obj)  { return copyFloat(  obj.name, obj.value); }
    bool copy(GP::SWITCH& obj)   { return copyBool(   obj.name, obj.state); }
    bool copy(GP::TEXT& obj)     { return copyString( obj.name, obj.text); }
    bool copy(GP::TIME& obj)     { return copyTime(   obj.name, obj.time); }

    // ===================== CLICK OBJ =====================
    bool click(GP::AREA& obj)        { return click() ? copy(obj) : 0; }
    bool click(GP::BUTTON& obj)      { return click(obj.name); }
    bool click(GP::BUTTON_MINI& obj) { return click(obj.name); }
    bool click(GP::CHECK& obj)       { return click() ? copy(obj) : 0; }
    bool click(GP::COLOR& obj)       { return click() ? copy(obj) : 0; }
    bool click(GP::DATE& obj)        { return click() ? copy(obj) : 0; }
    bool click(GP::NUMBER& obj)      { return click() ? copy(obj) : 0; }
    bool click(GP::NUMBER_F& obj)    { return click() ? copy(obj) : 0; }
    bool click(GP::PASS& obj)        { return click() ? copy(obj) : 0; }
    bool click(GP::SELECT& obj)      { return click() ? copy(obj) : 0; }
    bool click(GP::SLIDER& obj)      { return click() ? copy(obj) : 0; }
    bool click(GP::SPINNER& obj)     { return click() ? copy(obj) : 0; }
    bool click(GP::SWITCH& obj)      { return click() ? copy(obj) : 0; }
    bool click(GP::TEXT& obj)        { return click() ? copy(obj) : 0; }
    bool click(GP::TIME& obj)        { return click() ? copy(obj) : 0; }

    // отправить ответ на обновление
    bool answer(const String& s) {
        if(_answPtr) *_answPtr += s;
        return (bool)_answPtr;
    }
    bool answer(int v) {
        if(_answPtr) *_answPtr += v;
        return (bool)_answPtr;
    }

    // для float/double
    template <typename T>
    bool answer(T v, uint8_t dec) { return answer(String(v, (uint16_t)dec)); }
    bool answer(int* v, int am, int dec = 0) {
        String s;
        s.reserve(am * 4);
        for(int i = 0; i < am; i++) {
            if(dec) s += (float)v[i] / dec;
            else s += v[i];
            if(i != am - 1) s += ',';
        }
        return answer(s);
    }

    bool answer(GPcolor col)   { return answer(col.encode()); }
    bool answer(GP::Date date)   { return answer(date.encode()); }
    bool answer(GP::Time time)   { return answer(time.encode()); }
    bool answer(GPweek week)   { return answer(week.encode()); }
    bool answer(GP::Flags flags) { return answer(flags.encode()); }
    bool answer(Canvas& cv)    { return answer(cv._read()); }



    // ==================== UPDATE AUTO =====================
    // автоматическое обновление. Отправит значение из указанной переменной
    // Вернёт true в момент обновления

    bool updateString(const String& n, String& f)       { return update(n) ? (answer(f), 1) : 0; }
    template <typename T>
    bool updateInt(const String& n, T f)                { return update(n) ? (answer((int)f), 1) : 0; }
    template <typename T>
    bool updateFloat(const String& n, T f, int dec = 2) { return update(n) ? (answer(f, dec), 1) : 0; }
    bool updateBool(const String& n, bool f)            { return update(n) ? (answer(f), 1) : 0; }
    bool updateDate(const String& n, GP::Date f)          { return update(n) ? (answer(f), 1) : 0; }
    bool updateTime(const String& n, GP::Time f)          { return update(n) ? (answer(f), 1) : 0; }
    bool updateWeek(const String& n, GPweek f)          { return update(n) ? (answer(f), 1) : 0; }
    bool updateFlags(const String& n, GP::Flags f)        { return update(n) ? (answer(f), 1) : 0; }
    bool updateColor(const String& n, GPcolor f)        { return update(n) ? (answer(f), 1) : 0; }

    bool updateLog(GP::Log& log)         { return update(log.name) ? (answer(log.read()), 1) : 0; }

    // ================== UPDATE AUTO OBJ ===================
    bool update(GP::AREA& obj)        { return (update(obj.name) ? answer(obj.text) : 0); }
    bool update(GP::CHECK& obj)       { return (update(obj.name) ? answer(obj.state) : 0); }
    bool update(GP::COLOR& obj)       { return (update(obj.name) ? answer(obj.color) : 0); }
    bool update(GP::DATE& obj)        { return (update(obj.name) ? answer(obj.date) : 0); }
    bool update(GP::LABEL& obj)       { return (update(obj.name) ? answer(obj.text) : 0); }
    bool update(GP::LABEL_BLOCK& obj) { return (update(obj.name) ? answer(obj.text) : 0); }
    bool update(GP::LED& obj)         { return (update(obj.name) ? answer(obj.state) : 0); }
    bool update(GP::LED_GREEN& obj)   { return (update(obj.name) ? answer(obj.state) : 0); }
    bool update(GP::LED_RED& obj)     { return (update(obj.name) ? answer(obj.state) : 0); }
    bool update(GP::NUMBER& obj)      { return (update(obj.name) ? answer(obj.value) : 0); }
    bool update(GP::NUMBER_F& obj)    { return (update(obj.name) ? answer(obj.value, obj.decimals) : 0); }
    bool update(GP::PASS& obj)        { return (update(obj.name) ? answer(obj.text) : 0); }
    bool update(GP::SLIDER& obj)      { return (update(obj.name) ? answer(obj.value, obj.decimals) : 0); }
    bool update(GP::SPINNER& obj)     { return (update(obj.name) ? answer(obj.value, obj.decimals) : 0); }
    bool update(GP::SWITCH& obj)      { return (update(obj.name) ? answer(obj.state) : 0); }
    bool update(GP::TEXT& obj)        { return (update(obj.name) ? answer(obj.text) : 0); }
    bool update(GP::TIME& obj)        { return (update(obj.name) ? answer(obj.time) : 0); }
    bool update(GP::TITLE& obj)       { return (update(obj.name) ? answer(obj.text) : 0); }

    // clang-format on

    // вернёт true, если было обновление
    bool update() { return (bool)_updPtr; }

    // вернёт true, если было update с указанного компонента
    bool update(const String& name) { return update() ? _updPtr->equals(name) : 0; }

    // вернёт true, если имя обновляемого компонента НАЧИНАЕТСЯ с указанного
    bool updateSub(const String& name) { return update() ? _updPtr->startsWith(name) : 0; }

    // вернёт имя обновлённого компонента
    String updateName() { return update() ? (*_updPtr) : __empty_str__; }

    // вернёт часть имени обновляемого компонента, находящейся под номером idx после разделителя /
    String updateNameSub(int idx = 1) { return update() ? (listIdx(*_updPtr, idx, '/')) : __empty_str__; }

    String* _answPtr = nullptr;
    String* _updPtr = nullptr;
    const String* _argValPtr = nullptr;
    const String* _argNamePtr = nullptr;
    String _hold;
    uint8_t _holdF = 0;

private:
    int getIntUniv(const String& s) {
        if(s[0] == '#') {
            GPcolor col(s);
            return col.getHEX();
        } else return s.toInt();
    }
};

} // namespace GP
