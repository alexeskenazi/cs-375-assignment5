#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <chrono>

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
            if (i < (int)path.size() - 1) cout << " -> ";
        }
    }
};

int main() {
    cout << "Bellman-Ford Algorithm - CS375 Assignment 5" << endl;
    cout << "Problem B.3: Shortest Paths via Capital using Bellman-Ford" << endl;
    
    BellmanFordGraph g;
    
    g.addEdge('a', 'b', 5);
    g.addEdge('a', 'g', 21);
    g.addEdge('b', 'c', 8);
    g.addEdge('b', 'd', 12);
    g.addEdge('c', 'e', 7);
    g.addEdge('d', 'e', 9);
    g.addEdge('d', 'f', 11);
    g.addEdge('e', 'f', 6);
    g.addEdge('f', 'g', 15);
    g.addEdge('g', 'h', 18);
    g.addEdge('h', 'i', 14);
    g.addEdge('i', 'j', 10);
    
    char capital = 'a';
    
    auto start = high_resolution_clock::now();
    
    pair<int, vector<char> > result_d_i = g.shortestPathViaCapital('d', 'i', capital);
    pair<int, vector<char> > result_f_g = g.shortestPathViaCapital('f', 'g', capital);
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    if (result_d_i.first != -1) {
        cout << "Shortest Path: ";
        g.printPath(result_d_i.second);
        cout << endl;
        cout << "Shortest Distance: " << result_d_i.first << endl;
        cout << endl;
        cout << "Running-time: " << duration.count() << " microseconds" << endl;
        cout << endl;
    }
    
    if (result_f_g.first != -1) {
        cout << "Shortest Path: ";
        g.printPath(result_f_g.second);
        cout << endl;
        cout << "Shortest Distance: " << result_f_g.first << endl;
        cout << endl;
        cout << "Running-time: " << duration.count() << " microseconds" << endl;
    }
    
    return 0;
}