#ifndef EDITGRAPH_H
#define EDITGRAPH_H

#include <QDialog>
#include"graph.hpp"
#include"program.hpp"

namespace Ui {
class editGraph;
}

class editGraph : public QDialog
{
    Q_OBJECT

public:
    explicit editGraph(Program* program, QWidget* parent = nullptr);
    void populateComboBoxes();
    ~editGraph();

private slots:
    void on_insertCity_clicked();
    void on_deleteCity_clicked();
    void on_insertEdge_clicked();
    void on_deleteEdge_clicked();

private:
    Ui::editGraph *ui;
     Program* program;
};

#endif // EDITGRAPH_H
