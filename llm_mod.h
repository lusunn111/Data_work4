#ifndef LLM_MOD_H
#define LLM_MOD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

class llm_mod : public QObject
{
    Q_OBJECT

public:
    explicit llm_mod(QObject *parent = nullptr);

    // 向向量数据库增加文档
    QJsonObject addDocument(const QString &filePath);

    // 将文字转化为向量
    QJsonObject embedText(const QString &text);

    // 返回最相似的文档
    QJsonObject searchSimilar(const QString &question);

    // 返回 QA 链的思考过程和结果
    QJsonObject qaChain(const QString &question);

private:
    QNetworkAccessManager *networkManager;  // 用于发送 HTTP 请求

};

#endif // LLM_MOD_H
