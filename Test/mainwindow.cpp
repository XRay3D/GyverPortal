#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebEngineView>
#include <chrono>

void action();
void build();

// #include "../examples/demos/actionClick/actionClick.ino"
//#include "../examples/demos/actionForm/actionForm.ino"
// #include "../examples/demos/actionUpdate/actionUpdate.ino"
// #include "../examples/demos/buttonHold/buttonHold.ino"
// #include "../examples/demos/canvas/canvas.ino"
// #include "../examples/demos/demoAllComponents/demoAllComponents.ino"
// #include "../examples/demos/demoAuth/demoAuth.ino"
// #include "../examples/demos/demoColor/demoColor.ino"
// #include "../examples/demos/demoLog/demoLog.ino"
// #include "../examples/demos/demoLogManual/demoLogManual.ino"
// #include "../examples/demos/esp32-stream/esp32-stream.ino"
// #include "../examples/demos/jQupdate/jQupdate.ino"
 #include "../examples/demos/objTest/objTest.ino"
// #include "../examples/demos/objects/objects.ino"
// #include "../examples/demos/onlineCheck/onlineCheck.ino"
// #include "../examples/demos/pageTitle/pageTitle.ino"
// #include "../examples/demos/plotAjax/plotAjax.ino"
// #include "../examples/demos/plotStatic/plotStatic.ino"
// #include "../examples/demos/plotStock/plotStock.ino"
// #include "../examples/demos/popupAlert/popupAlert.ino"
// #include "../examples/demos/popupConfirm/popupConfirm.ino"
// #include "../examples/demos/popupPrompt/popupPrompt.ino"
// #include "../examples/demos/saveSettings/saveSettings.ino"
// #include "../examples/demos/systemInfo/systemInfo.ino"
// #include "../examples/demos/tableWrandom/tableWrandom.ino"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setup();

    auto view = new QWebEngineView{this};
    ui->verticalLayout->addWidget(view);
    ui->verticalLayout->setSpacing(6);
    ui->verticalLayout->setContentsMargins(6, 6, 6, 6);

    // view->load(QUrl("https://ya.ru/"));
    view->load(QUrl("http://localhost:80"));

    startTimer(100);

    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("State").toByteArray());
}

MainWindow::~MainWindow() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent* /*event*/) {
    loop();
}
