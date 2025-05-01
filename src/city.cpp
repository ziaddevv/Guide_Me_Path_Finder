#include "city.hpp"

void City::addNode_walk(string src, string dest, double distance, double time) {
    Transport t;
    t.Distance = distance;
    t.time = time;
    CityGraph_walk[src][dest] = t;
    CityGraph_walk[dest][src] = t;
}

void City::addNode_car(string src, string dest, double distance, double time) {
    Transport t;
    t.Distance = distance;
    t.time = time;
    CityGraph_car[src][dest] = t;
    CityGraph_car[dest][src] = t;
}

unordered_map<string, Transport> City::dijkstra_walk(string source) {
    unordered_map<string, Transport> shortest;

    for (const auto& [city, _] : CityGraph_walk) {
        shortest[city] = {numeric_limits<double>::infinity(), numeric_limits<double>::infinity()};
    }


    shortest[source] = {0, 0};


    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();


        if (dist > shortest[current].Distance) continue;

        for (const auto& [neighbor, t] : CityGraph_walk[current]) {
            double newDist = shortest[current].Distance + t.Distance;
            double newTime = shortest[current].time + t.time;

            if (newDist < shortest[neighbor].Distance) {
                shortest[neighbor] = {newDist, newTime};
                pq.push({newDist, neighbor});
            }
        }
    }

    return shortest;
}

unordered_map<string, Transport> City::dijkstra_car(string source) {
    unordered_map<string, Transport> shortest;


    for (const auto& [city, _] : CityGraph_car) {
        shortest[city] = {numeric_limits<double>::infinity(), numeric_limits<double>::infinity()};
    }

    shortest[source] = {0, 0};


    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();

        if (dist > shortest[current].Distance) continue;

        for (const auto& [neighbor, t] : CityGraph_car[current]) {
            double newDist = shortest[current].Distance + t.Distance;
            double newTime = shortest[current].time + t.time;

            if (newDist < shortest[neighbor].Distance) {
                shortest[neighbor] = {newDist, newTime};
                pq.push({newDist, neighbor});
            }
        }
    }

    return shortest;
}
