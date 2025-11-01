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
    
    Edge(char from, char to, int weight) : from(from), to(to), weight(weight) {}
};

class BellmanFordGraph {
private:
    vector<Edge> edges;
    map<char, int> nodeIndex;
    vector<char> indexToNode;
    int numNodes;
    
public:
    BellmanFordGraph() : numNodes(0) {}
    
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
    
    vector<int> bellmanFord(char start) {
        vector<int> dist(numNodes, INT_MAX);
        int startIdx = nodeIndex.find(start)->second;
        dist[startIdx] = 0;
        
        for (int i = 0; i < numNodes - 1; i++) {
            for (int j = 0; j < (int)edges.size(); j++) {
                Edge edge = edges[j];
                int u = nodeIndex.find(edge.from)->second;
                int v = nodeIndex.find(edge.to)->second;
                
                if (dist[u] != INT_MAX && dist[u] + edge.weight < dist[v]) {
                    dist[v] = dist[u] + edge.weight;
                }
            }
        }
        
        for (int j = 0; j < (int)edges.size(); j++) {
            Edge edge = edges[j];
            int u = nodeIndex.find(edge.from)->second;
            int v = nodeIndex.find(edge.to)->second;
            
            if (dist[u] != INT_MAX && dist[u] + edge.weight < dist[v]) {
                cout << "Negative cycle detected!" << endl;
                return vector<int>();
            }
        }
        return dist;
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
        vector<int> distFromCapital = bellmanFord(capital);
        
        if (distFromCapital.empty()) {
            cout << "Cannot compute paths due to negative cycle!" << endl;
            return make_pair(-1, vector<char>());
        }
        
        int startIdx = nodeIndex.find(start)->second;
        int endIdx = nodeIndex.find(end)->second;
        
        if (distFromCapital[startIdx] == INT_MAX || distFromCapital[endIdx] == INT_MAX) {
            vector<char> emptyPath;
            return make_pair(-1, emptyPath);
        }
        
        int totalDist = distFromCapital[startIdx] + distFromCapital[endIdx];
        
        vector<char> path;
        path.push_back(start);
        path.push_back(capital);
        path.push_back(end);
        
        return make_pair(totalDist, path);
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
    
    ofstream outputFile("B3_output.txt");
    
    auto start = high_resolution_clock::now();
    
    vector<pair<int, vector<char> > > results;
    for (int i = 0; i < (int)queries.size(); i++) {
        pair<int, vector<char> > result = g.shortestPathViaCapital(queries[i].first, queries[i].second, capital);
        results.push_back(result);
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    for (int i = 0; i < (int)results.size(); i++) {
        if (results[i].first == -1) {
            cout << "No path exists (disconnected components)" << endl;
            cout << endl;
            cout << "Running-time: " << duration.count() << " microseconds" << endl;
            cout << endl;
        } else {
            cout << "Shortest Path: ";
            g.printPath(results[i].second);
            cout << endl;
            cout << "Shortest Distance: " << results[i].first << endl;
            cout << endl;
            cout << "Running-time: " << duration.count() << " microseconds" << endl;
            cout << endl;
        }
        
        if (outputFile.is_open()) {
            if (results[i].first == -1) {
                outputFile << "No path exists (disconnected components)" << endl;
            } else {
                outputFile << "Shortest Path: ";
                for (int j = 0; j < (int)results[i].second.size(); j++) {
                    outputFile << results[i].second[j];
                    if (j < (int)results[i].second.size() - 1) outputFile << ", ";
                }
                outputFile << endl;
                outputFile << "Shortest Distance: " << results[i].first << endl;
            }
            outputFile << endl;
            outputFile << "Running-time: " << duration.count() << " microseconds" << endl;
            outputFile << endl;
        }
    }
    
    if (outputFile.is_open()) {
        outputFile.close();
    }
    
    return 0;
}