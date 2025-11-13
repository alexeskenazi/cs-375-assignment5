#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

struct Edge {
    char from, to;
    int weight;

    Edge(char from, char to, int weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
};

class BellmanFordGraph {
private:
    vector<Edge> edges;
    map<char, int> nodeIndex;
    vector<char> indexToNode;
    int numNodes;
    
public:
    BellmanFordGraph() {
        numNodes = 0;
    }
    
    void addEdge(char u, char v, int weight) {
        if (nodeIndex.find(u) == nodeIndex.end()) {
            nodeIndex[u] = numNodes++;
            indexToNode.push_back(u);
        }
        if (nodeIndex.find(v) == nodeIndex.end()) {
            nodeIndex[v] = numNodes++;
            indexToNode.push_back(v);
        }
        
        edges.push_back(Edge(u, v, weight));
        edges.push_back(Edge(v, u, weight));
    }
    
    int getNumNodes() const { return numNodes; }
    int getNumEdges() const { return edges.size(); }
    
    pair<vector<int>, vector<int> > bellmanFordWithParents(char start) {
        vector<int> dist(numNodes, INT_MAX);
        vector<int> parent(numNodes, -1);
        int startIdx = nodeIndex.find(start)->second;
        dist[startIdx] = 0;

        for (int i = 0; i < numNodes - 1; i++) {
            for (int j = 0; j < (int)edges.size(); j++) {
                Edge edge = edges[j];
                int u = nodeIndex.find(edge.from)->second;
                int v = nodeIndex.find(edge.to)->second;

                if (dist[u] != INT_MAX && dist[u] + edge.weight < dist[v]) {
                    dist[v] = dist[u] + edge.weight;
                    parent[v] = u;
                }
            }
        }

        for (int j = 0; j < (int)edges.size(); j++) {
            Edge edge = edges[j];
            int u = nodeIndex.find(edge.from)->second;
            int v = nodeIndex.find(edge.to)->second;

            if (dist[u] != INT_MAX && dist[u] + edge.weight < dist[v]) {
                cout << "Negative cycle detected!" << endl;
                return make_pair(vector<int>(), vector<int>());
            }
        }
        return make_pair(dist, parent);
    }

    // Extract path from parent array
    vector<char> extractPath(int from, int to, const vector<int>& parent) {
        vector<char> path;
        int curr = to;

        while (curr != from) {
            path.push_back(indexToNode[curr]);
            curr = parent[curr];
        }
        path.push_back(indexToNode[from]);

        // Reverse to get path from 'from' to 'to'
        for (int i = 0; i < (int)path.size() / 2; i++) {
            char temp = path[i];
            path[i] = path[path.size() - 1 - i];
            path[path.size() - 1 - i] = temp;
        }

        return path;
    }
    
    void printGraph() {
        cout << "Graph with " << numNodes << " nodes and " << edges.size() << " directed edges:" << endl;
        for (int i = 0; i < (int)edges.size(); i++) {
            cout << "Edge " << i << ": " << edges[i].from << " -> " << edges[i].to 
                 << " (weight: " << edges[i].weight << ")" << endl;
        }
    }
    
    map<char, int> getNodeIndex() const { return nodeIndex; }
    
    pair<int, vector<char> > shortestPathViaCapital(char start, char end, char capital) {
        pair<vector<int>, vector<int> > result = bellmanFordWithParents(capital);
        vector<int> distFromCapital = result.first;
        vector<int> parent = result.second;

        if (distFromCapital.empty()) {
            cout << "Cannot compute paths due to negative cycle!" << endl;
            return make_pair(-1, vector<char>());
        }

        int capitalIdx = nodeIndex.find(capital)->second;
        int startIdx = nodeIndex.find(start)->second;
        int endIdx = nodeIndex.find(end)->second;

        if (distFromCapital[startIdx] == INT_MAX || distFromCapital[endIdx] == INT_MAX) {
            vector<char> emptyPath;
            return make_pair(-1, emptyPath);
        }

        int totalDist = distFromCapital[startIdx] + distFromCapital[endIdx];

        // Build actual path: start -> ... -> capital -> ... -> end
        vector<char> pathToStart = extractPath(capitalIdx, startIdx, parent);
        vector<char> pathToEnd = extractPath(capitalIdx, endIdx, parent);

        vector<char> fullPath;
        // Reverse path from capital to start (to get start to capital)
        for (int i = (int)pathToStart.size() - 1; i >= 0; i--) {
            fullPath.push_back(pathToStart[i]);
        }
        // Add path from capital to end (skip capital since already added)
        for (int i = 1; i < (int)pathToEnd.size(); i++) {
            fullPath.push_back(pathToEnd[i]);
        }

        return make_pair(totalDist, fullPath);
    }
    
