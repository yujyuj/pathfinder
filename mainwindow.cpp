#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle( "Path Finder" );
    Graphics graphics(this);
    graph = Graph(graphics);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*) {

    QPainter painter(this);

    QPen pen(QColor(qRgb(85, 85, 85)),2);
    painter.setPen(pen);
    painter.drawRect(27, 47, 506, 506); // Outer frame
    painter.drawRect(27, 47+2+506, 506, 100); // lower frame
    painter.fillRect(27, 47+2+506, 506, 100, QColor(qRgb(85, 85, 85)));

    painter.setPen(QColor(qRgb(255, 229, 204)));
    for (int row=0; row<Graph::GRAPH_EDGE; row++) {
        for (int col=0; col<Graph::GRAPH_EDGE; col++) {

            int upper_left_x = row * 10 + 30;
            int upper_left_y = col * 10 + 50;

            painter.drawRect(upper_left_x, upper_left_y, 10, 10 );  // cell

            switch(graph.graph()[row][col]) {
                case static_cast<int>(Graph::cell_values::unvisited):
                    painter.fillRect(upper_left_x, upper_left_y, 10, 10, QColor(qRgb(255, 255, 255)));
                    break;
                case static_cast<int>(Graph::cell_values::wall):
                    painter.fillRect(upper_left_x, upper_left_y, 10, 10, QColor(qRgb(85, 85, 85)));
                    break;
                case static_cast<int>(Graph::cell_values::source):
                    painter.fillRect(upper_left_x, upper_left_y, 10, 10, QColor(qRgb(25, 201, 69)));
                    break;
                case static_cast<int>(Graph::cell_values::destination):
                    painter.fillRect(upper_left_x, upper_left_y, 10, 10, QColor(qRgb(255, 13, 13)));
                    break;
                case static_cast<int>(Graph::cell_values::visited):
                    painter.fillRect(upper_left_x, upper_left_y, 10, 10, QColor(qRgb(158,216,213)));
                    break;
                case static_cast<int>(Graph::cell_values::path):
                    painter.fillRect(upper_left_x, upper_left_y, 10, 10, QColor(qRgb(254, 182, 0)));
                    break;
                default:
                    painter.fillRect(upper_left_x, upper_left_y, 10, 10, QColor(qRgb(255, 255, 255)));
                    break;
            }
        }
    }
}


void MainWindow::on_actionNew_Graph_triggered() {
    ui->label_algoname->clear();
    ui->label_distance->clear();
    graph.new_graph();
}


void MainWindow::on_actionClear_Graph_triggered() {
    ui->label_algoname->clear();
    ui->label_distance->clear();
    graph.clear_graph();
}


void MainWindow::on_actionExit_triggered() { 
    QApplication::quit();
}


void MainWindow::on_actionDept_First_Search_triggered() {
    ui->label_algoname->setText("Depth First Search path length");
    ui->label_distance->setText("?");

    auto[distance, path] = graph.dfs();

    if (distance == -1) {
        ui->label_algoname->setText("Depth First Search: no path found");
        ui->label_distance->clear();
    } else {
        ui->label_distance->setText(QString::number(distance));
    }
}


void MainWindow::on_actionBreadth_First_Search_triggered() { 
    ui->label_algoname->setText("Breadth First Search path length");
    ui->label_distance->setText("?");

    auto[distance, path] = graph.bfs();

    if (distance == -1) {
        ui->label_algoname->setText("Breadth First Search: no path found");
        ui->label_distance->clear();
    } else {
        ui->label_distance->setText(QString::number(distance));
    }
}


void MainWindow::on_actionA_triggered() { 

}
