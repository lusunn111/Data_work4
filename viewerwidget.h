#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QString>
#include <QStringList>

class ViewerWidget
{
public:
    ViewerWidget() = default;

    // 高亮内容（全文或摘要）
    QString highlightContent(const QString& content, const QStringList& keywords, int maxLength = 300);

private:
    QString applyHighlight(const QString& text, const QStringList& keywords);
};

#endif // VIEWERWIDGET_H
