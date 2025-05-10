#include "program.hpp"

Program::Program() {
    loadGraphs(); // Load graphs during initialization
}

void Program::loadGraphs() {
    f.ReadGraphFromFile("C:\\Users\\Youssef Elshemy\\source\\repos\\wasalney_mini_Path_Finder\\filename.txt");
    f.ReadCityGraphsFromFile("C:\\Users\\Youssef Elshemy\\source\\repos\\wasalney_mini_Path_Finder\\CitiesGraphs.txt");
    graphs = f.graphs;
}

void Program::addGraph(const std::string& name) {
    Graph g;
    g.name = name;
    graphs.push_back(g);
}

bool Program::deleteGraph(const std::string& name) {
    auto it = std::remove_if(graphs.begin(), graphs.end(), [&](const Graph& g) {
        return g.name == name;
    });

    if (it != graphs.end()) {
        if (currentGraph && currentGraph->name == name) {
            currentGraph = nullptr;
        }
        graphs.erase(it, graphs.end());
        return true;
    }
    return false;
}

Graph* Program::getGraphByName(const std::string& name) {
    for (auto& g : graphs) {
        if (g.name == name)
            return &g;
    }
    return nullptr;
}

void Program::setCurrentGraph(const std::string& name) {
    currentGraph = getGraphByName(name);
}
