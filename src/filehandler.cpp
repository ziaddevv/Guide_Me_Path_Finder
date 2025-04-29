#include "filehandler.hpp"
#include <QFile>
#include<QTextStream>
#include<QMessageBox>
#include <sstream>
#include <vector>
Filehandler::Filehandler() {}
 string Filehandler::ReadFile()
{
    QFile file("C:\\Users\\DELL\\Documents\\wasalney_mini\\filename.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "info", file.errorString());
        return "";
    }
    QTextStream in(&file);
    return in.readAll().toStdString();
}

void Filehandler::ReadGraphFromFile(const string& filename)
{
    QFile file("C:\\Users\\DELL\\Documents\\wasalney_mini\\filename.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "info", file.errorString());
        return;
    }

    QTextStream in(&file);
    numberOfGraphs = in.readLine().toInt();

    while (!in.atEnd()) {
        string line = in.readLine().toStdString();
        Graph g;
        g.name = line;


        do {
            QString cityToNeighbors = in.readLine();
            if (cityToNeighbors == "#") break;


            QStringList parts = cityToNeighbors.split(" ");
            QString src = parts[0];
            QString dest = parts[1];
            double distance = parts[2].toDouble();
            double time = parts[3].toDouble();


            g.addCity(src.toStdString());
            g.addCity(dest.toStdString());
            g.addEdge(src.toStdString(), dest.toStdString(), distance, time);

        } while (true);


        graphs.push_back(g);
    }

    file.close();
}

