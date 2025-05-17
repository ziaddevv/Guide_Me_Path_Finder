#include "editgraph.h"
#include "ui_editgraph.h"
#include<QMessageBox>

editGraph::editGraph(Program* program, QWidget* parent)
: QDialog(parent), ui(new Ui::editGraph), program(program)
{
    ui->setupUi(this);
    ui->label->setText(QString::fromStdString(program->currentGraph->name));
    populateComboBoxes();
    connect(ui->IC, &QPushButton::clicked, this, &editGraph::on_insertCity_clicked);
    connect(ui->DC, &QPushButton::clicked, this, &editGraph::on_deleteCity_clicked);
    connect(ui->IE, &QPushButton::clicked, this, &editGraph::on_insertEdge_clicked);
    connect(ui->DE, &QPushButton::clicked, this, &editGraph::on_deleteEdge_clicked);
}


void editGraph::populateComboBoxes() {
    if (!program->currentGraph) return;

    const auto& cities = program->currentGraph->getAllCities();

    QList<QComboBox*> comboBoxes = { ui->DCity, ui->IECity1,  ui->IECity2, ui->DECity1, ui->DECity2 };

    for (auto comboBox : comboBoxes) {
        comboBox->clear();
    }

    for (const auto& city : cities) {
        for (auto comboBox : comboBoxes) {
            comboBox->addItem(QString::fromStdString(city));
        }
    }

    for (auto comboBox : comboBoxes) {
        comboBox->setCurrentIndex(-1);
    }
}

void editGraph::on_insertCity_clicked(){
    QString cityName = ui->insertCity->text().trimmed();

    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "City name cannot be empty.");
        return;
    }

    if (!program->currentGraph->containsCity(cityName.toStdString())) {
        program->currentGraph->addCity(cityName.toStdString());
        QMessageBox::information(this, "Success", "City added successfully.");
        ui->insertCity->clear();

        populateComboBoxes();

        program->isModified = true;
    } else {
        QMessageBox::warning(this, "Duplicate", "City already exists in the graph.");
    }
}

void editGraph::on_deleteCity_clicked(){
    QString cityName = ui->DCity->currentText().trimmed();

    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "City name cannot be empty.");
        return;
    }

    if (program->currentGraph->containsCity(cityName.toStdString())) {
        program->currentGraph->deleteCity(cityName.toStdString());
        QMessageBox::information(this, "Success", "City deleted successfully.");

        populateComboBoxes();

        ui->DCity->setCurrentIndex(-1);
        program->isModified = true;
    } else {
        QMessageBox::warning(this, "Not Found", "City does not exist in the graph.");
    }
}

void editGraph::on_insertEdge_clicked() {
    QString city1 = ui->IECity1->currentText().trimmed();
    QString city2 = ui->IECity2->currentText().trimmed();

    bool timeOk, distanceOk;
    double time = ui->time->text().toDouble(&timeOk);
    double distance = ui->distance->text().toDouble(&distanceOk);

    if (city1.isEmpty() || city2.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select both cities.");
        return;
    }

    if (!timeOk || !distanceOk) {
        QMessageBox::warning(this, "Input Error", "Please enter valid numerical values for time and distance.");
        return;
    }

    if (city1 == city2) {
        QMessageBox::warning(this, "Input Error", "The two cities must be different.");
        return;
    }

    if (!program->currentGraph->containsCity(city1.toStdString()) || !program->currentGraph->containsCity(city2.toStdString())) {
        QMessageBox::warning(this, "City Error", "Both cities must exist in the graph.");
        return;
    }

    program->currentGraph->addEdge(city1.toStdString(), city2.toStdString(), distance, time);

    QMessageBox::information(this, "Success", "Edge inserted successfully.");

    ui->time->clear();
    ui->distance->clear();
    ui->IECity1->setCurrentIndex(-1);
    ui->IECity2->setCurrentIndex(-1);
    program->isModified = true;
}

void editGraph::on_deleteEdge_clicked() {
    QString city1 = ui->DECity1->currentText().trimmed();
    QString city2 = ui->DECity2->currentText().trimmed();

    if (city1.isEmpty() || city2.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select both cities.");
        return;
    }

    if (city1 == city2) {
        QMessageBox::warning(this, "Input Error", "The two cities must be different.");
        return;
    }

    if (!program->currentGraph->containsEdge(city1.toStdString(), city2.toStdString())) {
        QMessageBox::warning(this, "Edge Not Found", "No edge exists between the selected cities.");
        return;
    }

    program->currentGraph->deleteEdge(city1.toStdString(), city2.toStdString());

    QMessageBox::information(this, "Success", "Edge deleted successfully.");

    ui->DECity1->setCurrentIndex(-1);
    ui->DECity2->setCurrentIndex(-1);
    program->isModified = true;
}

editGraph::~editGraph()
{

    delete ui;
}
