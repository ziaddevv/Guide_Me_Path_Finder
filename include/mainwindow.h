#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "graph.hpp"
#include "filehandler.hpp"
#include "cityexplorer.h"
#include "qmessagebox.h"
#include  "QGraphicsScene"
#include "QGraphicsView"
#include"QGraphicsEllipseItem"
#include "QGraphicsLineItem"
#include "set"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bfsBtn_clicked();
    void on_dfsBtn_clicked();
    void on_dijkstraBtn_clicked();
    void onMapSelectionChanged(int index);

    void on_exploreButton_clicked();

private:
    void updateCityComboBoxes();
    Ui::MainWindow *ui;
    std::vector<Graph> graphs;
       // Stores all loaded graphs
    Graph currentGraph;
            // Currently selected graph
     Filehandler f;
};
#endif // MAINWINDOW_H
