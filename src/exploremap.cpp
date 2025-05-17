#include "exploremap.h"
#include "ui_exploremap.h"
#include"graphviewitems.hpp"
#include<QMessageBox>
#include <QGraphicsTextItem>
#include <set>

ExploreMap::ExploreMap(Program* program, QWidget* parent)
    : QDialog(parent), ui(new Ui::ExploreMap), program(program) {
    ui->setupUi(this);
    ui->label->setText(QString::fromStdString(program->currentGraph->name));
    populateComboBoxes();

    // Set focus border style for all widgets inside this form
    this->setStyleSheet(R"(
        QWidget:focus {
            border: 2px solid #377DFF;
            border-radius: 4px;
        }
    )");
}

void ExploreMap::populateComboBoxes() {
    if (!program->currentGraph) return;

    const auto& cities = program->currentGraph->getAllCities();

    for (const auto& city : cities) {
        ui->city1->addItem(QString::fromStdString(city));
        ui->city2->addItem(QString::fromStdString(city));
    }

    ui->city1->setCurrentIndex(-1);
    ui->city2->setCurrentIndex(-1);
}

void ExploreMap::on_findPath_clicked() {
    if (!program->currentGraph) return;

    QString city1 = ui->city1->currentText();
    QString city2 = ui->city2->currentText();

    if (city1.isEmpty() || city2.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Source and destination cannot be empty.");
        return;
    }

    if (!ui->distance_rad->isChecked() && !ui->time_rad->isChecked()) {
        QMessageBox::warning(this, "Input Error", "Dijkstra's mode cannot be empty.");
        return;
    }

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
        showPath(shortestPath.path, 'd');
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
        summary << "| " << shortestPath.distanceOrTime << " hrs";
        pathResult.push_back(summary.str());
        showPath(shortestPath.path, 't');
    }

    QString output;
    for (const auto& part : pathResult) {
        output += QString::fromStdString(part) + " ";
    }
    ui->path->setText(output.trimmed());
}

