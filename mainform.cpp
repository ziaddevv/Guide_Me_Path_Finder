#include "mainform.h"
#include "ui_mainform.h"
#include "mainwindow.h"

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    ui->label_2->setStyleSheet("QLabel { color : Orange; }");
}

void MainForm::on_btnSelectMap_clicked() {
    // Hide the current content (Screen 1)
    // ui->->hide();  // Assuming 'currentWidget' is the object name of the current content


    // Show the new content (Screen 2)
    // ui->mainwindow->show();  // Assuming 'newWidget' is the object name of the new content

    MainWindow* w = new MainWindow();
    this->hide();
    w->show();
}

void MainForm::on_btnClose_clicked(){
    this->close();
}


MainForm::~MainForm()
{
    delete ui;
}
