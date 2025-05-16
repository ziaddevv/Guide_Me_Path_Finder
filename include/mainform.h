#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>

namespace Ui {
class MainForm;
}

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

private slots:
    void on_btnSelectMap_clicked();
    void on_btnClose_clicked();

private:
    Ui::MainForm *ui;

};

#endif // MAINFORM_H
