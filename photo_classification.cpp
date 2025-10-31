#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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
    
    edges.push_back(Edge(0, 1, 80));   // p1, p2
    edges.push_back(Edge(4, 8, 73));   // p5, p9
    edges.push_back(Edge(8, 12, 69));  // p9, p13
    edges.push_back(Edge(1, 2, 10));   // p2, p3
    edges.push_back(Edge(4, 6, 20));   // p5, p7
    edges.push_back(Edge(4, 1, 74));   // p5, p2
    edges.push_back(Edge(8, 17, 15));  // p9, p18
    edges.push_back(Edge(12, 13, 21)); // p13, p14
    edges.push_back(Edge(8, 0, 92));   // p9, p1
    
    int n = 20;
    int k = 3; 
    
    cout << "Photo Classification using Disjoint Sets" << endl;
    cout << "Number of photos: " << n << endl;
    cout << "Target categories: " << k << endl;
    cout << "Number of similarity edges: " << edges.size() << endl;
    
    UnionFind uf(n);
    
    for (int i = 0; i < (int)edges.size(); i++) {
        cout << "Processing edge: p" << (edges[i].u + 1) << " - p" << (edges[i].v + 1) 
             << " (similarity: " << edges[i].weight << ")" << endl;
        uf.unite(edges[i].u, edges[i].v);
        cout << "Components remaining: " << uf.getComponents() << endl;
        
        if (uf.getComponents() <= k) {
            cout << "Reached target of " << k << " categories!" << endl;
            break;
        }
    }
    
    return 0;
}