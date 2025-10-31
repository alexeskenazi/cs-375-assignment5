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
    
    pair<int, vector<char> > shortestPathViaCapital(char start, char end, char capital) {
        vector<int> distFromCapital = dijkstra(capital);
        
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
    g.addEdge('g', 'h', 18);
    g.addEdge('h', 'i', 14);
    g.addEdge('i', 'j', 10);
    
    char capital = 'a';
    cout << "Capital city: " << capital << endl;
    cout << "Total nodes: " << g.getNumNodes() << endl;
    
    cout << "\nAlgorithm 1 (O(n log n) - visits allowed):" << endl;
    
    pair<int, vector<char> > result_d_i = g.shortestPathViaCapital('d', 'i', capital);
    cout << "Shortest Path d -> i via a: ";
    g.printPath(result_d_i.second);
    cout << endl;
    cout << "Shortest Distance: " << result_d_i.first << endl;
    cout << endl;
    
    pair<int, vector<char> > result_f_g = g.shortestPathViaCapital('f', 'g', capital);
    cout << "Shortest Path f -> g via a: ";
    g.printPath(result_f_g.second);
    cout << endl;
    cout << "Shortest Distance: " << result_f_g.first << endl;
    
    return 0;
}