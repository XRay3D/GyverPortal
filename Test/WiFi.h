#pragma once

#include <Arduino.h>

enum E {
    Null,
    WIFI_AP,
    WIFI_STA,
    WL_CONNECTED,
};

struct Url {
    String url;
    String toString() const { return url; }
    operator String() const { return url; }
};

inline struct WiFi_ {


    WiFi_() { qDebug(""); }
    E getMode() {
        qDebug("");
        return {};
    }
    void mode(int) { qDebug(""); }
    void begin(String, String) { qDebug(""); }

    int status() {
        qDebug("");
        return WL_CONNECTED;
    }

    String SSID() {
        qDebug("");
        return {};
    }

    Url localIP() {
        qDebug("");
        return {};
    }
    Url softAPIP() {
        qDebug("");
        return {};
    }
    Url subnetMask() {
        qDebug("");
        return {};
    }
    Url gatewayIP() {
        qDebug("");
        return {};
    }

    String macAddress() {
        qDebug("");
        return {};
    }

    int RSSI() {
        qDebug("");
        return {};
    }
} WiFi;
