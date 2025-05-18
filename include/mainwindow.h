#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<math.h>
#include <QCloseEvent>
#include <QMainWindow>
#include "graphviewitems.hpp"
#include <vector>
#include "graph.hpp"
#include "filehandler.hpp"
#include "qmessagebox.h"
#include  "QGraphicsScene"
#include "QGraphicsView"
#include"QGraphicsEllipseItem"
#include "QGraphicsLineItem"
#include"QLineEdit"
#include"QInputDialog"
#include "set"
#include "program.hpp"
#include"ui_exploremap.h"
#include"exploremap.h"
#include"ui_editGraph.h"
#include <QTimer>
#include <QMap>
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
    void animateTraversal(const vector<string>& path);
    void resetGraphColors();
private slots:
    void onMapSelectionChanged(int index);
    void ShowMap(int index);
    void on_exploreButton_clicked();
    void on_addGraphButton_clicked();
    void on_deleteGraphButton_clicked();
    void updateGraphComboBox();
    void on_BFS_clicked();
    void on_DFS_clicked();
    void on_editGraph_clicked();

    void on_saveBtn_clicked();
    void animateTraversalStep();
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::MainWindow *ui;
    Program program;
    QTimer* animationTimer;
    QVector<QString> animationPath;
    int currentAnimationStep;
    QMap<QString, CityNode*> cityNodes;
    QMap<QPair<QString, QString>, EdgeLine*> edgeLines;
};
#endif // MAINWINDOW_H
