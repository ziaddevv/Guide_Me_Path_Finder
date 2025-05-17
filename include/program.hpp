#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "filehandler.hpp"
#include <vector>
#include <string>
using namespace std;
class Program {
public:
    Program();

    void loadGraphs();
    void saveGraphs();
    bool addGraph(const std::string& name);
    bool deleteGraph(const std::string& name);
    Graph* getGraphByName(const std::string& name);
    void setCurrentGraph(const std::string& name);

    Filehandler f;
     vector<Graph> graphs;
    Graph* currentGraph = nullptr;
     bool isModified = false;
};

#endif // PROGRAM_HPP
