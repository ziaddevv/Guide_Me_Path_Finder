#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Filehandler f;
    f.ReadGraphFromFile("C:\\Users\\Youssef Elshemy\\source\\repos\\wasalney_mini_Path_Finder\\filename.txt");
    graphs = f.graphs;

    // Populate map selection combo box
    for (const auto& graph : graphs) {
        ui->MapSelectionCmb->addItem(QString::fromStdString(graph.name));
    }

    // Connect map selection change signal
    connect(ui->MapSelectionCmb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onMapSelectionChanged);

    // Initialize with first map if available
    if (!graphs.empty()) {
        currentGraph = graphs[0];
        updateCityComboBoxes();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMapSelectionChanged(int index)
{
    if (index >= 0 && index < static_cast<int>(graphs.size())) {
        currentGraph = graphs[index];
        updateCityComboBoxes();
    }
}

void MainWindow::updateCityComboBoxes()
{
    // Clear existing items
    ui->FirstCityCmb->clear();
    ui->secondCityCmb->clear();
    ui->StartcityCmb->clear();

    // Add empty default item
    ui->FirstCityCmb->addItem("");
    ui->secondCityCmb->addItem("");
    ui->StartcityCmb->addItem("");

    // Get cities from current graph
    vector<string> cities = currentGraph.getAllCities();
    for (const auto& city : cities) {
        ui->FirstCityCmb->addItem(QString::fromStdString(city));
        ui->secondCityCmb->addItem(QString::fromStdString(city));
        ui->StartcityCmb->addItem(QString::fromStdString(city));
    }
}

void MainWindow::on_bfsBtn_clicked()
{
    ui->resultText2->clear();
    QString selectedOption = ui->StartcityCmb->currentText();
    if (selectedOption.isEmpty()) {
        ui->resultText2->setText("Please choose the start city");
        return;
    }
    ui->resultText2->setText(QString::fromStdString(currentGraph.BFS(selectedOption.toStdString())));
}

void MainWindow::on_dfsBtn_clicked()
{
    ui->resultText2->clear();
    QString selectedOption = ui->StartcityCmb->currentText();
    if (selectedOption.isEmpty()) {
        ui->resultText2->setText("Please choose the start city");
        return;
    }
    ui->resultText2->setText(QString::fromStdString(currentGraph.DFS(selectedOption.toStdString())));
}

void MainWindow::on_dijkstraBtn_clicked()
{
    ui->resultText->clear();
    QString selectedOption1 = ui->FirstCityCmb->currentText();
    QString selectedOption2 = ui->secondCityCmb->currentText();
    ui->chosenCities->setText(selectedOption1 + " → " + selectedOption2);
    if (selectedOption1.isEmpty() || selectedOption2.isEmpty()) {
        ui->resultText->setText("Please choose both cities");
        return;
    }
    ui->resultText->setText(QString::fromStdString(
        currentGraph.DijkstraDistance(selectedOption1.toStdString(), selectedOption2.toStdString())
    ));
}
























////////////////////////////
// Add cities
// g.addCity("Cairo");
// g.addCity("Giza");
// g.addCity("Alexandria");
// g.addCity("Aswan");
// g.addCity("Luxor");
// g.addCity("SharmElSheikh");
// g.addCity("Tanta");
// g.addCity("PortSaid");
// g.addCity("Suez");
// g.addCity("MarsaMatruh");
// g.addCity("Fayoum");
// g.addCity("Minya");

// // Add edges with distance (in km) and time (in hours)
// g.addEdge("Cairo", "Giza", 20.0, 0.5);
// g.addEdge("Cairo", "Alexandria", 220.0, 2.5);
// g.addEdge("Cairo", "Luxor", 650.0, 8.0);
// g.addEdge("Aswan", "Luxor", 200.0, 3.0);
// g.addEdge("SharmElSheikh", "Cairo", 500.0, 7.0);
// g.addEdge("Tanta", "Cairo", 100.0, 1.5);
// g.addEdge("PortSaid", "Cairo", 150.0, 2.0);
// g.addEdge("Suez", "Cairo", 130.0, 2.0);
// g.addEdge("MarsaMatruh", "Alexandria", 240.0, 3.5);
// g.addEdge("Fayoum", "Cairo", 130.0, 2.0);
// g.addEdge("Minya", "Cairo", 240.0, 4.0);
// g.addEdge("PortSaid", "Suez", 100.0, 1.5);
// g.addEdge("Luxor", "Tanta", 600.0, 7.5);
