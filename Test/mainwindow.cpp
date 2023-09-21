#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDomDocument>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QWebEngineView>
#include <QXmlStreamReader>
#include <chrono>
#include <stack>
#include <tuple>

void action();
void build();
void rndLabel();
void rndComp();

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
#include <array>
#include <format>
#include <iostream>
#include <map>
#include <vector>

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

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GP::GyverPortal ui;

// connect(comboBox, QOverload<const QString &>::of(&QComboBox::activated),
//     [=](const QString &text){ /* ... */ });
// Согласитесь, это будет гораздо проще для восприятия, чем нечто такое из предыдущего урока по сигналам и слотам

// connect(m_testClass, static_cast<void(TestClass::*)(int)>(&TestClass::testSignal), this, static_cast<void(Widget::*)(int)>(&Widget::onTestSlot));
// Давайте перепишем этот пример с использованием QOverload

// connect(m_testClass, QOverload<int>::of(&TestClass::testSignal), this, QOverload<int>::of(&Widget::onTestSlot));
// Запись стала не только короче, но и более понятной.

struct Flag {
};

template <typename Ty, typename... Ts>
struct Printable : std::tuple<Ts*...>, Flag {
    static constexpr auto Size = sizeof...(Ts);
    //    enum {
    //        Size = sizeof...(Ts)
    //    };
    using tuple = std::tuple<Ts*...>;
    Printable(Printable&&) = delete;
    Printable(const Printable&) = delete;
    void build() {
        print_impl(*static_cast<Ty*>(this), std::make_index_sequence<Size>{});
    }

private:
    template <typename _Ty, size_t... Is>
    static void print_impl(_Ty& t, std::index_sequence<Is...>) {
        if constexpr (std::is_base_of_v<Flag, _Ty>) {
            t.begin();
            ([](auto* val) {
                using T = std::decay_t<decltype(*val)>;
                if constexpr (std::is_base_of_v<Flag, T>) {
                    print_impl(*val, std::make_index_sequence<T::Size>{});
                } else
                    print_impl(*val, std::index_sequence<>{});
            }(std::get<Is>(t)),
                ...);
            t.end();
        } else {
            GP::GP.PUT_OBJ(t);
        }
    }
};

template <typename Ty, typename... Ts>
Printable(Ty, Ts...) -> Printable<Ty, Ts...>;

/// \brief The BOX class
template <typename... Ts>
struct BOX : Printable<BOX<Ts...>, Ts...> {
    using P = Printable<BOX<Ts...>, Ts...>;
    BOX(Ts&&... ts)
        : P{{&ts...}} { }
    void begin() const { GP::GP.BOX_BEGIN(); }
    void end() const { GP::GP.BOX_END(); }
};

template <typename... Ts>
BOX(Ts&&...) -> BOX<Ts...>;

/// \brief The GRID class
template <typename... Ts>
struct GRID : Printable<GRID<Ts...>, Ts...> {
    using P = Printable<GRID<Ts...>, Ts...>;
    GRID(Ts&&... ts)
        : P{{&ts...}} { }
    void begin() const { GP::GP.GRID_BEGIN(); }
    void end() const { GP::GP.GRID_END(); }
};

template <typename... Ts>
GRID(Ts&&...) -> GRID<Ts...>;

/// \brief The BLOCK_THIN_TAB class
template <typename... Ts>
struct BLOCK_THIN_TAB : Printable<BLOCK_THIN_TAB<Ts...>, Ts...> {
    using P = Printable<BLOCK_THIN_TAB<Ts...>, Ts...>;
    const String& name;
    BLOCK_THIN_TAB(const String& name, Ts&&... ts)
        : P{{&ts...}}
        , name{name} { }
    void begin() const { GP::GP.BLOCK_THIN_TAB_BEGIN(name); }
    void end() const { GP::GP.BLOCK_END(); }
};

