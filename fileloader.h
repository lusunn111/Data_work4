#ifndef FILELOADER_H
#define FILELOADER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <map>

class FileLoader
{
public:
    // 加载目录下的所有 .txt 文件，并返回 docId → 内容映射
    static std::map<int, QString> loadFromDirectory(const QString& folderPath);

    // 可选：返回 docId → 文件路径映射（用于 UI 显示）
    static std::map<int, QString> loadPathsFromDirectory(const QString& folderPath);
};

#endif // FILELOADER_H
