#include <QRegularExpression>
#include <QSyntaxHighlighter>

#pragma once

class XmlHighlighter : public QSyntaxHighlighter {
    // Q_OBJECT
public:
    explicit XmlHighlighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text);
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
