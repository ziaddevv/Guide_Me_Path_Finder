#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP
#include <iostream>
#include <fstream>
#include "graph.hpp"
#include<vector>
#include<city.hpp>
using namespace std;
class Filehandler
{

public:
    int numberOfGraphs;///isssu !! we need to add number of cities in each graph
    int numOfCitiesInFile;//this will be deleted ,used just for testing
    vector<Graph>graphs;
    unordered_map<string,City>citiesGraphs;
    Filehandler();
    void ReadGraphFromFile(const string& filename);
    void SaveInFile(const string& filename);
    void setGraphs(const std::vector<Graph>& g);
};

#endif // FILEHANDLER_HPP
