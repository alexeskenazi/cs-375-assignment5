#include <iostream>
#include <vector>
#include <climits>
#include <map>

using namespace std;

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
    
    char start = 'a';
    cout << "Running Bellman-Ford from node '" << start << "':" << endl;
    
    vector<int> distances = g.bellmanFord(start);
    
    cout << "Shortest distances from " << start << ":" << endl;
    map<char, int> nodeMap = g.getNodeIndex();
    for (map<char, int>::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
        char node = it->first;
        int idx = it->second;
        if (distances[idx] == INT_MAX) {
            cout << start << " to " << node << ": unreachable" << endl;
        } else {
            cout << start << " to " << node << ": " << distances[idx] << endl;
        }
    }
    
    return 0;
}