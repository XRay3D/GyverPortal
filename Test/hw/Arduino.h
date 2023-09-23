#pragma once
#include <QCoreApplication>
#include <QDateTime>
#include <QEventLoop>
#include <QString>
#include <QTimer>
#include <WString.h>
#include <cmath>
#include <filesystem>
#include <random>

using std::isnan;

#include "themes.h"

#define UPDATE_ERROR_OK               (0)
#define UPDATE_ERROR_WRITE            (1)
#define UPDATE_ERROR_ERASE            (2)
#define UPDATE_ERROR_READ             (3)
#define UPDATE_ERROR_SPACE            (4)
#define UPDATE_ERROR_SIZE             (5)
#define UPDATE_ERROR_STREAM           (6)
#define UPDATE_ERROR_MD5              (7)
#define UPDATE_ERROR_FLASH_CONFIG     (8)
#define UPDATE_ERROR_NEW_FLASH_CONFIG (9)
#define UPDATE_ERROR_MAGIC_BYTE       (10)

#define U_FLASH  0
#define U_SPIFFS 100
#define U_AUTH   200
///////////////////////////////////////////

// x: the numeric variable to which to write.
// n: which bit of the number to write, starting at 0 for the least-significant (rightmost) bit.
// b: the value to write to the bit (0 or 1).
template <typename T>
inline void bitWrite(T& x, int n, bool b) { b ? (x |= (1 << n)) : (x &= ~(1 << n)); }

// x: the number from which to read.
// n: which bit to read, starting at 0 for the least-significant (rightmost) bit.
template <typename T>
inline bool bitRead(T x, int n) { return x & (1 << n); }

inline constexpr auto DEG_TO_RAD{M_PI / 180.};
inline constexpr auto RAD_TO_DEG{180. / M_PI};

inline int random(int max) {
    static std::random_device rd; // non-deterministic generator
    std::uniform_int_distribution<int> dist(0, max);
    return dist(rd);
}
inline int random(int min, int max) {
    static std::random_device rd; // non-deterministic generator
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rd);
}

// struct String : QByteArray {
//     using QByteArray::QByteArray;
//     String(char c)
//         : QByteArray(&c, 1) {
//     }

//    String(int val)
//        : QByteArray{QByteArray::number(val)} {
//    }

//    String(float val)
//        : QByteArray{QByteArray::number(val)} {
//    }

//    String(double val)
//        : QByteArray{QByteArray::number(val)} {
//    }

//    String(QByteArray&& val)
//        : QByteArray{std::move(val)} {
//    }

//    String(const QByteArray& val)
//        : QByteArray{val} {
//    }

//    String& operator+=(int c) { return push_back(c), *this; }
//    template <int N>
//    String& operator+=(const char (&str)[N]) { return push_back(str), *this; }
//    String& operator+=(const char* str) { return push_back(str), *this; }

//    char* c_str() { return data(); }
//    const char* c_str() const { return constData(); }
//    char charAt(int i) const { return at(i); }
//    String substring(int f, int t) const { return {mid(f, f + t)}; }

//    template <typename T>
//    bool equals(const T& left) const { return operator==(left); }

//    String& remove(int i) { return QByteArray::remove(i, 1), *this; }
//};

// using StreamString = String;

// inline const String operator+(const String& a1, const String& a2) {
//     return {QByteArray(a1) += a2};
// }
// inline const String operator+(const String& a1, const char* a2) {
//     return {QByteArray(a1) += a2};
// }
// inline const String operator+(const String& a1, char a2) {
//     return {QByteArray(a1) += a2};
// }
// inline const String operator+(const char* a1, const String& a2) {
//     return {QByteArray(a1) += a2};
// }
// inline const String operator+(char a1, const String& a2) {
//     return {QByteArray(&a1, 1) += a2};
// }

// template <typename T>
// inline decltype(auto) F(T&& S) { return S; }

//#define F(STR) STR

inline const void* pgm_read_dword(const void* S) { return S; }

inline uint32_t millis() { return QTime::currentTime().msecsSinceStartOfDay(); }

inline void delay(uint32_t time) {
    if(time < 10) return;
    QEventLoop loop;
    QTimer::singleShot(time, &loop, [&loop] { loop.quit(); });
    loop.exec();
}

template <typename T>
inline void bitClear(T& val, size_t n) { val &= ~(1 << n); }

inline void yield() {
    qApp->processEvents();
}

inline int constrain(int a, int b, int c) { return std::clamp(a, b, c); }

inline struct ESP_ {
    int getFlashChipSize() { return {}; }
    int getFreeHeap() { return {}; }
    int getFreeSketchSpace() { return {}; }
    int getSketchSize() { return {}; }
    int getCycleCount() { return {}; }
    int getCpuFreqMHz() { return {}; }
    String getSdkVersion() { return {}; }
    void restart() { }
} ESP;

inline struct Update_ {
    Update_() { }
    bool begin(int, int) { return {}; }
    void printError(String&) { }
    bool end(bool = {}) { return {}; }
    bool write(char*, int) { return {}; }
    int getError() { return UPDATE_ERROR_OK; }
} Update;

inline struct Serial_ {
    void begin(int) { }
    template <typename T>
    void print(T&& val) {
        qInfo().nospace() << val;
    }
    template <typename T>
    void println(T&& val) {
        qInfo() << val;
    }
} Serial;
