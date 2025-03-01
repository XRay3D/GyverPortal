#pragma once
#if 0
#ifdef __cplusplus

// #include <pgmspace.h>

#include <QByteArray>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <type_traits>
#include <utility>

// #define PSTR_ALIGN  4
// #define PSTRN(s, n) (PGM_P __attribute__((aligned(n))) s) //(__extension__({static const char __pstr__[] __attribute__((__aligned__(n))) __attribute__((section( "\".irom0.pstr." __FILE__ "." __STRINGIZE(__LINE__) "."  __STRINGIZE(__COUNTER__) "\", \"aSM\", @progbits, 1 #"))) = (s); &__pstr__[0]; }))
// #define PSTR(s)     PSTRN(s, PSTR_ALIGN)
#define PSTR(s)             ((const char*)s)

// an abstract class used as a means to proide a unique pointer type
// but really has no body
class __FlashStringHelper { };
#define FPSTR(pstr_pointer) reinterpret_cast<const __FlashStringHelper*>(pstr_pointer)
#define F(string_literal)   (FPSTR(PSTR(string_literal)))

// support libraries that expect this name to be available
// replace with `using StringSumHelper = String;` in case something wants this constructible
class StringSumHelper;

// The string class
class String {
public:
    // constructors
    // creates a copy of the initial value.
    // if the initial value is null or invalid, or if memory allocation
    // fails, the string will be marked as invalid (i.e. "if (s)" will
    // be false).
    String() __attribute__((always_inline)) { // See init()
        init();
    }
    String(const char* cstr);
    String(const String& str);
    String(const __FlashStringHelper* str);
    String(String&& rval) noexcept;

    explicit String(char c) {
        sso.buff[0] = c;
        sso.buff[1] = 0;
        sso.len = 1;
        sso.isHeap = 0;
    }

    String(unsigned char, unsigned char base);
    explicit String(unsigned char value)
        : String(value, 10) { }

    String(int, unsigned char base);
    explicit String(int value)
        : String(value, 10) { }

    String(unsigned int, unsigned char base);
    explicit String(unsigned int value)
        : String(value, 10) { }

    String(long, unsigned char base);
    explicit String(long value)
        : String(value, 10) { }

    String(unsigned long, unsigned char base);
    explicit String(unsigned long value)
        : String(value, 10) { }

    String(long long, unsigned char base);
    explicit String(long long value)
        : String(value, 10) { }

    String(unsigned long long, unsigned char base);
    explicit String(unsigned long long value)
        : String(value, 10) { }

    String(float, unsigned char decimalPlaces);
    explicit String(float value)
        : String(value, 2) { }

    String(double, unsigned char decimalPlaces);
    explicit String(double value)
        : String(value, 2) { }

    ~String() {
        invalidate();
    }

    // memory management
    // return true on success, false on failure (in which case, the string
    // is left unchanged).  reserve(0), if successful, will validate an
    // invalid string (i.e., "if (s)" will be true afterwards)
    bool reserve(unsigned int size);
    unsigned int length(void) const {
        return buffer() ? len() : 0;
    }
    void clear(void) {
        setLen(0);
    }
    bool isEmpty(void) const {
        return length() == 0;
    }

    // assign string types as well as built-in numeric types
    String& operator=(const String& rhs);
    String& operator=(String&& rval) noexcept;
    String& operator=(const char* cstr);
    String& operator=(const __FlashStringHelper* str);
    String& operator=(char c);

    String& operator=(unsigned char value) {
        *this = String(value);
        return *this;
    }

    String& operator=(int value) {
        *this = String(value);
        return *this;
    }

    String& operator=(unsigned int value) {
        *this = String(value);
        return *this;
    }

    String& operator=(long value) {
        *this = String(value);
        return *this;
    }

    String& operator=(unsigned long value) {
        *this = String(value);
        return *this;
    }

    String& operator=(long long value) {
        *this = String(value);
        return *this;
    }

    String& operator=(unsigned long long value) {
        *this = String(value);
        return *this;
    }

