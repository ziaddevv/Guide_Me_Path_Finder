#include "cityexplorer.h"
#include "ui_cityexplorer.h"

CityExplorer::CityExplorer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CityExplorer)
{
    ui->setupUi(this);
}

CityExplorer::~CityExplorer()
{
    delete ui;
}

void CityExplorer::setCityData(const City& city)
{
    currentCity = city;  // save the city for later use
    QString cityInfo;
    cityInfo += "City Name: " + QString::fromStdString(city.name) + "\n";
    cityInfo += "---------------------------\n";

    for (const auto& [src, dests] : city.CityGraph_walk) {
        for (const auto& [dest, transport] : dests) {
            cityInfo += QString::fromStdString(src) + " -> " + QString::fromStdString(dest);
            cityInfo += " [Walk: " + QString::number(transport.Distance) + "km, " + QString::number(transport.time) + "min]\n";
        }
    }

    ui->lineEdit->setText(cityInfo);  // assuming QTextBrowser named textBrowser
}

