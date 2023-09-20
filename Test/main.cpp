#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char* argv[]) {

    qSetMessagePattern(QLatin1String(
        "%{if-debug}\033[38;2;128;128;128mD %{endif}" // QColor{128,128,128};
        "%{if-warning}\033[38;2;255;128;0mW %{endif}" // QColor{255,128,000};
        "%{if-critical}\033[38;2;255;0;0mC %{endif}"  // QColor{255,000,000};
        "%{if-fatal}\033[38;2;255;0;0mF %{endif}"     // QColor{128,128,128};
        "%{if-info}\033[38;2;128;255;255mI %{endif}"  // QColor{128,255,255};
        // "%{time HH:mm:ss.zzz} "
        // "%{appname} %{pid} %{threadid} "
        // "%{type} "
        // "%{file}:%{line} %{function} "
        "%{if-category}%{category} %{endif}%{message} "
        //"\x1b[0m"
        "\033[38;2;255;255;128m" // QColor{255,255,125};
        "\t%{function}\x1b[0m"));

    QApplication app(argc, argv);

    app.setApplicationName("GPTest");
    app.setOrganizationName("XrSoft");

    MainWindow w;
    w.show();

    qWarning("Application has started");
    return app.exec();
    qWarning("Application has stopped");
}
