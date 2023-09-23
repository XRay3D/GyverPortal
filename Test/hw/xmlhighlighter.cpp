#include "xmlhighlighter.h"

XmlHighlighter::XmlHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter{parent} {
    HighlightingRule rule;

    // Double quotes
    quotationFormat.setForeground(QColor{255, 170, 85});
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
    elementNameFormat.setForeground(QColor{85, 170, 255});
    rule.pattern = QRegularExpression(R"(<[\/]*[A-Za-z0-9_]+\b|>)");
    //        rule.pattern.setMinimal(true);
    rule.format = elementNameFormat;
    highlightingRules.append(rule);

    // Multi-line comment
    multiLineCommentFormat.setForeground(Qt::darkGreen);
    commentStartExpression = QRegularExpression("<!--");
    commentEndExpression = QRegularExpression("-->");
}

void XmlHighlighter::highlightBlock(const QString& text) {
    for(const auto& rule: highlightingRules) {
        auto globalMatch = rule.pattern.globalMatch(text);
        while(globalMatch.hasNext()) {
            auto match = globalMatch.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    // match multi-line comments
    int startIndex = 0;
    if(previousBlockState() != 1)
        startIndex = commentStartExpression.match(text).capturedStart();
    while(startIndex >= 0) {
        auto match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength;
        if(endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.match(text, startIndex + commentLength).capturedStart();
    }
}
