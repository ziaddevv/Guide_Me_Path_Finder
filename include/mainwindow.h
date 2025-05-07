#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<math.h>
#include <QMainWindow>
#include "graphviewitems.hpp"
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
#include "program.hpp"
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
    void onMapSelectionChanged(int index);
    void ShowMap(int index);
    void on_exploreButton_clicked();

private:
    void updateCityComboBoxes();
    Ui::MainWindow *ui;
    Program program;
};
#endif // MAINWINDOW_H
