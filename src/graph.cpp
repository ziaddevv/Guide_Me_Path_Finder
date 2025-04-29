#include "graph.hpp"


void Graph::addCity(const string& name) {
    adj[name];
    numberOfCities++;
}
int Graph::getnumberOfCities()
{
    return numberOfCities;
}
vector<string> Graph::getAllCities()
{
    vector<string>res;
    for(auto &[city,_]:adj)
    {
        res.push_back(city);
    }
    return res;
}
void Graph::addEdge(const string& src, const string& dest, double distance, double time) {
    if (src == dest) return;
    adj[src][dest] = {distance, time};
    adj[dest][src] = {distance, time};
}

void Graph::deleteCity(const string& name) {
    for (auto& [city, neighbors] : adj) {
        neighbors.erase(name);
    }
    adj.erase(name);
}

void Graph::deleteEdge(const string& src, const string& dest) {
    adj[src].erase(dest);
    adj[dest].erase(src);
}

string Graph::BFS(const string& start) {
    if (adj.find(start) == adj.end()) return "";

    unordered_set<string> visited;
    queue<string> q;
    string result;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        string city = q.front();
        q.pop();
        result += city + " ";

        for (auto& [neighbor, _] : adj[city]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return result;
}

string Graph::DFS(const string& start) {
    if (adj.find(start) == adj.end()) return "";

    unordered_set<string> visited;
    stack<string> st;
    string result;

    st.push(start);

    while (!st.empty()) {
        string city = st.top();
        st.pop();

        if (visited.find(city) == visited.end()) {
            visited.insert(city);
            result += city + " ";

            for (auto& [neighbor, _] : adj[city]) {
                if (visited.find(neighbor) == visited.end()) {
                    st.push(neighbor);
                }
            }
        }
    }

    return result;
}
string Graph::DijkstraDistance(const string& start, const string& destination) {

    if (adj.find(start) == adj.end() || adj.find(destination) == adj.end()) {
        return "Path not found";
    }


    unordered_map<string, double> minDistance;
    unordered_map<string, string> previous;
    priority_queue<pair<double, string>,
                   vector<pair<double, string>>,
                   greater<>> pq;


    for (auto& cityPair : adj) {
        minDistance[cityPair.first] = numeric_limits<double>::infinity();
    }
    minDistance[start] = 0.0;
    pq.push({0.0, start});


    while (!pq.empty()) {
        auto [distSoFar, city] = pq.top();
        pq.pop();
        if (distSoFar > minDistance[city]) continue;
        if (city == destination) break;

        for (auto& [neighbor, data] : adj[city]) {
            double edgeDist = data.first;
            double newDist = distSoFar + edgeDist;
            if (newDist < minDistance[neighbor]) {
                minDistance[neighbor] = newDist;
                previous[neighbor] = city;
                pq.push({newDist, neighbor});
            }
        }
    }


    if (minDistance[destination] == numeric_limits<double>::infinity()) {
        return "Path not found";
    }


    vector<string> path;
    for (string cur = destination; ; cur = previous[cur]) {
        path.push_back(cur);
        if (cur == start) break;
    }
    reverse(path.begin(), path.end());


    double totalTime = 0.0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        totalTime += adj[path[i]][path[i+1]].second;
    }


    stringstream result;
    for (size_t i = 0; i < path.size(); ++i) {
        result << path[i] << (i + 1 < path.size() ? " " : "");
    }
    result << " | " << minDistance[destination]
           << " | " << totalTime;

    return result.str();
}


string Graph::DijkstraTime(const string& start, const string& destination) {

    if (adj.find(start) == adj.end() || adj.find(destination) == adj.end()) {
        return "Path not found";
    }


    unordered_map<string, double> minTime;
    unordered_map<string, string> previous;
    priority_queue<pair<double, string>,
                   vector<pair<double, string>>,
                   greater<>> pq;


    for (auto& cityPair : adj) {
        minTime[cityPair.first] = numeric_limits<double>::infinity();
    }
    minTime[start] = 0.0;
    pq.push({0.0, start});


    while (!pq.empty()) {
        auto [timeSoFar, city] = pq.top();
        pq.pop();
        if (timeSoFar > minTime[city]) continue;
        if (city == destination) break;

        for (auto& [neighbor, data] : adj[city]) {
            double edgeTime = data.second;
            double newTime = timeSoFar + edgeTime;
            if (newTime < minTime[neighbor]) {
                minTime[neighbor] = newTime;
                previous[neighbor] = city;
                pq.push({newTime, neighbor});
            }
        }
    }


    if (minTime[destination] == numeric_limits<double>::infinity()) {
        return "Path not found";
    }


    vector<string> path;
    for (string cur = destination;; cur = previous[cur]) {
        path.push_back(cur);
        if (cur == start) break;
    }
    reverse(path.begin(), path.end());

    double totalDistance = 0.0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        totalDistance += adj[path[i]][path[i+1]].first;
    }

    stringstream result;
    for (size_t i = 0; i < path.size(); ++i) {
        result << path[i] << (i + 1 < path.size() ? " " : "");
    }
    result << " | " << minTime[destination]
           << " | " << totalDistance;

    return result.str();
}
