#ifndef EXPLOREMAP_H
#define EXPLOREMAP_H

#include <QDialog>

namespace Ui {
class ExploreMap;
}

class ExploreMap : public QDialog
{
    Q_OBJECT

public:
    explicit ExploreMap(QWidget *parent = nullptr);
    ~ExploreMap();

private:
    Ui::ExploreMap *ui;
};

#endif // EXPLOREMAP_H
