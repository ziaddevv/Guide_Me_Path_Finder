#include "filehandler.hpp"
#include <QFile>
#include<QTextStream>
#include<QMessageBox>
#include <vector>
Filehandler::Filehandler() {}

void Filehandler::setGraphs(const std::vector<Graph>& g) {
    this->graphs = g;
}
void Filehandler::ReadGraphFromFile(const string& filename)
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Failed to open file: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    graphs.clear();

    bool conversionOk = false;
    numberOfGraphs = in.readLine().toInt(&conversionOk);
    if (!conversionOk) {
        QMessageBox::critical(nullptr, "Error", "Invalid file format: Expected number of graphs");
        file.close();
        return;
    }

    try {
        while (!in.atEnd()) {
            string name = in.readLine().toStdString();
            if (name.empty()) continue;

            Graph g;
            g.name = name;

            while (!in.atEnd()) {
                QString line = in.readLine();

                if (line == "#") break;

                QStringList parts = line.split(" ");
                if (parts.size() < 4) {
                    throw runtime_error("Invalid edge format. Expected: source destination distance time");
                }

                QString src = parts[0];
                QString dest = parts[1];

                g.addCity(src.toStdString());


                if (dest == "ISOLATED") {

                    continue;
                }

                g.addCity(dest.toStdString());

                bool distanceOk = false, timeOk = false;
                double distance = parts[2].toDouble(&distanceOk);
                double time = parts[3].toDouble(&timeOk);

                if (!distanceOk || !timeOk) {
                    throw runtime_error("Invalid numeric values for distance or time");
                }

                g.addEdge(src.toStdString(), dest.toStdString(), distance, time);
            }
            graphs.push_back(g);
        }
    } catch (const exception& e) {
        QMessageBox::critical(nullptr, "Error", "Error parsing file: " + QString::fromStdString(e.what()));
    }
    file.close();
}
void Filehandler::SaveInFile(const string& filename)
{
    qDebug() << "Number of graphs to save:" << graphs.size();
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(nullptr, "Error", "Failed to open file for writing: " + file.errorString());
        return;
    }
    QTextStream out(&file);

    out << graphs.size() << Qt::endl;

    for (const Graph& g : graphs) {

        out << QString::fromStdString(g.name) << Qt::endl;


        std::unordered_set<std::string> processedCities;

        for (const auto& srcPair : g.adj) {
            const string& source = srcPair.first;

            for (const auto& destPair : srcPair.second) {
                const string& destination = destPair.first;
                double distance = destPair.second.first;
                double time = destPair.second.second;

                if (source < destination) {
                    out << QString::fromStdString(source) << " "
                        << QString::fromStdString(destination) << " "
                        << distance << " "
                        << time << Qt::endl;
                }


                processedCities.insert(source);
                processedCities.insert(destination);
            }
        }

        for (const auto& srcPair : g.adj) {
            const string& city = srcPair.first;


            if (srcPair.second.empty()) {

                out << QString::fromStdString(city) << " ISOLATED 0 0" << Qt::endl;
            }
        }


        out << "#" << Qt::endl;
    }
    file.close();
}
