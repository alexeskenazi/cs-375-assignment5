#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <queue>

using namespace std;

struct Edge {
    char to;
    int weight;
    
    Edge(char to, int weight) : to(to), weight(weight) {}
};

class Graph {
private:
    map<char, vector<Edge> > adj;
    map<char, int> nodeIndex;
    vector<char> indexToNode;
    int numNodes;
    
public:
    Graph() : numNodes(0) {}
    
    void addEdge(char u, char v, int weight) {
        if (nodeIndex.find(u) == nodeIndex.end()) {
            nodeIndex[u] = numNodes++;
            indexToNode.push_back(u);
        }
        if (nodeIndex.find(v) == nodeIndex.end()) {
            nodeIndex[v] = numNodes++;
            indexToNode.push_back(v);
        }
        
        adj[u].push_back(Edge(v, weight));
        adj[v].push_back(Edge(u, weight));
    }
    
    int getNumNodes() const { return numNodes; }
    
    map<char, int> getNodeIndex() const { return nodeIndex; }
    
    vector<int> dijkstra(char start) {
        vector<int> dist(numNodes, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
        
        int startIdx = nodeIndex[start];
        dist[startIdx] = 0;
        pq.push(make_pair(0, startIdx));
        
        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            
            if (d > dist[u]) continue;
            
            char uNode = indexToNode[u];
            for (int i = 0; i < (int)adj[uNode].size(); i++) {
                Edge edge = adj[uNode][i];
                int v = nodeIndex[edge.to];
                int newDist = dist[u] + edge.weight;
                
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    pq.push(make_pair(newDist, v));
                }
            }
        }
        
        return dist;
    }
    
    void printGraph() {
        cout << "Graph with " << numNodes << " nodes:" << endl;
        for (map<char, vector<Edge> >::iterator it = adj.begin(); it != adj.end(); ++it) {
            cout << "Node " << it->first << " -> ";
            for (int i = 0; i < (int)it->second.size(); i++) {
                cout << it->second[i].to << "(" << it->second[i].weight << ")";
                if (i < (int)it->second.size() - 1) cout << ", ";
            }
            cout << endl;
        }
    }
};

int main() {
    cout << "Shortest Paths via Capital - CS375 Assignment 5" << endl;
    cout << "Problem B.2: All-Pairs Shortest Paths via a Given Capital" << endl;
    
    Graph g;
    
    g.addEdge('a', 'b', 5);
    g.addEdge('a', 'g', 21);
    g.addEdge('b', 'c', 8);
    g.addEdge('b', 'd', 12);
    g.addEdge('c', 'e', 7);
    g.addEdge('d', 'e', 9);
    g.addEdge('d', 'f', 11);
    g.addEdge('e', 'f', 6);
    g.addEdge('f', 'g', 15);
    
    char capital = 'a';
    cout << "Capital city: " << capital << endl;
    
    g.printGraph();
    
    cout << "\nTesting Dijkstra's algorithm from capital '" << capital << "':" << endl;
    vector<int> distances = g.dijkstra(capital);
    
    cout << "Distances from " << capital << ":" << endl;
    for (map<char, int>::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
        char node = it->first;
        int idx = it->second;
        if (distances[idx] == INT_MAX) {
            cout << capital << " to " << node << ": unreachable" << endl;
        } else {
            cout << capital << " to " << node << ": " << distances[idx] << endl;
        }
    }
    
    return 0;
}