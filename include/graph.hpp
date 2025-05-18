#pragma once
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <limits>
#include<algorithm>

using namespace std;

class Graph {
private:

public:
    struct PathResult {
        vector<string> path;
        double distanceOrTime = 0.0;
    };
    int numberOfCities = 0;
    string name;
    unordered_map<string, unordered_map<string, pair<double, double>>> adj;
    vector<string>getAllCities();
    int getnumberOfCities();
    void addCity(const string& name);
    void addEdge(const string& src, const string& dest, double distance, double time);
    void deleteCity(const string& name);
    void deleteEdge(const string& src, const string& dest);
    bool containsCity(const string& name);
    bool containsEdge(const string& city1, const string& city2);
    vector<string> BFS(const string& start);
    vector<string> DFS(const string& start);
    PathResult DijkstraDistance(const string& start, const string& destination);
    PathResult DijkstraTime(const string& start, const string& destination);
};
