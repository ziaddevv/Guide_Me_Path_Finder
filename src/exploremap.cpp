#include "exploremap.h"
#include "ui_exploremap.h"

ExploreMap::ExploreMap(Program* program, QWidget* parent)
    : QDialog(parent), ui(new Ui::ExploreMap), program(program) {
    ui->setupUi(this);
    ui->lineEdit->setText(QString::fromStdString(program->currentGraph->name));
}
ExploreMap::~ExploreMap()
{
    delete ui;
}
