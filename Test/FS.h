#pragma once

#include "Arduino.h"
#include <QFile>

struct File {
    operator bool() const { return false; }
    bool isDirectory() const { return false; }
    File openNextFile() const { return {}; }

    String path() const { return {}; }
    String name() const { return {}; }
    int size() const { return {}; }
};

namespace fs {

struct FS {
    FS() { }

    File open(String str)
    {
        File f;
        // File f(str);
        // f.open(QIODevice::ReadWrite);
        return f;
    }
};

}
