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
    string ReadFile();
    void ReadGraphFromFile(const string& filename);
     void ReadCityGraphsFromFile(const string& filename);
};

#endif // FILEHANDLER_HPP
