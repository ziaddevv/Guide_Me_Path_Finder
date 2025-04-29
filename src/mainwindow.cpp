#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filehandler.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Filehandler f;
    f.ReadGraphFromFile("");
    ui->lineEdit->setText(QString::fromStdString(f.graphs[1].name));
    g=f.graphs[2];


    vector<string>cities=g.getAllCities();
    ui->FirstCityCmb->addItem(QString::fromStdString(""));
    ui->secondCityCmb->addItem(QString::fromStdString(""));
    ui->StartcityCmb->addItem(QString::fromStdString(""));
    for(auto &it:cities)
    {
        ui->FirstCityCmb->addItem(QString::fromStdString(it));
        ui->secondCityCmb->addItem(QString::fromStdString(it));
        ui->StartcityCmb->addItem(QString::fromStdString(it));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bfsBtn_clicked()
{


     ui->resultText2->clear();

     QString selectedOption = ui->StartcityCmb->currentText();
    if(selectedOption.isEmpty())
     {
         ui->resultText2->setText(QString::fromStdString("please choose the start city"));
        return ;
     }
      ui->resultText2->setText(QString::fromStdString(g.BFS(selectedOption.toStdString())));
}


void MainWindow::on_dfsBtn_clicked()
{
    ui->resultText2->clear();
     QString selectedOption = ui->StartcityCmb->currentText();
    if(selectedOption.isEmpty())
    {
        ui->resultText2->setText(QString::fromStdString("please choose the start city"));
        return ;
    }
     ui->resultText2->setText(QString::fromStdString(g.DFS(selectedOption.toStdString())));
}


void MainWindow::on_dijkstraBtn_clicked()
{
    ui->resultText->clear();

      QString selectedOption1 = ui->FirstCityCmb->currentText();
      QString selectedOption2 = ui->secondCityCmb->currentText();
      ui->chosenCities->setText(QString::fromStdString( selectedOption1.toStdString()+" ------>"+selectedOption2.toStdString()));
      if(selectedOption1.isEmpty()||selectedOption2.isEmpty())
      {
          ui->resultText->setText(QString::fromStdString("please choose the start city"));
          return ;
      }
      ui->resultText->setText(QString::fromStdString(g.DijkstraDistance(selectedOption1.toStdString(),selectedOption2.toStdString())));
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
