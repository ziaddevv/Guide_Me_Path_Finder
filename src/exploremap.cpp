#include "exploremap.h"
#include "ui_exploremap.h"

ExploreMap::ExploreMap(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExploreMap)
{
    ui->setupUi(this);
}

ExploreMap::~ExploreMap()
{
    delete ui;
}
