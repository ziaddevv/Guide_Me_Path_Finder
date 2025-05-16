#include "filehandler.hpp"
#include <QFile>
#include<QTextStream>
#include<QMessageBox>
#include <vector>
Filehandler::Filehandler() {}
 string Filehandler::ReadFile()
{
    QFile file("C:\\Users\\Youssef Elshemy\\source\\repos\\wasalney_mini_Path_Finder\\filename.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "info", file.errorString());
        return "";
    }
    QTextStream in(&file);
    return in.readAll().toStdString();
}

void  Filehandler::ReadCityGraphsFromFile(const string& filename){
 QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Failed to open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    citiesGraphs.clear();
     bool conversionOk = false;
     numOfCitiesInFile  = in.readLine().toInt(&conversionOk);
     if (!conversionOk) {
         QMessageBox::critical(nullptr, "Error", "Invalid file format: Expected number of graphs");
         file.close();
         return;
     }
     try {

         while (!in.atEnd()) {
             string city = in.readLine().toStdString();
             if (city.empty()) continue;

             City c;
             c.name=city;
             //////////////////////
             while (!in.atEnd()) {
                 QString line = in.readLine();

                 // Check if this is the end of the current graph --> each graph ends with #
                 if (line == "#") break;

                 QStringList parts = line.split(" ");
                 if (parts.size() < 5) {
                     throw runtime_error("Invalid edge format. Expected: source destination distance time");
                 }

                 QString src = parts[0];
                 QString dest = parts[1];

                 bool distanceOk = false, timeOk = false;
                 double distance = parts[2].toDouble(&distanceOk);
                 double time = parts[3].toDouble(&timeOk);
                 QString transport=parts[4];

                 if (!distanceOk || !timeOk) {
                     throw runtime_error("Invalid numeric values for distance or time");
                 }
                 if(transport=="car")
                 {
                     c.addNode_car(src.toStdString(),dest.toStdString(),distance,time);
                 }else if(transport=="walk")
                 {
                     c.addNode_walk(src.toStdString(),dest.toStdString(),distance,time);
                 }else
                 {
                     c.addNode_walk(src.toStdString(),dest.toStdString(),distance,time);
                     c.addNode_car(src.toStdString(),dest.toStdString(),distance,time);
                 }
                 // g.addCity(src.toStdString());
                 // g.addCity(dest.toStdString());
                 // g.addEdge(src.toStdString(), dest.toStdString(), distance, time);

             }

             citiesGraphs[city]=c;

         }
     } catch (const exception& e) {
         QMessageBox::critical(nullptr, "Error", "Error parsing file: " + QString::fromStdString(e.what()));
     }

     file.close();
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

                // Check if this is the end of the current graph --> each graph ends with #
                if (line == "#") break;

                QStringList parts = line.split(" ");
                if (parts.size() < 4) {
                    throw runtime_error("Invalid edge format. Expected: source destination distance time");
                }

                QString src = parts[0];
                QString dest = parts[1];

                bool distanceOk = false, timeOk = false;
                double distance = parts[2].toDouble(&distanceOk);
                double time = parts[3].toDouble(&timeOk);

                if (!distanceOk || !timeOk) {
                    throw runtime_error("Invalid numeric values for distance or time");
                }

                g.addCity(src.toStdString());
                g.addCity(dest.toStdString());
                g.addEdge(src.toStdString(), dest.toStdString(), distance, time);
            }

            graphs.push_back(g);
        }
    } catch (const exception& e) {
        QMessageBox::critical(nullptr, "Error", "Error parsing file: " + QString::fromStdString(e.what()));
    }

    file.close();
}
