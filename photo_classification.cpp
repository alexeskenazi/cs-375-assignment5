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
    edges.push_back(Edge(12, 1, 59));  // p13, p2
    edges.push_back(Edge(19, 3, 6));   // p20, p4
    edges.push_back(Edge(17, 16, 17)); // p18, p17
    edges.push_back(Edge(10, 2, 74));  // p11, p3
    edges.push_back(Edge(6, 17, 83));  // p7, p18
    edges.push_back(Edge(6, 11, 27));  // p7, p12
    edges.push_back(Edge(0, 9, 16));   // p1, p10
    edges.push_back(Edge(4, 14, 47));  // p5, p15
    edges.push_back(Edge(19, 13, 94)); // p20, p14
    edges.push_back(Edge(13, 17, 77)); // p14, p18
    edges.push_back(Edge(1, 3, 30));   // p2, p4
    edges.push_back(Edge(10, 13, 62)); // p11, p14
    edges.push_back(Edge(2, 19, 85));  // p3, p20
    edges.push_back(Edge(10, 6, 62));  // p11, p7
    edges.push_back(Edge(5, 9, 87));   // p6, p10
    edges.push_back(Edge(11, 14, 81)); // p12, p15
    edges.push_back(Edge(2, 18, 39));  // p3, p19
    edges.push_back(Edge(8, 7, 45));   // p9, p8
    edges.push_back(Edge(18, 3, 65));  // p19, p4
    edges.push_back(Edge(7, 15, 70));  // p8, p16
    edges.push_back(Edge(14, 16, 64)); // p15, p17
    edges.push_back(Edge(0, 10, 32));  // p1, p11
    edges.push_back(Edge(10, 5, 43));  // p11, p6
    edges.push_back(Edge(11, 5, 87));  // p12, p6
    edges.push_back(Edge(11, 18, 70)); // p12, p19
    edges.push_back(Edge(6, 15, 39));  // p7, p16
    edges.push_back(Edge(12, 5, 22));  // p13, p6
    edges.push_back(Edge(3, 15, 68));  // p4, p16
    edges.push_back(Edge(16, 9, 56));  // p17, p10
    edges.push_back(Edge(18, 14, 71)); // p19, p15
    
    int n = 20;
    int k = 3; 
    
    cout << "Sorting edges by similarity (highest first)..." << endl;
    sort(edges.begin(), edges.end());
    
    cout << "Top 5 highest similarities:" << endl;
    for (int i = 0; i < 5 && i < (int)edges.size(); i++) {
        cout << "p" << (edges[i].u + 1) << " - p" << (edges[i].v + 1) 
             << ": " << edges[i].weight << endl;
    }
    
    UnionFind uf(n);
    
    for (int i = 0; i < (int)edges.size(); i++) {
        if (uf.getComponents() <= k) {
            break;
        }
        
        if (!uf.connected(edges[i].u, edges[i].v)) {
            uf.unite(edges[i].u, edges[i].v);
        }
    }
    
    cout << "Final number of categories: " << uf.getComponents() << endl;
    
    return 0;
}