    String& operator=(float value) {
        *this = String(value);
        return *this;
    }

    String& operator=(double value) {
        *this = String(value);
        return *this;
    }

    // concatenate (works w/ built-in types, same as assignment)

    // returns true on success, false on failure (in which case, the string
    // is left unchanged).  if the argument is null or invalid, the
    // concatenation is considered unsuccessful.
    bool concat(const String& str);
    bool concat(const char* cstr);
    bool concat(const char* cstr, unsigned int length);
    bool concat(const __FlashStringHelper* str);
    bool concat(char c);

    bool concat(unsigned char c);
    bool concat(int num);
    bool concat(unsigned int num);
    bool concat(long num);
    bool concat(unsigned long num);
    bool concat(long long num);
    bool concat(unsigned long long num);
    bool concat(float num);
    bool concat(double num);

    // if there's not enough memory for the concatenated value, the string
    // will be left unchanged (but this isn't signalled in any way)
    template <typename T>
    String& operator+=(const T& rhs) {
        concat(rhs);
        return *this;
    }

    // checks whether the internal buffer pointer is set.
    // (should not be the case for us, since we always reset the pointer to the SSO buffer instead of setting it to nullptr)
    explicit operator bool() const {
        return buffer() != nullptr;
    }

    int compareTo(const String& s) const;
    bool equals(const String& s) const;
    bool equals(const char* cstr) const;
    bool equals(const __FlashStringHelper* s) const;
    bool operator==(const String& rhs) const {
        return equals(rhs);
    }
    bool operator==(const char* cstr) const {
        return equals(cstr);
    }
    bool operator!=(const String& rhs) const {
        return !equals(rhs);
    }
    bool operator!=(const char* cstr) const {
        return !equals(cstr);
    }
    bool operator<(const String& rhs) const;
    bool operator>(const String& rhs) const;
    bool operator<=(const String& rhs) const;
    bool operator>=(const String& rhs) const;
    bool equalsIgnoreCase(const String& s) const;
    bool equalsIgnoreCase(const __FlashStringHelper* s) const;
    unsigned char equalsConstantTime(const String& s) const;
    bool startsWith(const String& prefix) const;
    bool startsWith(const char* prefix) const;
    bool startsWith(const __FlashStringHelper* prefix) const;
    bool startsWith(const String& prefix, unsigned int offset) const;
    bool startsWith(const __FlashStringHelper* prefix, unsigned int offset) const;
    bool endsWith(const String& suffix) const;
    bool endsWith(const char* suffix) const;
    bool endsWith(const __FlashStringHelper* suffix) const;

    // character access
    char charAt(unsigned int index) const {
        return operator[](index);
    }
    void setCharAt(unsigned int index, char c);
    char operator[](unsigned int index) const;
    char& operator[](unsigned int index);
    void getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index = 0) const;
    void toCharArray(char* buf, unsigned int bufsize, unsigned int index = 0) const {
        getBytes((unsigned char*)buf, bufsize, index);
    }
    const char* c_str() const { return buffer(); }
    char* begin() { return wbuffer(); }
    char* end() { return wbuffer() + length(); }
    const char* begin() const { return c_str(); }
    const char* end() const { return c_str() + length(); }

    // search
    int indexOf(char ch, unsigned int fromIndex = 0) const;
    int indexOf(const char* str, unsigned int fromIndex = 0) const;
    int indexOf(const __FlashStringHelper* str, unsigned int fromIndex = 0) const {
        return indexOf((const char*)str, fromIndex);
    }
    int indexOf(const String& str, unsigned int fromIndex = 0) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(char ch, unsigned int fromIndex) const;
    int lastIndexOf(const String& str) const;
    int lastIndexOf(const String& str, unsigned int fromIndex) const;
    int lastIndexOf(const __FlashStringHelper* str) const;
    int lastIndexOf(const __FlashStringHelper* str, unsigned int fromIndex) const;
    String substring(unsigned int beginIndex) const {
        return substring(beginIndex, len());
    }
    String substring(unsigned int beginIndex, unsigned int endIndex) const;

    // modification
    void replace(char find, char replace);
    void replace(const String& find, const String& replace);
    void replace(const char* find, const String& replace);
    void replace(const __FlashStringHelper* find, const String& replace);
    void replace(const char* find, const char* replace);
    void replace(const __FlashStringHelper* find, const char* replace);
    void replace(const __FlashStringHelper* find, const __FlashStringHelper* replace);

    // Pass the biggest integer if the count is not specified.
    // The remove method below will take care of truncating it at the end of the string.
    void remove(unsigned int index, unsigned int count = (unsigned int)-1);
    void toLowerCase(void);
    void toUpperCase(void);
    void trim(void);

    // parsing/conversion
    long toInt(void) const;
    float toFloat(void) const;
    double toDouble(void) const;

    explicit operator QByteArray() const { return QByteArray{buffer(), len()}; }
    String& operator=(const QByteArray& ba) { return *this = String{ba}; }
    String(const QByteArray& ba)
        : String{ba.constData()} { }

