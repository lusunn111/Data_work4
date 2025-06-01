#include "fileloader.h"

std::map<int, QString> FileLoader::loadFromDirectory(const QString& folderPath)
{
    std::map<int, QString> docIdToContent;
    QDir dir(folderPath);
    if (!dir.exists()) return docIdToContent;

    QStringList filters;
    filters << "*.txt";

    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);
    int docId = 0;

    for (const QFileInfo &fileInfo : fileList) {
        QFile file(fileInfo.absoluteFilePath());
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            docIdToContent[docId++] = content;
            file.close();
        }
    }

    return docIdToContent;
}

std::map<int, QString> FileLoader::loadPathsFromDirectory(const QString& folderPath)
{
    std::map<int, QString> docIdToPath;
    QDir dir(folderPath);
    if (!dir.exists()) return docIdToPath;

    QStringList filters;
    filters << "*.txt";

    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);
    int docId = 0;

    for (const QFileInfo &fileInfo : fileList) {
        docIdToPath[docId++] = fileInfo.absoluteFilePath();
    }

    return docIdToPath;
}
