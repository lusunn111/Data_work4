#include "llm_mod.h"

llm_mod::llm_mod(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

// 向向量数据库增加文档
QJsonObject llm_mod::addDocument(const QString &filePath)
{
    QJsonObject json;
    json["file_path"] = filePath;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:5000/add_document"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, data);

    // 使用 QEventLoop 实现同步等待
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();

    QByteArray response = reply->readAll();
    reply->deleteLater();

    return QJsonDocument::fromJson(response).object();
}

// 将文字转化为向量
QJsonObject llm_mod::embedText(const QString &text)
{
    QJsonObject json;
    json["text"] = text;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:5000/embed_text"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, data);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray response = reply->readAll();
    reply->deleteLater();

    return QJsonDocument::fromJson(response).object();
}

// 返回最相似的文档
QJsonObject llm_mod::searchSimilar(const QString &question)
{
    QJsonObject json;
    json["question"] = question;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:5000/search_similar"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, data);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray response = reply->readAll();
    reply->deleteLater();

    return QJsonDocument::fromJson(response).object();
}

// 返回 QA 链的思考过程和结果
QJsonObject llm_mod::qaChain(const QString &question)
{
    QJsonObject json;
    json["question"] = question;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:5000/qa_chain"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, data);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray response = reply->readAll();
    reply->deleteLater();

    return QJsonDocument::fromJson(response).object();
}