protected:
    // Contains the string info when we're not in SSO mode
    struct _ptr {
        char* buff;
        uint16_t cap;
        uint16_t len;
    };
    // This allows strings up up to 11 (10 + \0 termination) without any extra space.
    enum {
        SSOSIZE = sizeof(struct _ptr) + 4 - 1
    }; // Characters to allocate space for SSO, must be 12 or more
    struct _sso {
        char buff[SSOSIZE];
        unsigned char len : 7; // Ensure only one byte is allocated by GCC for the bitfields
        unsigned char isHeap : 1;
    } __attribute__((packed)); // Ensure that GCC doesn't expand the flag byte to a 32-bit word for alignment issues
    enum {
        CAPACITY_MAX = 65535
    }; // If typeof(cap) changed from uint16_t, be sure to update this enum to the max value storable in the type
    union {
        struct _ptr ptr;
        struct _sso sso;
    };
    // Accessor functions
    bool isSSO() const { return !sso.isHeap; }
    unsigned int len() const { return isSSO() ? sso.len : ptr.len; }
    unsigned int capacity() const { return isSSO() ? (unsigned int)SSOSIZE - 1 : ptr.cap; } // Size of max string not including terminal NUL
    void setSSO(bool set) { sso.isHeap = !set; }
    void setLen(int len) {
        if(isSSO()) {
            setSSO(true); // Avoid emitting of bitwise EXTRACT-AND-OR ops (store-merging optimization)
            sso.len = len;
        } else
            ptr.len = len;
    }
    void setCapacity(int cap) {
        if(!isSSO()) ptr.cap = cap;
    }
    void setBuffer(char* buff) {
        if(!isSSO()) ptr.buff = buff;
    }
    // Buffer accessor functions
    const char* buffer() const { return isSSO() ? sso.buff : ptr.buff; }
    char* wbuffer() { return const_cast<char*>(buffer()); } // Writable version of buffer

    // concatenation is done via non-member functions
    // make sure we still have access to internal methods, since we optimize based on capacity of both sides and want to manipulate internal buffers directly
    friend String operator+(const String& lhs, String&& rhs);
    friend String operator+(String&& lhs, String&& rhs);
    friend String operator+(char lhs, String&& rhs);
    friend String operator+(const char* lhs, String&& rhs);
    friend String operator+(const __FlashStringHelper* lhs, String&& rhs);

