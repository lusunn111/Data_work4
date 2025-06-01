#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "textprocessor.h"
#include "searchengine.h"
#include "llm_mod.h"
#include "viewerwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_searchButton_clicked();

private:
    Ui::MainWindow *ui;

    TextProcessor* processor;
    SearchEngine* searchEngine;
    llm_mod* llm;
};

#endif // MAINWINDOW_H
