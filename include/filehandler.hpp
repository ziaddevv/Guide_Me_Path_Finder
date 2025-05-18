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
    int numberOfGraphs;///isssu !! we need to add number of cities in each graph
    int numOfCitiesInFile;//this will be deleted ,used just for testing
    vector<Graph>graphs;
    Filehandler();
    void ReadGraphFromFile(const string& filename);
    void SaveInFile(const string& filename);
    void setGraphs(const vector<Graph>& g);
};

#endif // FILEHANDLER_HPP