protected:
    // TODO: replace init() with a union constructor, so it's called implicitly

    void init(void) __attribute__((always_inline)) {
        sso.buff[0] = 0;
        sso.len = 0;
        sso.isHeap = 0;
        // Without the 6 statements shown below, GCC simply emits such as: "MOVI.N aX,0", "S8I aX,a2,0" and "S8I aX,a2,11" (8 bytes in total)
        sso.buff[1] = 0;
        sso.buff[2] = 0;
        sso.buff[3] = 0;
        sso.buff[8] = 0;
        sso.buff[9] = 0;
        sso.buff[10] = 0;
        // With the above, thanks to store-merging, GCC can use the narrow form of 32-bit store insn ("S32I.N") and emits:
        //   "MOVI.N aX,0", "S32I.N aX,a2,0" and "S32I.N aX,a2,8" (6 bytes in total)
        // (Literature: Xtensa(R) Instruction Set Reference Manual, "S8I - Store 8-bit" [p.504] and "S32I.N - Narrow Store 32-bit" [p.512])
        // Unfortunately, GCC seems not to re-evaluate the cost of inlining after the store-merging optimizer stage,
        // `always_inline` attribute is necessary in order to keep inlining.
    }

    // resets the string storage to the initial state
    void invalidate(void);
    bool changeBuffer(unsigned int maxStrLen);

    // copy or insert at a specific position
    String& copy(const char* cstr, unsigned int length);
    String& copy(const __FlashStringHelper* pstr, unsigned int length);

    String& insert(size_t position, char);
    String& insert(size_t position, const char*);
    String& insert(size_t position, const __FlashStringHelper*);
    String& insert(size_t position, const char*, size_t length);
    String& insert(size_t position, const String&);

    // rvalue helper
    void move(String& rhs) noexcept;
};

// concatenation (note that it's done using non-method operators to handle both possible type refs)

inline String operator+(const String& lhs, const String& rhs) {
    String res;
    res.reserve(lhs.length() + rhs.length());
    res += lhs;
    res += rhs;
    return res;
}

inline String operator+(String&& lhs, const String& rhs) {
    lhs += rhs;
    return std::move(lhs);
}

String operator+(const String& lhs, String&& rhs);
String operator+(String&& lhs, String&& rhs);

template <typename T,
    typename = std::enable_if_t<!std::is_same_v<String, std::decay_t<T>>>>
inline String operator+(const String& lhs, const T& value) {
    String res(lhs);
    res += value;
    return res;
}

template <typename T,
    typename = std::enable_if_t<!std::is_same_v<String, std::decay_t<T>>>>
inline String operator+(String&& lhs, const T& value) {
    lhs += value;
    return std::move(lhs);
}

// `String(char)` is explicit, but we used to have StringSumHelper silently allowing the following:
// `String x; x = 'a' + String('b') + 'c';`
// For comparison, `std::string(char)` does not exist. However, we are allowed to chain `char` as both lhs and rhs

String operator+(char lhs, const String& rhs);

inline String operator+(char lhs, String&& rhs) {
    return std::move(rhs.insert(0, lhs));
}

// both `char*` and `__FlashStringHelper*` are implicitly converted into `String()`, calling the `operator+(const String& ...);`
// however, here we:
// - do an automatic `reserve(total length)` for the resulting string
// - possibly do rhs.insert(0, ...), when &&rhs capacity could fit both

String operator+(const char* lhs, const String& rhs);

inline String operator+(const char* lhs, String&& rhs) {
    return std::move(rhs.insert(0, lhs));
}

inline String operator+(const __FlashStringHelper* lhs, const String& rhs) {
    return reinterpret_cast<const char*>(lhs) + rhs;
}

inline String operator+(const __FlashStringHelper* lhs, String&& rhs) {
    return std::move(rhs.insert(0, lhs));
}

extern const String emptyString;

