#include "viewerwidget.h"
#include <QRegularExpression>

QString ViewerWidget::highlightContent(const QString& content, const QStringList& keywords, int maxLength)
{
    QString snippet = content.left(maxLength);  // 可选截断内容
    return applyHighlight(snippet, keywords);
}

QString ViewerWidget::applyHighlight(const QString& text, const QStringList& keywords)
{
    QString result = text;
    for (const QString& word : keywords) {
        if (word.trimmed().isEmpty()) continue;

        QRegularExpression regex("(" + QRegularExpression::escape(word) + ")", QRegularExpression::CaseInsensitiveOption);
        result.replace(regex, "<mark>\\1</mark>");
    }
    return result;
}
