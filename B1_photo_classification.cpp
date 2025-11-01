#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

struct Edge {
    int u, v, weight;
    
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
    
    bool operator<(const Edge& other) const {
        return weight > other.weight;
    }
};

class UnionFind {
private:
    vector<int> parent, rank;
    int components;
    
public:
    UnionFind(int n) : parent(n), rank(n, 0), components(n) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        
        if (rank[px] < rank[py]) {
            parent[px] = py;
        } else if (rank[px] > rank[py]) {
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
        components--;
        return true;
    }
    
    int getComponents() {
        return components;
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

int main() {
    vector<Edge> edges;
    
    ifstream inputFile("B1_input.txt");
    if (!inputFile.is_open()) {
        cout << "Error: Could not open B1_input.txt" << endl;
        return 1;
    }
    
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string photo1, photo2;
        int similarity;
        
        if (ss >> photo1 >> photo2 >> similarity) {
            int u = stoi(photo1.substr(1)) - 1;  // Convert p1 to 0, p2 to 1, etc.
            int v = stoi(photo2.substr(1)) - 1;
            edges.push_back(Edge(u, v, similarity));
        }
    }
    inputFile.close();
    
    int n = 20;
    int k = 3; 
    
    auto start = high_resolution_clock::now();
    
    sort(edges.begin(), edges.end());
    
    UnionFind uf(n);
    
    for (int i = 0; i < (int)edges.size(); i++) {
        if (uf.getComponents() <= k) {
            break;
        }
        
        if (!uf.connected(edges[i].u, edges[i].v)) {
            uf.unite(edges[i].u, edges[i].v);
        }
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    map<int, vector<int> > groups;
    for (int i = 0; i < n; i++) {
        int root = uf.find(i);
        groups[root].push_back(i + 1);
    }
    
    int groupNum = 1;
    for (map<int, vector<int> >::iterator it = groups.begin(); it != groups.end(); ++it) {
        cout << "Group " << groupNum << " = " << it->second.size() << "; photos: ";
        for (int i = 0; i < (int)it->second.size(); i++) {
            cout << "p" << it->second[i];
            if (i < (int)it->second.size() - 1) cout << ", ";
        }
        cout << endl;
        groupNum++;
    }
    
    cout << "Running-time: " << duration.count() << " microseconds" << endl;
    
    ofstream outputFile("B1_output.txt");
    if (outputFile.is_open()) {
        groupNum = 1;
        for (map<int, vector<int> >::iterator it = groups.begin(); it != groups.end(); ++it) {
            outputFile << "Group " << groupNum << " = " << it->second.size() << "; photos: ";
            for (int i = 0; i < (int)it->second.size(); i++) {
                outputFile << "p" << it->second[i];
                if (i < (int)it->second.size() - 1) outputFile << ", ";
            }
            outputFile << endl;
            groupNum++;
        }
        outputFile << "Running-time: " << duration.count() << " microseconds" << endl;
        outputFile.close();
    }
    
    return 0;
}