#include "exploremap.h"
#include "ui_exploremap.h"
#include<QMessageBox>

ExploreMap::ExploreMap(Program* program, QWidget* parent)
    : QDialog(parent), ui(new Ui::ExploreMap), program(program) {
    ui->setupUi(this);
    ui->label->setText(QString::fromStdString(program->currentGraph->name));
    populateComboBoxes();
    connect(ui->findPath, &QPushButton::clicked, this, &ExploreMap::on_findPath_clicked);
    connect(ui->BFS, &QPushButton::clicked, this, &ExploreMap::on_BFS_clicked);
    connect(ui->DFS, &QPushButton::clicked, this, &ExploreMap::on_DFS_clicked);
    connect(ui->IC, &QPushButton::clicked, this, &ExploreMap::on_insertCity_clicked);
    connect(ui->DC, &QPushButton::clicked, this, &ExploreMap::on_deleteCity_clicked);
    connect(ui->IE, &QPushButton::clicked, this, &ExploreMap::on_insertEdge_clicked);
    connect(ui->DE, &QPushButton::clicked, this, &ExploreMap::on_deleteEdge_clicked);
}

void ExploreMap::populateComboBoxes() {
    if (!program->currentGraph) return;

    const auto& cities = program->currentGraph->getAllCities();

    QList<QComboBox*> comboBoxes = { ui->city1, ui->city2, ui->start, ui->DCity, ui->IECity1,  ui->IECity2, ui->DECity1, ui->DECity2 };

    for (const auto& city : cities) {
        for (auto comboBox : comboBoxes) {
            comboBox->addItem(QString::fromStdString(city));
        }
    }

    for (auto comboBox : comboBoxes) {
        comboBox->setCurrentIndex(-1);
    }
}

void ExploreMap::on_findPath_clicked() {
    if (!program->currentGraph) return;

    QString city1 = ui->city1->currentText();
    QString city2 = ui->city2->currentText();

    if (city1.isEmpty() || city2.isEmpty()) return;

    if (!ui->distance_rad->isChecked() && !ui->time_rad->isChecked()) return;

    vector<string> pathResult;
    if (ui->distance_rad->isChecked()){
        auto shortestPath = program->currentGraph->DijkstraDistance(city1.toStdString(), city2.toStdString());
        if (shortestPath.path.empty()) {
            ui->path->setText("No path found.");
            return;
        }

        for (size_t i = 0; i < shortestPath.path.size(); ++i) {
            pathResult.push_back(shortestPath.path[i]);
            if (i + 1 < shortestPath.path.size()) {
                pathResult.push_back("-->");
            }
        }
        std::ostringstream summary;
        summary << "| " << shortestPath.distanceOrTime << " Km";
        pathResult.push_back(summary.str());
    }

    if (ui->time_rad->isChecked()){
        auto shortestPath = program->currentGraph->DijkstraTime(city1.toStdString(), city2.toStdString());
        if (shortestPath.path.empty()) {
            ui->path->setText("No path found.");
            return;
        }

        for (size_t i = 0; i < shortestPath.path.size(); ++i) {
            pathResult.push_back(shortestPath.path[i]);
            if (i + 1 < shortestPath.path.size()) {
                pathResult.push_back("-->");
            }
        }
        std::ostringstream summary;
        summary << "| " << shortestPath.distanceOrTime << " h Using Car";
        pathResult.push_back(summary.str());
    }

    QString output;
    for (const auto& part : pathResult) {
        output += QString::fromStdString(part) + " ";
    }
    ui->path->setText(output.trimmed());
}

void ExploreMap::on_BFS_clicked(){
    if (!program->currentGraph) return;

    QString start = ui->start->currentText();

    if (start.isEmpty()) return;

    auto graph = program->currentGraph->BFS(start.toStdString());

    if (graph.empty()) {
        ui->path->setText("No path found.");
        return;
    }

    QString result;
    for (size_t i = 0; i < graph.size(); ++i) {
        result += QString::fromStdString(graph[i]);
        if (i + 1 < graph.size())
            result += " --> ";
    }

    ui->traversal->setText(result);
}

void ExploreMap::on_DFS_clicked(){
    if (!program->currentGraph) return;

    QString start = ui->start->currentText();

    if (start.isEmpty()) return;

    auto graph = program->currentGraph->DFS(start.toStdString());

    if (graph.empty()) {
        ui->path->setText("No path found.");
        return;
    }

    QString result;
    for (size_t i = 0; i < graph.size(); ++i) {
        result += QString::fromStdString(graph[i]);
        if (i + 1 < graph.size())
            result += " --> ";
    }

    ui->traversal->setText(result);
}

void ExploreMap::on_insertCity_clicked(){
    QString cityName = ui->insertCity->text().trimmed();

    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "City name cannot be empty.");
        return;
    }

    if (!program->currentGraph->containsCity(cityName.toStdString())) {
        program->currentGraph->addCity(cityName.toStdString());
        QMessageBox::information(this, "Success", "City added successfully.");
        ui->insertCity->clear();

        QList<QComboBox*> comboBoxes = { ui->city1, ui->city2, ui->start, ui->DCity, ui->IECity1,  ui->IECity2, ui->DECity1, ui->DECity2 };
        for (auto comboBox : comboBoxes) {
                comboBox->addItem(cityName);
        }
    } else {
        QMessageBox::warning(this, "Duplicate", "City already exists in the graph.");
    }
}

void ExploreMap::on_deleteCity_clicked(){
    QString cityName = ui->DCity->currentText().trimmed();

    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "City name cannot be empty.");
        return;
    }

    if (program->currentGraph->containsCity(cityName.toStdString())) {
        program->currentGraph->deleteCity(cityName.toStdString());
        QMessageBox::information(this, "Success", "City deleted successfully.");

        QList<QComboBox*> comboBoxes = { ui->city1, ui->city2, ui->start, ui->DCity, ui->IECity1,  ui->IECity2, ui->DECity1, ui->DECity2 };
        for (auto comboBox : comboBoxes) {
            int index = comboBox->findText(cityName);
            if (index != -1) {
                comboBox->removeItem(index);
            }
        }
        ui->DCity->setCurrentIndex(-1);
    } else {
        QMessageBox::warning(this, "Not Found", "City does not exist in the graph.");
    }
}

void ExploreMap::on_insertEdge_clicked() {
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

    ui->IECity1->setCurrentIndex(-1);
    ui->IECity2->setCurrentIndex(-1);
}

void ExploreMap::on_deleteEdge_clicked() {
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
}

ExploreMap::~ExploreMap()
{
    delete ui;
}
