#include "textprocessor.h"
#include <QList>

TextProcessor::TextProcessor(const cppjieba::Jieba& jieba) : jieba(jieba) {}

void TextProcessor::tokenize(const std::string& text, std::vector<std::string>& tokens) {
    jieba.Cut(text, tokens);
}

void TextProcessor::buildInvertedIndex(const QString &text,
                                       int docId,
                                       InvertedIndex &invertedIndex,
                                       std::vector<QString> &lines)
{
    QList<QString> liness = text.split('\n');

    for (const QString &line : liness)
        lines.push_back(line);

    int lineNumber = 0;
    int charOffset = 0;

    for (const QString& line : lines) {
        lineNumber++;
        std::string lineStd = line.toStdString();
        std::vector<std::string> tokens;
        tokenize(lineStd, tokens);

        for (const auto& token : tokens) {
            WordStats& stats = invertedIndex[token][docId];
            stats.freq++;
            stats.lineNumbers.insert(lineNumber);
            stats.charOffsets.insert(charOffset); // 粗略记录起始字符位
        }

        charOffset += line.length() + 1; // +1 是换行符
    }
}
