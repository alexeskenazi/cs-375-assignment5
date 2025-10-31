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
    cout << "Photo Classification Program - CS375 Assignment 5" << endl;
    cout << "Problem B.1: Object Classification via Disjoint Sets" << endl;
    
    UnionFind uf(5);
    cout << "Initial components: " << uf.getComponents() << endl;
    
    uf.unite(0, 1);
    cout << "After uniting 0 and 1: " << uf.getComponents() << " components" << endl;
    
    uf.unite(2, 3);
    cout << "After uniting 2 and 3: " << uf.getComponents() << " components" << endl;
    
    return 0;
}