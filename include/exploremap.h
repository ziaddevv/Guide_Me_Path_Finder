#ifndef EXPLOREMAP_H
#define EXPLOREMAP_H

#include <QDialog>
#include"graph.hpp"
#include"program.hpp"

namespace Ui {
class ExploreMap;
}

class ExploreMap : public QDialog
{
    Q_OBJECT

public:
    explicit ExploreMap(Program* program, QWidget* parent = nullptr);
    void populateComboBoxes();
    ~ExploreMap();

private slots:
    void on_findPath_clicked();
    void on_BFS_clicked();
    void on_DFS_clicked();
    void on_insertCity_clicked();
    void on_deleteCity_clicked();
    void on_insertEdge_clicked();
    void on_deleteEdge_clicked();

private:
    Ui::ExploreMap *ui;
     Program* program;
};

#endif // EXPLOREMAP_H
