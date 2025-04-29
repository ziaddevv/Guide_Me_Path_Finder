#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.hpp"
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

private:
    Ui::MainWindow *ui;
   Graph g;
};
#endif // MAINWINDOW_H
