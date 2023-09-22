#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xmlhighlighter.h"
#include <QDomDocument>
#include <QShortcut>
#include <QTextEdit>
#include <QWebEngineView>
#include <QXmlStreamReader>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <stack>
#include <tuple>
#include <vector>

void action();
void build();
void rndLabel();
void rndComp();
void startup();

// #include "../examples/builds/dynamicComponents/dynamicComponents.ino"
// #include "../examples/builds/selectBuild/selectBuild.ino"
// #include "../examples/customComponents/customComponents.ino"
// #include "../examples/demos/actionClick/actionClick.ino"
// #include "../examples/demos/actionForm/actionForm.ino"
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
// #include "../examples/demos/objTest/objTest.ino"
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
// #include "../examples/design/GridAndBlocks/GridAndBlocks.ino"
// #include "../examples/design/blocks/blocks.ino"
// #include "../examples/design/blocks2/blocks2.ino"
// #include "../examples/design/boxes/boxes.ino"
// #include "../examples/design/boxesMacro/boxesMacro.ino"
// #include "../examples/design/icons/icons.ino"
// #include "../examples/design/layoutApp/layoutApp.ino"
// #include "../examples/design/layoutDashboard/layoutDashboard.ino"
// #include "../examples/design/multiPageButtons/multiPageButtons.ino"
// #include "../examples/design/multiPageTabLinks/multiPageTabLinks.ino"
// #include "../examples/design/multiPageTabs/multiPageTabs.ino"
// #include "../examples/design/multiPageTabsManual/multiPageTabsManual.ino"
// #include "../examples/design/slidersEqual/slidersEqual.ino"
// #include "../examples/design/tables/tables.ino"
// #include "../examples/design/ui/ui.ino"
// #include "../examples/design/uiManual/uiManual.ino"
// #include "../examples/files/fileDownloadAuto/fileDownloadAuto.ino"
// #include "../examples/files/fileDownloadManual/fileDownloadManual.ino"
// #include "../examples/files/fileEmbed/fileEmbed.ino"
// #include "../examples/files/fileUploadAuto/fileUploadAuto.ino"
// #include "../examples/files/fileUploadManual/fileUploadManual.ino"
// #include "../examples/projects/eepromLogPlot/eepromLogPlot.ino"
// #include "../examples/projects/webSerial/webSerial.ino"
// #include "../examples/projects/wifiLogin/wifiLogin.ino"
// #include "../examples/system/OTAcustom/OTAcustom.ino"
// #include "../examples/system/OTAupdate/OTAupdate.ino"
// #include "../examples/system/accessNetIp/accessNetIp.ino"
// #include "../examples/system/localLambda/localLambda.ino"
// #include "../examples/system/localPortal/localPortal.ino"
// #include "../examples/system/localPortal2/localPortal2.ino"
// #include "../examples/system/rtos2core/rtos2core.ino"

/*

using std::cout;
using std::endl;
// using namespace std;

// std::array coil = {60, 126, 256, 375, 434, 1150, 2290, 1600};
//std::array coil = {6, 13, 26, 38, 43, 115, 229, 160};
 std::array coil = {
     5,
     12,
     24,
     36,
     110,
     220,
     380,
 };
std::map<int, int> map;

int main() {
    cout << "Hello World!" << endl;

    for (int i{coil.size() - 1}; i > 0; --i)
        coil[i] -= coil[i - 1];

    for (int val: coil)
        cout << val << endl;

    for (int i{}; i < (1 << coil.size() * 2); ++i) {
        int k{};
        for (int j{1}; int v: coil) {
            k += (j & i) ? v : 0;
            j <<= 1;
        }
        for (int j{(1 << coil.size())}; int v: coil) {
            if (!(j >> 7 & i))
                k -= (j & i) ? v : 0;
            j <<= 1;
        }
        if (k > 0 && !map.contains(k))
            map[k] = i;
    }

    for (int tmp{}; auto [val, i]: map) {
        cout << std::format("{:3} : {:014b} D {:3}\n", val, i, tmp - val);
        tmp = val;
    }

    return 0;
}

*/
#if 1

#include <GyverPortal.h>

GP::GyverPortal ui;

