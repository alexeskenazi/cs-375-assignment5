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

// Edge structure for adjacency list
struct Edge {
    char to;
    int weight;

    Edge(char to, int weight) {
        this->to = to;
        this->weight = weight;
    }
};

// Type aliases to make complex types easier to read
typedef pair<char, char> CityPair;           // (start city, end city)
typedef pair<int, vector<char> > PathResult;  // (distance, path)
typedef map<CityPair, PathResult> PathMap;    // stores all paths

// Graph class using adjacency list
class Graph {
private:
    map<char, vector<Edge> > adj;
    map<char, int> nodeIndex;  // char to index
    vector<char> indexToNode; // index to char
    int numNodes;

public:
    Graph() {
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
        
        adj[u].push_back(Edge(v, weight));
        adj[v].push_back(Edge(u, weight));
    }
    
    int getNumNodes() const { return numNodes; }
    
    map<char, int> getNodeIndex() const { return nodeIndex; }
    
    // Dijkstra's algorithm implementation - returns distances and parent pointers
    pair<vector<int>, vector<int> > dijkstraWithParents(char start) {
        vector<int> dist(numNodes, INT_MAX);
        vector<int> parent(numNodes, -1);
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
                    parent[v] = u;
                    pq.push(make_pair(newDist, v));
                }
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

    // Check if two paths share any vertices (except the capital)
    bool pathsShareVertices(const vector<char>& path1, const vector<char>& path2, char capital) {
        map<char, bool> visited;

        // Mark all vertices in path1 (except capital)
        for (int i = 0; i < (int)path1.size(); i++) {
            if (path1[i] != capital) {
                visited[path1[i]] = true;
            }
        }

        // Check if any vertex in path2 (except capital) is in path1
        for (int i = 0; i < (int)path2.size(); i++) {
            if (path2[i] != capital && visited[path2[i]]) {
                return true;
            }
        }

        return false;
    }
    
