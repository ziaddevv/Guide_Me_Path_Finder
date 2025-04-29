#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP
#include <iostream>
#include <fstream>
#include "graph.hpp"
#include<vector>
using namespace std;
class Filehandler
{

public:
    int numberOfGraphs;
    vector<Graph>graphs;
    Filehandler();
    string ReadFile();
    void ReadGraphFromFile(const string& filename);
};

#endif // FILEHANDLER_HPP
