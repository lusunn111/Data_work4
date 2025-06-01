#include "searchengine.h"
#include <QDebug>
#include <cmath>
#include <QJsonArray>


SearchEngine::SearchEngine(TextProcessor* processor, llm_mod* llm)
    : processor(processor), llm(llm) {}

void SearchEngine::loadCorpus(const QString& folderPath)
{
    docIdToText = FileLoader::loadFromDirectory(folderPath);
    docIdToPath = FileLoader::loadPathsFromDirectory(folderPath);

    for (const auto& [docId, content] : docIdToText) {
        std::vector<QString> lines;
        processor->buildInvertedIndex(content, docId, invertedIndex, lines);

        QJsonObject obj = llm->embedText(content);
        docIdToEmbedding[docId] = parseEmbedding(obj);
    }

    qDebug() << "Corpus loaded. Documents:" << docIdToText.size();
}

std::vector<int> SearchEngine::keywordSearch(const QString& query)
{
    std::string queryStd = query.toStdString();
    std::vector<std::string> tokens;
    processor->tokenize(queryStd, tokens);

    std::set<int> resultSet;
    for (const auto& word : tokens) {
        if (invertedIndex.count(word)) {
            for (const auto& [docId, stats] : invertedIndex[word]) {
                resultSet.insert(docId);
            }
        }
    }

    return std::vector<int>(resultSet.begin(), resultSet.end());
}

std::vector<ScoredDoc> SearchEngine::rankDocuments(const std::vector<int>& docIds,
                                                   const QVector<float>& queryEmbedding,
                                                   const QString& queryText)
{
    std::string queryStd = queryText.toStdString();
    std::vector<std::string> tokens;
    processor->tokenize(queryStd, tokens);

    std::vector<ScoredDoc> results;

    for (int docId : docIds) {
        float freqScore = 0.0;
        for (const auto& word : tokens) {
            if (invertedIndex.count(word) && invertedIndex[word].count(docId)) {
                freqScore += invertedIndex[word][docId].freq;
            }
        }

        float semScore = cosineSimilarity(queryEmbedding, docIdToEmbedding[docId]);

        ScoredDoc doc;
        doc.docId = docId;
        doc.keywordScore = freqScore;
        doc.semanticScore = semScore;
        doc.content = docIdToText[docId];

        results.push_back(doc);
    }

    std::sort(results.begin(), results.end(), [](const ScoredDoc& a, const ScoredDoc& b) {
        return (0.5 * a.semanticScore + 0.5 * a.keywordScore)
             > (0.5 * b.semanticScore + 0.5 * b.keywordScore);
    });

    return results;
}

QVector<float> SearchEngine::parseEmbedding(const QJsonObject& obj)
{
    QVector<float> vec;
    QJsonArray arr = obj["embedding"].toArray();
    for (const auto& v : arr) {
        vec.append(static_cast<float>(v.toDouble()));
    }
    return vec;
}

float SearchEngine::cosineSimilarity(const QVector<float>& a, const QVector<float>& b)
{
    if (a.size() != b.size()) return 0;

    float dot = 0, normA = 0, normB = 0;
    for (int i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
        normA += a[i] * a[i];
        normB += b[i] * b[i];
    }

    return (normA == 0 || normB == 0) ? 0 : dot / (std::sqrt(normA) * std::sqrt(normB));
}