    void printPath(const vector<char>& path) {
        for (int i = 0; i < (int)path.size(); i++) {
            cout << path[i];
            if (i < (int)path.size() - 1) cout << ", ";
        }
    }
};

int main() {
    BellmanFordGraph g;
    
    ifstream inputFile("B2_input.txt");
    if (!inputFile.is_open()) {
        cout << "Error: Could not open B2_input.txt" << endl;
        return 1;
    }
    
    string line;
    vector<pair<char, char> > queries;
    
    while (getline(inputFile, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token1, token2, token3;
        
        if (ss >> token1 >> token2 >> token3) {
            char u = token1[0];
            char v = token2[0];
            int weight = stoi(token3);
            g.addEdge(u, v, weight);
        } else if (ss.str().find(' ') != string::npos) {
            stringstream ss2(line);
            char start, end;
            if (ss2 >> start >> end) {
                queries.push_back(make_pair(start, end));
            }
        }
    }
    inputFile.close();
    
    char capital = 'a';

    cout << "=== BELLMAN-FORD ALGORITHM ===" << endl;
    cout << "Graph has " << g.getNumNodes() << " nodes and " << g.getNumEdges() << " directed edges" << endl;
    cout << "Capital city: " << capital << endl;
    cout << "Number of queries: " << queries.size() << endl;
    for (int i = 0; i < (int)queries.size(); i++) {
        cout << "  Query " << (i+1) << ": " << queries[i].first << " to " << queries[i].second << endl;
    }
    cout << endl;

    ofstream outputFile("B3_output.txt");

    cout << "Running Bellman-Ford from capital '" << capital << "'..." << endl;
    cout << "Will relax edges " << (g.getNumNodes() - 1) << " times" << endl << endl;

    auto start = high_resolution_clock::now();

    vector<pair<int, vector<char> > > results;
    for (int i = 0; i < (int)queries.size(); i++) {
        cout << "Computing path: " << queries[i].first << " -> " << capital << " -> " << queries[i].second << endl;
        pair<int, vector<char> > result = g.shortestPathViaCapital(queries[i].first, queries[i].second, capital);
        results.push_back(result);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << endl;

    for (int i = 0; i < (int)results.size(); i++) {
        cout << "//** Print out the shortest distance D and the shortest path from Source node "
             << queries[i].first << " to Destination node " << queries[i].second
             << " via node a (for example, " << queries[i].first << " --> " << queries[i].second
             << "); **//" << endl;
        cout << endl;

        if (results[i].first == -1) {
            cout << "No path exists (disconnected components)" << endl;
            cout << endl;
        } else {
            cout << "Shortest Path: ";
            g.printPath(results[i].second);
            cout << endl;
            cout << "Shortest Distance: " << results[i].first << endl;
            cout << endl;
        }

        if (outputFile.is_open()) {
            outputFile << "//** Print out the shortest distance D and the shortest path from Source node "
                       << queries[i].first << " to Destination node " << queries[i].second
                       << " via node a (for example, " << queries[i].first << " --> " << queries[i].second
                       << "); **//" << endl;
            outputFile << endl;

            if (results[i].first == -1) {
                outputFile << "No path exists (disconnected components)" << endl;
                outputFile << endl;
            } else {
                outputFile << "Shortest Path: ";
                for (int j = 0; j < (int)results[i].second.size(); j++) {
                    outputFile << results[i].second[j];
                    if (j < (int)results[i].second.size() - 1) outputFile << ", ";
                }
                outputFile << endl;
                outputFile << "Shortest Distance: " << results[i].first << endl;
                outputFile << endl;
            }
        }
    }

    cout << "//** print out running time **//" << endl;
    cout << "Running-time: " << duration.count() << " microseconds" << endl;
    cout << endl;

    if (outputFile.is_open()) {
        outputFile << "//** print out running time **//" << endl;
        outputFile << "Running-time: " << duration.count() << " microseconds" << endl;
        outputFile << endl;
    }
    
    if (outputFile.is_open()) {
        outputFile.close();
    }
    
    return 0;
}