#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

// Edge structure for storing graph edges
struct Edge {
    int u, v, weight;

    Edge(int u, int v, int weight) {
        this->u = u;
        this->v = v;
        this->weight = weight;
    }

    // Sort by weight in descending order (higher similarity first)
    bool operator<(const Edge& other) const {
        return weight > other.weight;
    }
};

// Union-Find data structure for disjoint sets
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    int components;

public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        components = n;
        // Initialize each element as its own parent
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    // Find with path compression
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // path compression
        }
        return parent[x];
    }

    // Union by rank
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false;  // already in same set
        }

        // Union by rank
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
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

    // Read input from file
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
            // Convert photo names like p1, p2 to indices 0, 1, etc
            int u = stoi(photo1.substr(1)) - 1;
            int v = stoi(photo2.substr(1)) - 1;
            edges.push_back(Edge(u, v, similarity));
        }
    }
    inputFile.close();
    
    int n = 20;  // number of photos
    int k = 3;   // number of groups we want

    auto start = high_resolution_clock::now();

    // Sort edges by weight (descending order for max similarity)
    sort(edges.begin(), edges.end());

    UnionFind uf(n);

    // Process edges until we have k groups
    for (int i = 0; i < (int)edges.size(); i++) {
        if (uf.getComponents() <= k) {
            break;  // we have our 3 groups
        }

        if (!uf.connected(edges[i].u, edges[i].v)) {
            uf.unite(edges[i].u, edges[i].v);
        }
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    // Group photos by their root parent
    map<int, vector<int> > groups;
    for (int i = 0; i < n; i++) {
        int root = uf.find(i);
        groups[root].push_back(i + 1);  // +1 to convert back to photo numbers
    }

    // Print results
    int groupNum = 1;
    for (map<int, vector<int> >::iterator it = groups.begin(); it != groups.end(); ++it) {
        cout << "Group " << groupNum << " = " << it->second.size() << "; photos: ";
        for (int i = 0; i < (int)it->second.size(); i++) {
            cout << "p" << it->second[i];
            if (i < (int)it->second.size() - 1) {
                cout << ", ";
            }
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