#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->MapSelectionCmb->addItem("");
    for (const auto& graph : program.graphs) {
        ui->MapSelectionCmb->addItem(QString::fromStdString(graph.name));
    }


    connect(ui->MapSelectionCmb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onMapSelectionChanged);


    if (!program.graphs.empty()) {
        program.currentGraph = &program.graphs[0];
        updateCityComboBoxes();
        ShowMap(0);
    }
    // updateCityComboBoxes();
    // ShowMap(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_exploreButton_clicked()
{

    ExploreMap* exploreMap = new ExploreMap(&program, this);
    exploreMap->setAttribute(Qt::WA_DeleteOnClose);
    exploreMap->show();

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
    if(index==0)
    {
         program.currentGraph = NULL;
    }
    else if (index > 0 && index < static_cast<int>(program.graphs.size()+1)) {
        program.currentGraph = &program.graphs[index-1];
        updateCityComboBoxes();
        QGraphicsScene* scene = new QGraphicsScene(this);
        scene->setBackgroundBrush(Qt::lightGray);
        ui->graphicsView->setScene(scene);

        // Force-directed layout setup
        std::map<std::string, QPointF> positions;
        std::map<std::string, QPointF> velocities;

        // Initialize cities with spread-out positions
        int cityCount = program.currentGraph->getAllCities().size();
        if (cityCount > 0) {
            // Create initial positions in a circle for better starting distribution
            double radius = std::min(ui->graphicsView->width(), ui->graphicsView->height()) * 0.35;
            double centerX = ui->graphicsView->width() / 2.0;
            double centerY = ui->graphicsView->height() / 2.0;

            int i = 0;
            for (const auto& city : program.currentGraph->getAllCities()) {
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
        for (const auto& city : program.currentGraph->getAllCities()) {
            for (const auto& [neighbor, edgeData] : program.currentGraph->adj[city]) {
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
            for (const auto& city : program.currentGraph->getAllCities()) {
                forces[city] = QPointF(0, 0);
            }

            // Apply repulsive forces between all nodes
            for (const auto& city1 : program.currentGraph->getAllCities()) {
                for (const auto& city2 : program.currentGraph->getAllCities()) {
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
            for (const auto& city : program.currentGraph->getAllCities()) {
                for (const auto& [neighbor, edgeData] : program.currentGraph->adj[city]) {
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
            for (const auto& city : program.currentGraph->getAllCities()) {
                QPointF toCenter = QPointF(centerX, centerY) - positions[city];
                double dist = std::sqrt(toCenter.x() * toCenter.x() + toCenter.y() * toCenter.y());
                if (dist > 300) { // Only apply if far from center
                    forces[city] += toCenter * (0.01 * (dist - 300) / dist);
                }
            }

            // Update positions and velocities
            double maxSpeed = (iter < ITERATIONS / 2) ? 10.0 : 5.0; // Lower max speed later
            for (const auto& city : program.currentGraph->getAllCities()) {
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
        for (const auto& city : program.currentGraph->getAllCities()) {
            QPointF pos = positions[city];
            CityNode* node = new CityNode(QRectF(pos.x()-15, pos.y()-15, 30, 30), QString::fromStdString(city));
            scene->addItem(node);
            QGraphicsTextItem* label = scene->addText(QString::fromStdString(city));
            label->setPos(pos.x() - label->boundingRect().width()/2, pos.y() + 20);
        }

        // Draw edges
        std::set<std::pair<std::string, std::string>> drawn;
        for (const auto& city : program.currentGraph->getAllCities()) {
            for (const auto& [neighbor, edgeData] : program.currentGraph->adj[city]) {
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
    vector<string> cities = program.currentGraph->getAllCities();
    for (const auto& city : cities) {
        ui->FirstCityCmb->addItem(QString::fromStdString(city));
        ui->secondCityCmb->addItem(QString::fromStdString(city));
        // ui->StartcityCmb->addItem(QString::fromStdString(city));
    }
}




void MainWindow::on_addGraphButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add New Graph"),
                                         tr("Graph name:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !name.isEmpty()) {
        program.addGraph(name.toStdString());
        updateGraphComboBox();  // Refresh the graph list in the UI
        QMessageBox::information(this, "Graph Added", "Graph added successfully.");
    }
}

void MainWindow::updateGraphComboBox() {
    ui->MapSelectionCmb->clear();
    ui->MapSelectionCmb->addItem("");
    for (const auto& g : program.graphs) {
        ui->MapSelectionCmb->addItem(QString::fromStdString(g.name));
    }
}

void MainWindow::on_deleteGraphButton_clicked()
{
    QString name = ui->MapSelectionCmb->currentText();
    if (name.isEmpty()) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Graph",
                                  "Are you sure you want to delete the graph '" + name + "'?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        bool deleted = program.deleteGraph(name.toStdString());
        if (deleted) {
            updateGraphComboBox();
            ui->graphicsView->scene()->clear();
            QMessageBox::information(this, "Deleted", "Graph deleted.");
        } else {
            QMessageBox::warning(this, "Error", "Graph not found.");
        }
    }
}

