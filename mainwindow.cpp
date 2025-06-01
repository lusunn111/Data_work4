// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextCursor>
#include <QMessageBox>
#include <QJsonArray>
#include <QDebug>
#include <QRegularExpression>
#include "viewerwidget.h"

const int MAX_DOC = 10;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1500, 600);

    // 初始化 jieba 分词器
    cppjieba::Jieba* jieba = new cppjieba::Jieba(
        "C:/Users/mzh99/Desktop/Data_Structure_Big_Work/my_work/Project4/jiebacpp_ok/dict/jieba.dict.utf8",
        "C:/Users/mzh99/Desktop/Data_Structure_Big_Work/my_work/Project4/jiebacpp_ok/dict/hmm_model.utf8",
        "C:/Users/mzh99/Desktop/Data_Structure_Big_Work/my_work/Project4/jiebacpp_ok/dict/user.dict.utf8",
        "C:/Users/mzh99/Desktop/Data_Structure_Big_Work/my_work/Project4/jiebacpp_ok/dict/idf.utf8",
        "C:/Users/mzh99/Desktop/Data_Structure_Big_Work/my_work/Project4/jiebacpp_ok/dict/stop_words.utf8"
    );

    processor = new TextProcessor(*jieba);
    llm = new llm_mod(this);
    searchEngine = new SearchEngine(processor, llm);

    QString folderPath = QFileDialog::getExistingDirectory(this, "选择语料目录");
    if (folderPath.isEmpty()) return;

    searchEngine->loadCorpus(folderPath);

    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::on_searchButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete processor;
    delete llm;
    delete searchEngine;
}

void MainWindow::on_searchButton_clicked()
{
    QString query = ui->inputLine->text().trimmed();
    if (query.isEmpty()) return;

    // 获取语义向量
    QJsonObject emb = llm->embedText(query);
    QVector<float> queryVec = searchEngine->parseEmbedding(emb);

    // 分词
    std::string queryStd = query.toStdString();
    std::vector<std::string> tokens;
    processor->tokenize(queryStd, tokens);

    // 合并倒排检索结果文档集合
    std::set<int> allDocIds;
    for (const std::string& token : tokens) {
        auto ids = searchEngine->keywordSearch(QString::fromStdString(token));
        allDocIds.insert(ids.begin(), ids.end());
    }

    QStringList keywordList;
    for (const auto& token : tokens) {
        keywordList << QString::fromStdString(token);
    }

    auto ranked = searchEngine->rankDocuments(std::vector<int>(allDocIds.begin(), allDocIds.end()), queryVec, query);

    ui->resultBrowser->clear();

    ViewerWidget viewer;
    int count = 0;
    for (const auto& doc : ranked) {
        if (++count > 5) break; // 显示前5条

        // 展示关键词所在的上下文段落
        QStringList lines = doc.content.split("\n", Qt::SkipEmptyParts);
        QStringList matchedSnippets;
        for (const auto& line : lines) {
            for (const auto& word : keywordList) {
                if (line.contains(word, Qt::CaseInsensitive)) {
                    matchedSnippets << viewer.highlightContent(line, keywordList);
                    break;
                }
            }
            if (matchedSnippets.size() >= 3) break;
        }

        QString display = QString("<b>文档 %1</b> | 相似度: %2 | 词频得分: %3<br>%4<hr>")
                            .arg(doc.docId)
                            .arg(doc.semanticScore, 0, 'f', 3)
                            .arg(doc.keywordScore)
                            .arg(matchedSnippets.join("<br>"));

        ui->resultBrowser->append(display);
    }

    if (ranked.empty()) {
        ui->resultBrowser->setHtml("<i>未找到相关结果。</i>");
    }

    // 显示 LLM 回答内容
    QJsonObject sim = llm->searchSimilar(query);
    QString simContent;
    if (sim.contains("documents")) {
        QJsonArray docs = sim["documents"].toArray();
        for (const auto& doc : docs) {
            QJsonObject obj = doc.toObject();
            simContent += QString("📄 相似文档 %1：\n%2\n\n")
                .arg(obj["source"].toString())
                .arg(obj["content"].toString());
        }
    } else {
        simContent = "未找到相似文档\n";
    }

    QJsonObject qa = llm->qaChain(query);
    QString qaAnswer = qa.contains("answer") ? qa["answer"].toString() : "未找到答案";

    QString finalAnswer = "<b>🔍 相似文档：</b><br>" + simContent + "<hr><b>💬 问答链结果：</b><br>" + qaAnswer;
    ui->llmBrowser->setHtml(finalAnswer);
}
