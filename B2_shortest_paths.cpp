#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <queue>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

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
    
    map<pair<char, char>, pair<int, vector<char> > > allPairsViaCapitalAlg2(char capital) {
        map<pair<char, char>, pair<int, vector<char> > > result;
        vector<int> distFromCapital = dijkstra(capital);
        
        for (map<char, int>::iterator it1 = nodeIndex.begin(); it1 != nodeIndex.end(); ++it1) {
            for (map<char, int>::iterator it2 = nodeIndex.begin(); it2 != nodeIndex.end(); ++it2) {
                char u = it1->first;
                char v = it2->first;
                
                if (u != capital && v != capital && u != v) {
                    int totalDist = distFromCapital[it1->second] + distFromCapital[it2->second];
                    vector<char> path;
                    path.push_back(u);
                    path.push_back(capital);
                    path.push_back(v);
                    result[make_pair(u, v)] = make_pair(totalDist, path);
                }
            }
        }
        
        return result;
    }
    
    void printPath(const vector<char>& path) {
        for (int i = 0; i < (int)path.size(); i++) {
            cout << path[i];
            if (i < (int)path.size() - 1) cout << ", ";
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
    Graph g;
    
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
    
    ofstream outputFile("B2_output.txt");
    
    auto start1 = high_resolution_clock::now();
    
    vector<pair<int, vector<char> > > results;
    for (int i = 0; i < (int)queries.size(); i++) {
        pair<int, vector<char> > result = g.shortestPathViaCapital(queries[i].first, queries[i].second, capital);
        results.push_back(result);
    }
    
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    
    for (int i = 0; i < (int)results.size(); i++) {
        cout << "Shortest Path: ";
        g.printPath(results[i].second);
        cout << endl;
        cout << "Shortest Distance: " << results[i].first << endl;
        cout << endl;
        cout << "Running-time: " << duration1.count() << " microseconds" << endl;
        cout << endl;
        
        if (outputFile.is_open()) {
            outputFile << "Shortest Path: ";
            for (int j = 0; j < (int)results[i].second.size(); j++) {
                outputFile << results[i].second[j];
                if (j < (int)results[i].second.size() - 1) outputFile << ", ";
            }
            outputFile << endl;
            outputFile << "Shortest Distance: " << results[i].first << endl;
            outputFile << endl;
            outputFile << "Running-time: " << duration1.count() << " microseconds" << endl;
            outputFile << endl;
        }
    }
    
    auto start2 = high_resolution_clock::now();
    
    map<pair<char, char>, pair<int, vector<char> > > allPairs = g.allPairsViaCapitalAlg2(capital);
    
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    
    for (int i = 0; i < (int)queries.size(); i++) {
        map<pair<char, char>, pair<int, vector<char> > >::iterator it = allPairs.find(queries[i]);
        if (it != allPairs.end()) {
            cout << "Shortest Path: ";
            g.printPath(it->second.second);
            cout << endl;
            cout << "Shortest Distance: " << it->second.first << endl;
            cout << endl;
            cout << "Running-time: " << duration2.count() << " microseconds" << endl;
            cout << endl;
            
            if (outputFile.is_open()) {
                outputFile << "Shortest Path: ";
                for (int j = 0; j < (int)it->second.second.size(); j++) {
                    outputFile << it->second.second[j];
                    if (j < (int)it->second.second.size() - 1) outputFile << ", ";
                }
                outputFile << endl;
                outputFile << "Shortest Distance: " << it->second.first << endl;
                outputFile << endl;
                outputFile << "Running-time: " << duration2.count() << " microseconds" << endl;
                outputFile << endl;
            }
        }
    }
    
    if (outputFile.is_open()) {
        outputFile.close();
    }
    
    return 0;
}