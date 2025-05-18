#ifndef EXPLOREMAP_H
#define EXPLOREMAP_H

#include <QDialog>
#include"graph.hpp"
#include"program.hpp"
using namespace std;
namespace Ui {
class ExploreMap;
}

class ExploreMap : public QDialog
{
    Q_OBJECT

public:
    explicit ExploreMap(Program* program, QWidget* parent = nullptr);
    void populateComboBoxes();
    ~ExploreMap();
    struct City {
        string name;
        int x, y; // screen coordinates
    };

private slots:
    void on_findPath_clicked();
    void showPath(const vector<string>& highlightPath, char mode);

private:
    Ui::ExploreMap *ui;
     Program* program;
};

#endif // EXPLOREMAP_H