void ExploreMap::showPath(const std::vector<std::string>& path, char mode) {
    // Validate input
    if (!program || !program->currentGraph || path.empty()) {
        qWarning() << "Invalid input for path visualization";
        return;
    }

    // Create scene
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    ui->visualizePath->setScene(scene);

    // Get all cities from the graph
    std::vector<std::string> allCities;
    for (const auto& [city, _] : program->currentGraph->adj) {
        allCities.push_back(city);
    }

    // Initialize node positions in a circle
    std::map<std::string, QPointF> positions;
    std::map<std::string, QPointF> velocities;
    const double radius = std::min(ui->visualizePath->width(), ui->visualizePath->height()) * 0.35;
    const double centerX = ui->visualizePath->width() / 2.0;
    const double centerY = ui->visualizePath->height() / 2.0;

    for (size_t i = 0; i < allCities.size(); i++) {
        const double angle = 2.0 * M_PI * i / allCities.size();
        positions[allCities[i]] = QPointF(
            centerX + radius * std::cos(angle),
            centerY + radius * std::sin(angle)
            );
        velocities[allCities[i]] = QPointF(0, 0);
    }

    // Calculate average edge value based on mode
    const auto& graph = program->currentGraph->adj;
    double totalValue = 0.0;
    int edgeCount = 0;

    for (const auto& [city, neighbors] : graph) {
        for (const auto& [neighbor, edgeData] : neighbors) {
            totalValue += (mode == 't') ? edgeData.second : edgeData.first;
            edgeCount++;
        }
    }

    const double avgValue = (edgeCount > 0) ? totalValue / edgeCount : 100.0;
    const double valueScale = 200.0 / avgValue;

    // Force-directed layout parameters
    const int ITERATIONS = 150;
    const double REPULSION = 10000.0;
    const double SPRING_K = 0.002;
    const double DAMPING = 0.85;
    const double PADDING = 50.0;

    // Run force-directed layout
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::map<std::string, QPointF> forces;

        // Initialize forces to zero
        for (const auto& city : allCities) {
            forces[city] = QPointF(0, 0);
        }

        // Calculate repulsive forces
        for (const auto& city1 : allCities) {
            for (const auto& city2 : allCities) {
                if (city1 != city2) {
                    QPointF delta = positions[city1] - positions[city2];
                    double distance = std::max(1.0, std::hypot(delta.x(), delta.y()));
                    double repulsion = std::min(REPULSION / (distance * distance), 200.0);
                    forces[city1] += delta * (repulsion / distance);
                }
            }
        }

        // Calculate attractive forces (springs)
        for (const auto& [city, neighbors] : graph) {
            for (const auto& [neighbor, edgeData] : neighbors) {
                QPointF delta = positions[city] - positions[neighbor];
                double distance = std::max(1.0, std::hypot(delta.x(), delta.y()));
                double edgeValue = (mode == 't') ? edgeData.second : edgeData.first;
                double idealDist = edgeValue * valueScale;
                double spring = -SPRING_K * (distance - idealDist);
                forces[city] += delta * (spring / distance);
            }
        }

        // Update positions with damping
        const double maxSpeed = (iter < ITERATIONS / 2) ? 10.0 : 5.0;
        for (const auto& city : allCities) {
            velocities[city] = (velocities[city] + forces[city]) * DAMPING;

            // Limit maximum speed
            double speed = std::hypot(velocities[city].x(), velocities[city].y());
            if (speed > maxSpeed) {
                velocities[city] *= (maxSpeed / speed);
            }

            // Update position with bounds checking
            positions[city] += velocities[city];
            positions[city].setX(std::clamp(positions[city].x(), PADDING, ui->visualizePath->width() - PADDING));
            positions[city].setY(std::clamp(positions[city].y(), PADDING, ui->visualizePath->height() - PADDING));
        }

        // Additional damping in late iterations
        if (iter > ITERATIONS * 0.7) {
            for (auto& [_, vel] : velocities) {
                vel *= 0.95;
            }
        }
    }

    // Scale and center the layout
    double minX = std::numeric_limits<double>::max();
    double minY = minX;
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = maxX;

    for (const auto& [_, pos] : positions) {
        minX = std::min(minX, pos.x());
        minY = std::min(minY, pos.y());
        maxX = std::max(maxX, pos.x());
        maxY = std::max(maxY, pos.y());
    }

    const double scaleX = (maxX > minX) ? (ui->visualizePath->width() - 100) / (maxX - minX) : 1.0;
    const double scaleY = (maxY > minY) ? (ui->visualizePath->height() - 100) / (maxY - minY) : 1.0;
    const double scale = std::min(scaleX, scaleY);

    if (scale < 0.9 || scale > 1.1) {
        const double centerX = (minX + maxX) / 2;
        const double centerY = (minY + maxY) / 2;
        const double viewCenterX = ui->visualizePath->width() / 2;
        const double viewCenterY = ui->visualizePath->height() / 2;

        for (auto& [_, pos] : positions) {
            pos = QPointF(
                (pos.x() - centerX) * scale + viewCenterX,
                (pos.y() - centerY) * scale + viewCenterY
                );
        }
    }

    // Draw all nodes
    for (const auto& city : allCities) {
        const QPointF pos = positions.at(city);
        CityNode* node = new CityNode(QRectF(pos.x()-15, pos.y()-15, 30, 30),
                                      QString::fromStdString(city));

        // Highlight nodes in path
        if (std::find(path.begin(), path.end(), city) != path.end()) {
            node->setBrush(Qt::yellow);
        }

        scene->addItem(node);

        // Add city label
        QGraphicsTextItem* label = scene->addText(QString::fromStdString(city));
        label->setPos(pos.x() - label->boundingRect().width() / 2,
                      pos.y() + 20);
    }

    // Draw all edges
    std::set<std::pair<std::string, std::string>> drawnEdges;

    for (const auto& [city, neighbors] : graph) {
        for (const auto& [neighbor, edgeData] : neighbors) {
            // Skip if already drawn
            if (drawnEdges.count({neighbor, city})) continue;

            const QPointF from = positions.at(city);
            const QPointF to = positions.at(neighbor);

            // Create edge
            EdgeLine* edge = new EdgeLine(QLineF(from.x(), from.y(), to.x(), to.y()),
                                          QString::fromStdString(city),
                                          QString::fromStdString(neighbor));

            // Check if edge is part of the path
            for (size_t i = 0; i + 1 < path.size(); ++i) {
                if ((path[i] == city && path[i+1] == neighbor) ||
                    (path[i] == neighbor && path[i+1] == city)) {
                    edge->setPen(QPen(Qt::yellow, 3));
                    break;
                }
            }

            scene->addItem(edge);

            // Add edge label (distance or time)
            const double value = (mode == 't') ? edgeData.second : edgeData.first;
            const QString unit = (mode == 't') ? " hrs" : " km";
            QGraphicsTextItem* label = scene->addText(QString::number(value) + unit);

            // Position label perpendicular to edge
            const QPointF mid = (from + to) / 2;
            QLineF edgeLine(from, to);
            QLineF normal = edgeLine.normalVector();
            normal.setLength(15);
            label->setPos(mid + QPointF(normal.dx(), normal.dy()));
            label->setDefaultTextColor(Qt::white);

            drawnEdges.insert({city, neighbor});
        }
    }
}


ExploreMap::~ExploreMap()
{
    delete ui;
}
