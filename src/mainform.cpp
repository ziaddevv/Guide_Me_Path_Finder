#include "mainform.h"
#include "ui_mainform.h"
#include "mainwindow.h"

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    ui->label_2->setStyleSheet("QLabel { color : Orange; }");

    // Set focus border style for all widgets inside this form
    this->setStyleSheet(R"(
        QWidget:focus {
            border: 2px solid #377DFF;
            border-radius: 4px;
        }
    )");
}

void MainForm::on_btnSelectMap_clicked() {
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
