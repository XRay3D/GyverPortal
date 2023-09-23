#pragma once

#include "Arduino.h"
#include "WiFi.h"

struct DNSServer {
    DNSServer() { }
    void start(uint16_t, String, Url) { }
    void stop() { }
    void processNextRequest() { }
};

inline struct MDNS_ {
    MDNS_() { }
    void begin(String) { }
    void addService(String, String, uint16_t) { }
} MDNS;
