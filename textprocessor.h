#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QString>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "cppjieba/Jieba.hpp"

struct WordStats {
    int freq = 0;
    std::set<int> lineNumbers;
    std::set<int> charOffsets; // 可选，可空
};

// 单词 -> 文档 ID -> WordStats
typedef std::map<std::string, std::map<int, WordStats>> InvertedIndex;

class TextProcessor
{
public:
    TextProcessor(const cppjieba::Jieba& jieba);

    void buildInvertedIndex(const QString &text,
                            int docId,
                            InvertedIndex &invertedIndex,
                            std::vector<QString> &lines);
    void tokenize(const std::string& text, std::vector<std::string>& tokens);

private:
    const cppjieba::Jieba& jieba;
//    void tokenize(const std::string& text, std::vector<std::string>& tokens);
};

#endif // TEXTPROCESSOR_H
