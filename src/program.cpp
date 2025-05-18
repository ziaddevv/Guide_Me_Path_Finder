#include "program.hpp"

Program::Program() {
    loadGraphs(); // Load graphs during initialization
}

void Program::loadGraphs() {
    f.ReadGraphFromFile("C:\\Users\\DELL\\Documents\\GitHub\\wasalney_mini_Path_Finder\\filename.txt");
    graphs = f.graphs;
}
void Program::saveGraphs()
{
    f.setGraphs(graphs);
    f.SaveInFile("C:\\Users\\DELL\\Documents\\GitHub\\wasalney_mini_Path_Finder\\filename.txt");

}

bool Program::addGraph(const std::string& name) {
    for (const auto& graph : graphs) {
        if (graph.name == name) {
            return false;
        }
    }

    graphs.push_back(Graph());
    graphs.back().name = name;

    isModified = true;
    return true;
}

bool Program::deleteGraph(const std::string& name) {

    auto it = std::find_if(graphs.begin(), graphs.end(), [&](const Graph& g) {
        return g.name == name;
    });

    if (it != graphs.end()) {

        if (currentGraph && currentGraph->name == name) {
            currentGraph = nullptr;
        }


        graphs.erase(it);
        isModified = true;
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
