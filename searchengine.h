#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QString>
#include <QVector>
#include <map>
#include <vector>
#include <string>
#include "textprocessor.h"
#include "llm_mod.h"
#include "fileloader.h"

// 文档评分结构
struct ScoredDoc {
    int docId;
    float semanticScore;
    float keywordScore;
    QString content;
};

class SearchEngine
{
public:
    SearchEngine(TextProcessor* processor, llm_mod* llm);

    void loadCorpus(const QString& folderPath);

    // 关键词倒排检索，返回候选文档 ID
    std::vector<int> keywordSearch(const QString& query);

    // 融合排序，返回最终结果列表
    std::vector<ScoredDoc> rankDocuments(const std::vector<int>& docIds,
                                         const QVector<float>& queryEmbedding,
                                         const QString& queryText);
    QVector<float> parseEmbedding(const QJsonObject& obj);

private:
    TextProcessor* processor;
    llm_mod* llm;

    InvertedIndex invertedIndex;
    std::map<int, QString> docIdToText;
    std::map<int, QString> docIdToPath;
    std::map<int, QVector<float>> docIdToEmbedding;


    float cosineSimilarity(const QVector<float>& a, const QVector<float>& b);
};

#endif // SEARCHENGINE_H
