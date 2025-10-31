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

int main() {
    cout << "Photo Classification Program - CS375 Assignment 5" << endl;
    cout << "Problem B.1: Object Classification via Disjoint Sets" << endl;
    
    Edge e1(1, 2, 80);
    Edge e2(3, 4, 50);
    cout << "Testing Edge structure: Edge(1,2,80) > Edge(3,4,50)? " << (e1 < e2 ? "true" : "false") << endl;
    
    return 0;
}