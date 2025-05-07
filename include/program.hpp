#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#include"filehandler.hpp"
#include"vector"

using namespace std;
class Program
{
public:
    Program();
    Filehandler f;
    std::vector<Graph> graphs;
        // Stores all loaded graphs
    Graph* currentGraph = nullptr;
    void addGraph(string name);

};

#endif // PROGRAM_HPP