void build() {
    GP::GP.BUILD_BEGIN(600);
    GP::GP.THEME(GP::DARK);

    // включаем границы таблиц (ДЛЯ НАГЛЯДНОСТИ)
    GP::GP.TABLE_BORDER(true);

    // делаем таблицу вручную
    GP::GP.TITLE("Manual Table");
    GP::GP.TABLE_BEGIN();

    GP::GP.TR();
    GP::GP.TD();
    GP::GP.LABEL("Row 1");
    GP::GP.TD();
    GP::GP.CHECK("");

    GP::GP.TR();
    GP::GP.TD();
    GP::GP.LABEL("Row 2");
    GP::GP.TD();
    GP::GP.CHECK("");

    GP::GP.TABLE_END();

    //    // делаем таблицу с объединением ячеек
    //    GP::GP.TITLE("Span Table");
    //    GP::GP.TABLE_BEGIN();

    //    GP::GP.TR();
    //    GP::GP.TD(GP::Align::CENTER, 2);
    //    GP::GP.LABEL("Big table cell");

    //    GP::GP.TR();
    //    GP::GP.TD();
    //    GP::GP.LABEL("First");
    //    GP::GP.TD();
    //    GP::GP.LABEL("Second");

    //    GP::GP.TABLE_END();

    // авто таблица 1
    GP::GP.TITLE("Auto Table 1");
    GP::M_TABLE(
        GP::FOR(2, // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
            GP::M_TR(
                GP::LABEL("", String("Switch #")), GP::SWITCH(""), GP::SWITCH(""), GP::SWITCH(""), GP::SWITCH(""))))
        .build();

    GP::GP.TITLE("Auto Table 2");
    GP::M_TABLE(
        GP::M_TR( // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
            GP::LABEL("", String("Switch #")), GP::SWITCH(""), GP::SWITCH("")),
        GP::M_TR( // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
            GP::LABEL("", String("Switch #")), GP::SWITCH(""), GP::SWITCH("")),
        GP::M_TR( // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
            GP::LABEL("", String("Switch #")), GP::SWITCH(""), GP::SWITCH("")),
        GP::M_TR( // макрос создаёт РЯД таблицы, элементы ЧЕРЕЗ ЗАПЯТУЮ
            GP::LABEL("", String("Switch #")), GP::SWITCH(""), GP::SWITCH("")))
        .build();

    // авто таблица 2
    // с указанием размеров ячеек
    GP::GP.TITLE("Auto Table 2");
    GP::M_TABLE("30%,20%,50%",
        GP::FOR{5,
            GP::M_TR(
                GP::LABEL("", String("Label #")), GP::CHECK(""), GP::BUTTON("", "Btn"))

        })
        .build();

    // авто таблица 3
    // с указанием ориентации ячеек
    GP::GP.TITLE("Auto Table 3");
    GP::M_TABLE("", std::array{GP::Align::LEFT, GP::Align::CENTER, GP::Align::RIGHT},
        GP::FOR{3,
            GP::M_TR(GP::LABEL("", String("#")), GP::CHECK(""), GP::LED(""))})
        .build(); //);

    GP::GP.BUILD_END();
}

void setup() {
    Serial.begin(115200);
    ui.attachBuild(build);
    ui.start();
}

void loop() {
    ui.tick();
}

#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setup();

    auto view = new QWebEngineView{this};
    ui->splitter->addWidget(view);

    auto textEdit = new QTextEdit{this};
    new XmlHighlighter{textEdit->document()};
    textEdit->setFontFamily("JetBrains Mono Light");
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    ui->splitter->addWidget(textEdit);

    new QShortcut({"esc"}, this, [this] { close(); });

    //    ui->verticalLayout->setSpacing(6);
    //    ui->verticalLayout->setContentsMargins(6, 6, 6, 6);
    //    ui->verticalLayout->setColumnStretch(0, 1);
    //    ui->verticalLayout->setColumnStretch(1, 1);

    view->load(QUrl("http://localhost:1234"));

    //    connect(view->page()->action(QWebEnginePage::ViewSource), &QAction::triggered, [=] {
    //        view->page()->toHtml([textEdit](auto&& str) { textEdit->setPlainText(str); });
    //        //        view->page()->toHtml([textEdit](auto &&str){textEdit, &QTextEdit::setHtml(str);});
    //    });
    connect(view->page(), &QWebEnginePage::loadFinished, [=](bool ok) {
        using std::placeholders::_1;
        if (ok)
            //           view->page()->toHtml(std::bind(&QTextEdit::setPlainText, textEdit, _1));
            view->page()->toHtml([textEdit](const QString& xmlIn) {
                QString xmlOut{};
                QString tag;
                static QRegularExpression tagRr(R"((<(\w+))?.*(<\\(\w+)>)?)");
                static QRegularExpression tagRr2(R"(<[^\]\w+))");
                int i{-4};
                for (auto&& var: QString(xmlIn).replace("><", ">\n<").split('\n')) {
                    //                    if(var.startsWith('<') && !var.startsWith("</") && !var.startsWith("<meta")) ++i;
                    //                    if(var.contains("</")) --i;
                    xmlOut += QString(i * 3, ' ') + var + '\n';
                }

                //                QXmlStreamReader reader(xmlIn);
                //                QXmlStreamWriter writer(&xmlOut);
                //                try {
                //                    writer.setAutoFormatting(true);
                //                    while (!reader.atEnd()) {
                //                        reader.readNext();
                //                         if (!reader.isWhitespace() && reader.tokenString() != "Invalid") {
                //                            qCritical() << reader.tokenString() << reader.text();
                //                            writer.writeCurrentToken(reader);
                //                        } else {
                //                            qDebug() << reader.tokenString() << reader.text();
                //                        }
                //                    }
                //                } catch (...) {
                //                }
                //                QDomDocument input;
                //                input.setContent(xmlIn);
                //                QDomDocument output(input);
                //                QTextStream stream(&xmlOut);
                //                output.save(stream, QDomNode::EncodingFromTextStream);
                textEdit->setPlainText(0 ? xmlIn : xmlOut);
            });
    });

    startTimer(10);

    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("State").toByteArray());
    ui->splitter->restoreState(settings.value("Splitter").toByteArray());
}

MainWindow::~MainWindow() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    settings.setValue("Splitter", ui->splitter->saveState());
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent* /*event*/) {
    loop();
}
