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
    ~ExploreMap();

private:
    Ui::ExploreMap *ui;
     Program* program;
};

#endif // EXPLOREMAP_H
