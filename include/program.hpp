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
    bool addGraph(const string& name);
    bool deleteGraph(const string& name);
    Graph* getGraphByName(const string& name);
    void setCurrentGraph(const string& name);

    Filehandler f;
     vector<Graph> graphs;
    Graph* currentGraph = nullptr;
     bool isModified = false;
};

#endif // PROGRAM_HPP
