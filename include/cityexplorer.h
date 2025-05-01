#ifndef CITYEXPLORER_H
#define CITYEXPLORER_H

#include <QDialog>
#include <QString>
#include "city.hpp" // for City class

namespace Ui {
class CityExplorer;
}

class CityExplorer : public QDialog
{
    Q_OBJECT

public:
    explicit CityExplorer(QWidget *parent = nullptr);
    ~CityExplorer();

    void setCityData(const City& city);

private:
    Ui::CityExplorer *ui;
     City currentCity;
};

#endif // CITYEXPLORER_H
