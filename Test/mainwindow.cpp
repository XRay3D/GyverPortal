#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDomDocument>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QWebEngineView>
#include <QXmlStreamReader>
#include <chrono>

void action();
void build();
void rndLabel();
void rndComp();

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
#include "../examples/builds/dynamicComponents/dynamicComponents.ino"

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
        for(const auto& rule: highlightingRules) {
            auto globalMatch = rule.pattern.globalMatch(text);
            while(globalMatch.hasNext()) {
                auto match = globalMatch.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }

        setCurrentBlockState(0);

        //        // match multi-line comments
        //        int startIndex = 0;
        //        if (previousBlockState() != 1)
        //            startIndex = commentStartExpression.indexIn(text);
        //        while (startIndex >= 0) {
        //            int endIndex = commentEndExpression.indexIn(text, startIndex);
        //            int commentLength;
        //            if (endIndex == -1) {
        //                setCurrentBlockState(1);
        //                commentLength = text.length() - startIndex;
        //            } else {
        //                commentLength = endIndex - startIndex
        //                    + commentEndExpression.matchedLength();
        //            }
        //            setFormat(startIndex, commentLength, multiLineCommentFormat);
        //            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        //        }
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
        if(ok)
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
                textEdit->setPlainText(0 ? xmlIn : xmlOut);
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
