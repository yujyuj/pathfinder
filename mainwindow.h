#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "graph.h"
#include "graphics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Graph graph;
    void paintEvent(QPaintEvent*) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_Graph_triggered();
    void on_actionClear_Graph_triggered();
    void on_actionExit_triggered();
    void on_actionDept_First_Search_triggered();
    void on_actionBreadth_First_Search_triggered();
    void on_actionA_triggered();
};

#endif // MAINWINDOW_H