    PathResult shortestPathViaCapital(char start, char end, char capital) {
        pair<vector<int>, vector<int> > result = dijkstraWithParents(capital);
        vector<int> distFromCapital = result.first;
        vector<int> parent = result.second;

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
    
    PathMap allPairsViaCapitalAlg2(char capital) {
        PathMap result;
        pair<vector<int>, vector<int> > dijkstraResult = dijkstraWithParents(capital);
        vector<int> distFromCapital = dijkstraResult.first;
        vector<int> parent = dijkstraResult.second;

        int capitalIdx = nodeIndex.find(capital)->second;

        for (map<char, int>::iterator it1 = nodeIndex.begin(); it1 != nodeIndex.end(); ++it1) {
            for (map<char, int>::iterator it2 = nodeIndex.begin(); it2 != nodeIndex.end(); ++it2) {
                char u = it1->first;
                char v = it2->first;

                if (u != capital && v != capital && u != v) {
                    int uIdx = it1->second;
                    int vIdx = it2->second;

                    if (distFromCapital[uIdx] == INT_MAX || distFromCapital[vIdx] == INT_MAX) {
                        vector<char> emptyPath;
                        result[make_pair(u, v)] = make_pair(-1, emptyPath);
                    } else {
                        // Extract actual paths from capital
                        vector<char> pathToU = extractPath(capitalIdx, uIdx, parent);
                        vector<char> pathToV = extractPath(capitalIdx, vIdx, parent);

                        // Check if paths share any vertices (except capital)
                        if (pathsShareVertices(pathToU, pathToV, capital)) {
                            // Paths overlap - violates no-revisit constraint
                            vector<char> emptyPath;
                            result[make_pair(u, v)] = make_pair(-1, emptyPath);
                        } else {
                            // Paths are disjoint - valid path exists
                            int totalDist = distFromCapital[uIdx] + distFromCapital[vIdx];

                            // Build full path: u -> ... -> capital -> ... -> v
                            vector<char> fullPath;
                            for (int i = (int)pathToU.size() - 1; i >= 0; i--) {
                                fullPath.push_back(pathToU[i]);
                            }
                            for (int i = 1; i < (int)pathToV.size(); i++) {
                                fullPath.push_back(pathToV[i]);
                            }

                            result[make_pair(u, v)] = make_pair(totalDist, fullPath);
                        }
                    }
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
    vector<CityPair> queries;
    
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

    cout << "=== ALGORITHM 1: O(n log n) - Visits Allowed ===" << endl;
    cout << "Graph has " << g.getNumNodes() << " nodes" << endl;
    cout << "Capital city: " << capital << endl;
    cout << "Number of queries: " << queries.size() << endl;
    for (int i = 0; i < (int)queries.size(); i++) {
        cout << "  Query " << (i+1) << ": " << queries[i].first << " to " << queries[i].second << endl;
    }
    cout << endl;

    ofstream outputFile("B2_output.txt");

    cout << "Running Dijkstra from capital '" << capital << "'..." << endl;
    auto start1 = high_resolution_clock::now();

    vector<PathResult> results;
    for (int i = 0; i < (int)queries.size(); i++) {
        cout << "Computing path: " << queries[i].first << " -> " << capital << " -> " << queries[i].second << endl;
        PathResult result = g.shortestPathViaCapital(queries[i].first, queries[i].second, capital);
        results.push_back(result);
    }

    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
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
    cout << "Running-time: " << duration1.count() << " microseconds" << endl;
    cout << endl;

    if (outputFile.is_open()) {
        outputFile << "//** print out running time **//" << endl;
        outputFile << "Running-time: " << duration1.count() << " microseconds" << endl;
        outputFile << endl;
    }
    
    auto start2 = high_resolution_clock::now();
    
    cout << endl << "=== ALGORITHM 2: O(n^2) - No Revisits ===" << endl;
    cout << "Precomputing all pairs of cities via capital..." << endl;

    PathMap allPairs = g.allPairsViaCapitalAlg2(capital);

    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    cout << "Computed paths for all " << allPairs.size() << " city pairs" << endl << endl;

    for (int i = 0; i < (int)queries.size(); i++) {
        PathMap::iterator it = allPairs.find(queries[i]);
        if (it != allPairs.end()) {
            cout << "//** Print out the shortest distance D and the shortest path from Source node "
                 << queries[i].first << " to Destination node " << queries[i].second
                 << " via node a (for example, " << queries[i].first << " --> " << queries[i].second
                 << "); **//" << endl;
            cout << endl;

            if (it->second.first == -1) {
                cout << "No valid path (paths would overlap - violates no-revisit constraint)" << endl;
                cout << endl;
            } else {
                cout << "Shortest Path: ";
                g.printPath(it->second.second);
                cout << endl;
                cout << "Shortest Distance: " << it->second.first << endl;
                cout << endl;
            }

            if (outputFile.is_open()) {
                outputFile << "//** Print out the shortest distance D and the shortest path from Source node "
                           << queries[i].first << " to Destination node " << queries[i].second
                           << " via node a (for example, " << queries[i].first << " --> " << queries[i].second
                           << "); **//" << endl;
                outputFile << endl;

                if (it->second.first == -1) {
                    outputFile << "No valid path (paths would overlap - violates no-revisit constraint)" << endl;
                    outputFile << endl;
                } else {
                    outputFile << "Shortest Path: ";
                    for (int j = 0; j < (int)it->second.second.size(); j++) {
                        outputFile << it->second.second[j];
                        if (j < (int)it->second.second.size() - 1) outputFile << ", ";
                    }
                    outputFile << endl;
                    outputFile << "Shortest Distance: " << it->second.first << endl;
                    outputFile << endl;
                }
            }
        }
    }

    cout << "//** print out running time **//" << endl;
    cout << "Running-time: " << duration2.count() << " microseconds" << endl;
    cout << endl;

    if (outputFile.is_open()) {
        outputFile << "//** print out running time **//" << endl;
        outputFile << "Running-time: " << duration2.count() << " microseconds" << endl;
        outputFile << endl;
    }
    
    if (outputFile.is_open()) {
        outputFile.close();
    }
    
    return 0;
}