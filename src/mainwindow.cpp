#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     this->f.ReadGraphFromFile("C:\\Users\\DELL\\Documents\\wasalney_mini\\filename.txt");
     f.ReadCityGraphsFromFile("C:\\Users\\DELL\\Documents\\wasalney_mini\\CitiesGraphs.txt");
     graphs = f.graphs;


     ui->MapSelectionCmb->addItem(QString::fromStdString(""));
    for (const auto& graph : graphs) {
        ui->MapSelectionCmb->addItem(QString::fromStdString(graph.name));
   }



    connect(ui->MapSelectionCmb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onMapSelectionChanged);


    if (!graphs.empty()) {
        currentGraph = graphs[0];
        updateCityComboBoxes();
        ShowMap(0);// this is the default one
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_exploreButton_clicked()
{

    QString selectedCityName = ui->secondCityCmb->currentText();

    // Corrected the parentheses issue here
    //ui->resultText2->setText(selectedCityName);
    if (f.citiesGraphs.find(selectedCityName.toStdString()) != f.citiesGraphs.end()) {
        CityExplorer* explorer = new CityExplorer(this);
        explorer->setCityData(f.citiesGraphs[selectedCityName.toStdString()]);
        explorer->exec();  // or show() if you want it modeless
    } else {
        QMessageBox::warning(this, "Error", "City not found!");
    }
}
float max(float a,double b)
{
    if(a>b)
        return a;
    else
        return b;
}
void MainWindow::ShowMap(int index)
{
    if (index > 0 && index < static_cast<int>(graphs.size()+1)) {
        currentGraph = graphs[index-1];
        updateCityComboBoxes();
        QGraphicsScene* scene = new QGraphicsScene(this);
        scene->setBackgroundBrush(Qt::lightGray);
        ui->graphicsView->setScene(scene);

        // Force-directed layout setup
        std::map<std::string, QPointF> positions;
        std::map<std::string, QPointF> velocities;

        // Initialize cities with spread-out positions
        int cityCount = currentGraph.getAllCities().size();
        if (cityCount > 0) {
            // Create initial positions in a circle for better starting distribution
            double radius = std::min(ui->graphicsView->width(), ui->graphicsView->height()) * 0.35;
            double centerX = ui->graphicsView->width() / 2.0;
            double centerY = ui->graphicsView->height() / 2.0;

            int i = 0;
            for (const auto& city : currentGraph.getAllCities()) {
                // Position cities in a circle
                double angle = 2.0 * M_PI * i / cityCount;
                double x = centerX + radius * std::cos(angle);
                double y = centerY + radius * std::sin(angle);
                positions[city] = QPointF(x, y);
                velocities[city] = QPointF(0, 0);
                i++;
            }
        }

        // Calculate the average edge distance to use for scaling
        double totalDistance = 0.0;
        int edgeCount = 0;
        for (const auto& city : currentGraph.getAllCities()) {
            for (const auto& [neighbor, edgeData] : currentGraph.adj[city]) {
                totalDistance += edgeData.first;
                edgeCount++;
            }
        }
        double avgDistance = (edgeCount > 0) ? totalDistance / edgeCount : 100.0;
        double distanceScale = 200.0 / avgDistance; // Scale factor to make average edge ~200px

        // Run force-directed algorithm iterations
        const int ITERATIONS = 150;
        const double REPULSION = 10000.0;
        const double SPRING_K = 0.002;
        const double DAMPING = 0.85;

        for (int iter = 0; iter < ITERATIONS; iter++) {
            // Calculate forces for each node
            std::map<std::string, QPointF> forces;

            // Initialize forces to zero
            for (const auto& city : currentGraph.getAllCities()) {
                forces[city] = QPointF(0, 0);
            }

            // Apply repulsive forces between all nodes
            for (const auto& city1 : currentGraph.getAllCities()) {
                for (const auto& city2 : currentGraph.getAllCities()) {
                    if (city1 != city2) {
                        QPointF delta = positions[city1] - positions[city2];
                        double distance = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
                        if (distance < 1.0) distance = 1.0; // Avoid division by very small numbers

                        // Repulsive force is inversely proportional to distance
                        double repulsionStrength = REPULSION / (distance * distance);
                        // Limit maximum repulsion
                        repulsionStrength = std::min(repulsionStrength, 200.0);

                        if (distance > 0) {
                            QPointF repulsion = delta * (repulsionStrength / distance);
                            forces[city1] += repulsion;
                        }
                    }
                }
            }

            // Apply spring forces for edges
            for (const auto& city : currentGraph.getAllCities()) {
                for (const auto& [neighbor, edgeData] : currentGraph.adj[city]) {
                    QPointF delta = positions[city] - positions[neighbor];
                    double distance = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
                    if (distance < 1.0) distance = 1.0;

                    // Spring force proportional to difference between actual and desired distance
                    double edgeLength = edgeData.first;  // Distance in km
                    double idealDist = edgeLength * distanceScale;

                    // Apply nonlinear spring force
                    double springFactor = -SPRING_K * (distance - idealDist);
                    if (distance > 0) {
                        QPointF spring = delta * (springFactor / distance);
                        forces[city] += spring;
                    }
                }
            }

            // Add a small force toward the center to prevent flying away
            double centerX = ui->graphicsView->width() / 2.0;
            double centerY = ui->graphicsView->height() / 2.0;
            for (const auto& city : currentGraph.getAllCities()) {
                QPointF toCenter = QPointF(centerX, centerY) - positions[city];
                double dist = std::sqrt(toCenter.x() * toCenter.x() + toCenter.y() * toCenter.y());
                if (dist > 300) { // Only apply if far from center
                    forces[city] += toCenter * (0.01 * (dist - 300) / dist);
                }
            }

            // Update positions and velocities
            double maxSpeed = (iter < ITERATIONS / 2) ? 10.0 : 5.0; // Lower max speed later
            for (const auto& city : currentGraph.getAllCities()) {
                velocities[city] = (velocities[city] + forces[city]) * DAMPING;

                // Limit maximum velocity
                double speed = std::sqrt(velocities[city].x() * velocities[city].x() +
                                         velocities[city].y() * velocities[city].y());
                if (speed > maxSpeed) {
                    velocities[city] *= (maxSpeed / speed);
                }

                positions[city] += velocities[city];

                // Ensure nodes stay within bounds with some padding
                double viewWidth = ui->graphicsView->width();
                double viewHeight = ui->graphicsView->height();
                double padding = 50.0;
                positions[city].setX(std::max(padding, std::min(viewWidth - padding, positions[city].x())));
                positions[city].setY(std::max(padding, std::min(viewHeight - padding, positions[city].y())));
            }

            // Slow things down in later iterations
            if (iter > ITERATIONS * 0.7) {
                for (auto& [city, vel] : velocities) {
                    vel *= 0.95;
                }
            }
        }

        // Scale the layout to fit better in the view
        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double maxY = std::numeric_limits<double>::lowest();

        for (const auto& [city, pos] : positions) {
            minX = std::min(minX, pos.x());
            minY = std::min(minY, pos.y());
            maxX = std::max(maxX, pos.x());
            maxY = std::max(maxY, pos.y());
        }

        double scaleX = (maxX > minX) ? (ui->graphicsView->width() - 100) / (maxX - minX) : 1.0;
        double scaleY = (maxY > minY) ? (ui->graphicsView->height() - 100) / (maxY - minY) : 1.0;
        double scale = std::min(scaleX, scaleY);

        // Apply scaling and centering if needed
        if (scale < 0.9 || scale > 1.1) {
            double centerX = (minX + maxX) / 2;
            double centerY = (minY + maxY) / 2;
            double viewCenterX = ui->graphicsView->width() / 2;
            double viewCenterY = ui->graphicsView->height() / 2;

            for (auto& [city, pos] : positions) {
                // Center and scale
                double newX = (pos.x() - centerX) * scale + viewCenterX;
                double newY = (pos.y() - centerY) * scale + viewCenterY;
                positions[city] = QPointF(newX, newY);
            }
        }

        // Draw nodes (cities)
        for (const auto& city : currentGraph.getAllCities()) {
            QPointF pos = positions[city];
            CityNode* node = new CityNode(QRectF(pos.x()-15, pos.y()-15, 30, 30), QString::fromStdString(city));
            scene->addItem(node);
            QGraphicsTextItem* label = scene->addText(QString::fromStdString(city));
            label->setPos(pos.x() - label->boundingRect().width()/2, pos.y() + 20);
        }

        // Draw edges
        std::set<std::pair<std::string, std::string>> drawn;
        for (const auto& city : currentGraph.getAllCities()) {
            for (const auto& [neighbor, edgeData] : currentGraph.adj[city]) {
                if (drawn.count({neighbor, city}) == 0) {
                    QPointF from = positions[city];
                    QPointF to = positions[neighbor];
                    EdgeLine* edge = new EdgeLine(QLineF(from.x(), from.y(), to.x(), to.y()),
                                                  QString::fromStdString(city),
                                                  QString::fromStdString(neighbor));
                    scene->addItem(edge);

                    // Add distance label with offset
                    QPointF mid = (from + to) / 2;
                    QLineF line(from, to);
                    QLineF normal = line.normalVector();
                    normal.setLength(15);
                    QPointF labelPos = mid + QPointF(normal.dx(), normal.dy());

                    QGraphicsTextItem* distLabel = scene->addText(QString::number(edgeData.first) + " km");
                    distLabel->setPos(labelPos);
                    distLabel->setDefaultTextColor(Qt::darkRed);

                    drawn.insert({city, neighbor});
                    drawn.insert({neighbor, city});
                }
            }
        }
    }
}
void MainWindow::onMapSelectionChanged(int index)
{
    ShowMap(index);
}


void MainWindow::updateCityComboBoxes()
{
    // Clear existing items
    ui->FirstCityCmb->clear();
    ui->secondCityCmb->clear();
   // ui->StartcityCmb->clear();

    // Add empty default item
    ui->FirstCityCmb->addItem("");
    ui->secondCityCmb->addItem("");
   // ui->StartcityCmb->addItem("");

    // Get cities from current graph
    vector<string> cities = currentGraph.getAllCities();
    for (const auto& city : cities) {
        ui->FirstCityCmb->addItem(QString::fromStdString(city));
        ui->secondCityCmb->addItem(QString::fromStdString(city));
       // ui->StartcityCmb->addItem(QString::fromStdString(city));
    }
}

void MainWindow::on_bfsBtn_clicked()
{
   // // ui->resultText2->clear();
   // / QString selectedOption = ui->StartcityCmb->currentText();
   //  if (selectedOption.isEmpty()) {
   //     // ui->resultText2->setText("Please choose the start city");
   //      return;
   //  }
   // // ui->resultText2->setText(QString::fromStdString(currentGraph.BFS(selectedOption.toStdString())));


}

void MainWindow::on_dfsBtn_clicked()
{
   // // ui->resultText2->clear();
   //  //QString selectedOption = ui->StartcityCmb->currentText();
   //  if (selectedOption.isEmpty()) {
   //   //   ui->resultText2->setText("Please choose the start city");
   //      return;
   //  }
   // // ui->resultText2->setText(QString::fromStdString(currentGraph.DFS(selectedOption.toStdString())));
}

void MainWindow::on_dijkstraBtn_clicked()
{
   // ui->resultText->clear();
   //  QString selectedOption1 = ui->FirstCityCmb->currentText();
   //  QString selectedOption2 = ui->secondCityCmb->currentText();
   //  //ui->chosenCities->setText(selectedOption1 + " → " + selectedOption2);
   //  if (selectedOption1.isEmpty() || selectedOption2.isEmpty()) {
   //  //    ui->resultText->setText("Please choose both cities");
   //      return;
   //  }
   // // ui->resultText->setText(QString::fromStdString(
   //      //currentGraph.DijkstraDistance(selectedOption1.toStdString(), selectedOption2.toStdString())
   //  ));
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