#endif // __cplusplus
#else
/*
  WString.h - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus

#include <QByteArray>
#include <QDebug>
#include <QString>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// #include <avr/pgmspace.h>

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

//////class __FlashStringHelper;
//////#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

#define memcpy_P  memcpy
#define memmove_P memmove
#define strlen_P  strlen
#define strstr_P  strstr

#define PGM_P                 const char*
#define FPSTR(string_literal) (const char*)string_literal
#define PSTR(string_literal)  (const char*)string_literal
#define F(string_literal)     string_literal

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class StringSumHelper;

// The string class
class String {
    // use a function pointer to allow for "if (s)" without the
    // complications of an operator bool(). for more information, see:
    // http://www.artima.com/cppsource/safebool.html
    typedef void (String::*StringIfHelperType)() const;
    void StringIfHelper() const { }

public:
    // constructors
    // creates a copy of the initial value.
    // if the initial value is null or invalid, or if memory allocation
    // fails, the string will be marked as invalid (i.e. "if (s)" will
    // be false).
    String(const char* cstr = "");
    String(const String& str);
    ////////	String(const __FlashStringHelper *str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    String(String&& rval);
    String(StringSumHelper&& rval);
#endif
    explicit String(char c);
    explicit String(unsigned char, unsigned char base = 10);
    explicit String(int, unsigned char base = 10);
    explicit String(unsigned int, unsigned char base = 10);
    explicit String(long, unsigned char base = 10);
    explicit String(unsigned long, unsigned char base = 10);
    explicit String(float, unsigned char decimalPlaces = 2);
    explicit String(double, unsigned char decimalPlaces = 2);
    ~String(void);

    void clear() {
        memset(buffer, 0, len);
        len = 0;
    }

    // memory management
    // return true on success, false on failure (in which case, the string
    // is left unchanged).  reserve(0), if successful, will validate an
    // invalid string (i.e., "if (s)" will be true afterwards)
    unsigned char reserve(unsigned int size);
    inline unsigned int length(void) const { return len; }

    // creates a copy of the assigned value.  if the value is null or
    // invalid, or if the memory allocation fails, the string will be
    // marked as invalid ("if (s)" will be false).
    String& operator=(const String& rhs);
    String& operator=(const char* cstr);
    ////////	String & operator = (const __FlashStringHelper *str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    String& operator=(String&& rval);
    String& operator=(StringSumHelper&& rval);
#endif

    // concatenate (works w/ built-in types)

    // returns true on success, false on failure (in which case, the string
    // is left unchanged).  if the argument is null or invalid, the
    // concatenation is considered unsucessful.
    unsigned char concat(const String& str);
    unsigned char concat(const char* cstr);
    unsigned char concat(char c);
    unsigned char concat(unsigned char c);
    unsigned char concat(int num);
    unsigned char concat(unsigned int num);
    unsigned char concat(long num);
    unsigned char concat(unsigned long num);
    unsigned char concat(float num);
    unsigned char concat(double num);
    ////////	unsigned char concat(const __FlashStringHelper * str);

    // if there's not enough memory for the concatenated value, the string
    // will be left unchanged (but this isn't signalled in any way)
    String& operator+=(const String& rhs) {
        concat(rhs);
        return (*this);
    }
    String& operator+=(const char* cstr) {
        concat(cstr);
        return (*this);
    }
    String& operator+=(char c) {
        concat(c);
        return (*this);
    }
    String& operator+=(unsigned char num) {
        concat(num);
        return (*this);
    }
    String& operator+=(int num) {
        concat(num);
        return (*this);
    }
    String& operator+=(unsigned int num) {
        concat(num);
        return (*this);
    }
    String& operator+=(long num) {
        concat(num);
        return (*this);
    }
    String& operator+=(unsigned long num) {
        concat(num);
        return (*this);
    }
    String& operator+=(float num) {
        concat(num);
        return (*this);
    }
    String& operator+=(double num) {
        concat(num);
        return (*this);
    }
    ////////	String & operator += (const __FlashStringHelper *str){concat(str); return (*this);}

    friend StringSumHelper& operator+(const StringSumHelper& lhs, const String& rhs);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, const char* cstr);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, char c);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, unsigned char num);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, int num);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, unsigned int num);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, long num);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, unsigned long num);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, float num);
    friend StringSumHelper& operator+(const StringSumHelper& lhs, double num);
    ////////	friend StringSumHelper & operator + (const StringSumHelper &lhs, const __FlashStringHelper *rhs);

    // comparison (only works w/ Strings and "strings")
    operator StringIfHelperType() const { return buffer ? &String::StringIfHelper : 0; }
    int compareTo(const String& s) const;
    unsigned char equals(const String& s) const;
    unsigned char equals(const char* cstr) const;
    unsigned char operator==(const String& rhs) const { return equals(rhs); }
    unsigned char operator==(const char* cstr) const { return equals(cstr); }
    unsigned char operator!=(const String& rhs) const { return !equals(rhs); }
    unsigned char operator!=(const char* cstr) const { return !equals(cstr); }
    unsigned char operator<(const String& rhs) const;
    unsigned char operator>(const String& rhs) const;
    unsigned char operator<=(const String& rhs) const;
    unsigned char operator>=(const String& rhs) const;
    unsigned char equalsIgnoreCase(const String& s) const;
    unsigned char startsWith(const String& prefix) const;
    unsigned char startsWith(const String& prefix, unsigned int offset) const;
    unsigned char endsWith(const String& suffix) const;

    // character acccess
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    char operator[](unsigned int index) const;
    char& operator[](unsigned int index);
    void getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index = 0) const;
    void toCharArray(char* buf, unsigned int bufsize, unsigned int index = 0) const { getBytes((unsigned char*)buf, bufsize, index); }
    const char* c_str() const { return buffer; }
    char* begin() { return buffer; }
    char* end() { return buffer + length(); }
    const char* begin() const { return c_str(); }
    const char* end() const { return c_str() + length(); }

    // search
    int indexOf(char ch) const;
    int indexOf(char ch, unsigned int fromIndex) const;
    int indexOf(const String& str) const;
    int indexOf(const String& str, unsigned int fromIndex) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(char ch, unsigned int fromIndex) const;
    int lastIndexOf(const String& str) const;
    int lastIndexOf(const String& str, unsigned int fromIndex) const;
    String substring(unsigned int beginIndex) const { return substring(beginIndex, len); };
    String substring(unsigned int beginIndex, unsigned int endIndex) const;

    // modification
    void replace(char find, char replace);
    void replace(const String& find, const String& replace);
    void remove(unsigned int index);
    void remove(unsigned int index, unsigned int count);
    void toLowerCase(void);
    void toUpperCase(void);
    void trim(void);

    // parsing/conversion
    long toInt(void) const;
    float toFloat(void) const;
    double toDouble(void) const;

    explicit operator QByteArray() const { return QByteArray{buffer, len}; }
    String& operator=(const QByteArray& ba) { return *this = String{ba}; }
    String(const QByteArray& ba)
        : String{ba.constData()} { }

protected:
    char* buffer;          // the actual char array
    unsigned int capacity; // the array length minus one (for the '\0')
    unsigned int len;      // the String length (not counting the '\0')
protected:
    void init(void);
    void invalidate(void);
    unsigned char changeBuffer(unsigned int maxStrLen);
    unsigned char concat(const char* cstr, unsigned int length);

    // copy and move
    String& copy(const char* cstr, unsigned int length);
    //////String & copy(const __FlashStringHelper *pstr, unsigned int length);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    void move(String& rhs);
#endif
};

class StringSumHelper : public String {
public:
    StringSumHelper(const String& s)
        : String(s) { }
    StringSumHelper(const char* p)
        : String(p) { }
    StringSumHelper(char c)
        : String(c) { }
    StringSumHelper(unsigned char num)
        : String(num) { }
    StringSumHelper(int num)
        : String(num) { }
    StringSumHelper(unsigned int num)
        : String(num) { }
    StringSumHelper(long num)
        : String(num) { }
    StringSumHelper(unsigned long num)
        : String(num) { }
    StringSumHelper(float num)
        : String(num) { }
    StringSumHelper(double num)
        : String(num) { }
};

inline QDebug operator<<(QDebug debug, const String& c) {
    QDebugStateSaver saver(debug);
    debug.nospace() << c.operator QByteArray();
    return debug;
}

#endif // __cplusplus
#endif // String_class_h

#endif
