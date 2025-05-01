#ifndef CITY_HPP
#define CITY_HPP
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <limits>
#include<algorithm>
using namespace std;
// enum transportation {
//     walk,
//     car
// };
struct Transport
{
    double Distance;
    double time;
};

class City
{
public:
    City() = default;
    string name;
    unordered_map<string, unordered_map<string, Transport>> CityGraph_walk;
    unordered_map<string, unordered_map<string, Transport>> CityGraph_car;

    void addNode_walk(string src, string dest, double distance, double time);
    void addNode_car(string src, string dest, double distance, double time);

    unordered_map<string, Transport> dijkstra_walk(string source);
    unordered_map<string, Transport> dijkstra_car(string source);
};

#endif // CITY_HPP
