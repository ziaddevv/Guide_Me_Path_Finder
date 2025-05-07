#include "program.hpp"

Program::Program() {
    f.ReadGraphFromFile("C:\\Users\\DELL\\Documents\\wasalney_mini\\filename.txt");
    f.ReadCityGraphsFromFile("C:\\Users\\DELL\\Documents\\wasalney_mini\\CitiesGraphs.txt");
    graphs = f.graphs;
}
void Program::addGraph(string name)
{

}