template <typename... Ts>
BLOCK_THIN_TAB(const String&, Ts&&...) -> BLOCK_THIN_TAB<Ts...>;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP());

    ui.start();
    ui.attachBuild([] {
        GP::GP.BUILD_BEGIN();
        GP::GP.THEME(GP::DARK);
        *GP::tmpPageBuf += "<!--//////////////////////////////////////////-->\n";

        GRID(
            BLOCK_THIN_TAB(
                "Котел",
                BLOCK_THIN_TAB(
                    "Отопление",
                    BOX(GP::LABEL("", "Темп. прямой"), GP::LABEL("", "30", "t_in")),
                    BOX(GP::LABEL("", "Темп. обратн"), GP::LABEL("", "22", "t_out"))),
                BLOCK_THIN_TAB(
                    "Горячая вода",
                    BOX(GP::LABEL("", "Темп. хол."), GP::LABEL("", "23", "t_cold")),
                    BOX(GP::LABEL("", "Темп. гор."), GP::LABEL("", "24", "t_hot"))),
                BOX(GP::LABEL("", "Нагрев вкл"), GP::LED("boilerOn", false))))
            .build();

        GRID(
            BLOCK_THIN_TAB(
                "UPS",
                BOX(GP::LABEL("", "Вход"), GP::NUMBER("v_in", "", 221)),
                BOX(GP::LABEL("", "Выход"), GP::NUMBER("v_out", "", 220))))
            .build();

        *GP::tmpPageBuf += "<!--//////////////////////////////////////////-->\n";

        GP::GP.BUILD_END();
    });

    ui.attach([](GP::GyverPortal& p) {
        //        if (p.click("clk"))
        //            Serial.println("Button click");
        //        if (p.click("ext")) {
        //            Serial.println("Exit portal");
        //            p.stop();
        //        }
    });
}

void loop() {
    // ui.tick();
}

class XmlHighlighter : public QSyntaxHighlighter {
    // Q_OBJECT
public:
    explicit XmlHighlighter(QTextDocument* parent = nullptr)
        : QSyntaxHighlighter{parent} {
        HighlightingRule rule;

        // Double quotes
        quotationFormat.setForeground(Qt::blue);
        rule.pattern = QRegularExpression(R"(".*")");
        //        rule.pattern.setMinimal(true); // Match the shortest first
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        // element name
        propertyFormat.setForeground(Qt::red);
        rule.pattern = QRegularExpression(R"(\b[A-Za-z0-9_]+[\s]*(?=\=))");
        //        rule.pattern.setMinimal(true);
        rule.format = propertyFormat;
        highlightingRules.append(rule);

        // element name
        elementNameFormat.setForeground(Qt::blue);
        rule.pattern = QRegularExpression(R"(<[\/]*[A-Za-z0-9_]+\b|>)");
        //        rule.pattern.setMinimal(true);
        rule.format = elementNameFormat;
        highlightingRules.append(rule);

        // Multi-line comment
        multiLineCommentFormat.setForeground(Qt::darkGreen);
        commentStartExpression = QRegularExpression("<!--");
        commentEndExpression = QRegularExpression("-->");
    }

protected:
    void highlightBlock(const QString& text) {
        for (const auto& rule: highlightingRules) {
            auto globalMatch = rule.pattern.globalMatch(text);
            while (globalMatch.hasNext()) {
                auto match = globalMatch.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }

        setCurrentBlockState(0);

        // match multi-line comments
        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = commentStartExpression.match(text).capturedStart();
        while (startIndex >= 0) {
            auto match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex + match.capturedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.match(text, startIndex + commentLength).capturedStart();
        }
    }
signals:

public slots:
private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat elementNameFormat;
    QTextCharFormat propertyFormat;
};

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

    //    ui->verticalLayout->setSpacing(6);
    //    ui->verticalLayout->setContentsMargins(6, 6, 6, 6);
    //    ui->verticalLayout->setColumnStretch(0, 1);
    //    ui->verticalLayout->setColumnStretch(1, 1);

    view->load(QUrl("http://localhost:80"));

    //    connect(view->page()->action(QWebEnginePage::ViewSource), &QAction::triggered, [=] {
    //        view->page()->toHtml([textEdit](auto&& str) { textEdit->setPlainText(str); });
    //        //        view->page()->toHtml([textEdit](auto &&str){textEdit, &QTextEdit::setHtml(str);});
    //    });
    connect(view->page(), &QWebEnginePage::loadFinished, [=](bool ok) {
        using std::placeholders::_1;
        if (ok)
            //           view->page()->toHtml(std::bind(&QTextEdit::setPlainText, textEdit, _1));
            view->page()->toHtml([textEdit](const QString& xmlIn) {
                QString xmlOut;

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
                QDomDocument input;
                input.setContent(xmlIn);
                QDomDocument output(input);
                QTextStream stream(&xmlOut);
                output.save(stream, QDomNode::EncodingFromTextStream);
                textEdit->setPlainText(1 ? xmlIn : xmlOut);
            });
    });

    startTimer(1);

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
