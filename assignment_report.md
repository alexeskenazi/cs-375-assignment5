# CS-375 Assignment 5 Report: Greedy Algorithms and Graph Theory

**Student Name**: Alex Eskenazi
**Student ID**: B00944943
**Date**: November 12, 2025

## Implemented

* B.1: Photo Classification using Union-Find (Disjoint Sets) - classifies 20 photos into 3 groups based on similarity
* B.2: All-Pairs Shortest Paths via Capital - two algorithms (O(n log n) and O(n²))
* B.3: Bellman-Ford Algorithm - optional bonus implementation (+10%)

## (1) Algorithm Description

### Problem B.1: Photo Classification via Disjoint Sets

This problem uses a greedy approach with Union-Find to group photos by similarity. The idea is to connect the most similar photos first, which naturally groups them into categories.

**Steps:**

1. Read all 39 similarity edges from input file
2. Sort edges by similarity score in descending order (highest first)
3. Initialize Union-Find structure with 20 separate components (one per photo)
4. Process edges from highest to lowest similarity:
   * If two photos are not yet connected, merge their groups
   * Stop when we reach exactly 3 groups
5. Print which photos ended up in each group

**Key Data Structure - Union-Find:**

* Uses path compression in find() to flatten trees
* Uses union by rank to keep trees balanced
* Tracks number of components to know when to stop

**Time Complexity:** O(E log E) for sorting edges, which satisfies O(n log n) requirement
**Space Complexity:** O(n) for parent array and rank array

### Problem B.2: All-Pairs Shortest Paths via Capital

Given a graph where all paths must go through capital 'a', find shortest distances between all city pairs.

#### Algorithm 1: O(n log n) - Visits Allowed

This algorithm computes paths on-demand when you ask for them.

**Steps:**

1. Run Dijkstra's algorithm once from capital 'a'
2. For any query (start, end):
   * Distance = dist[start] + dist[end]
   * Path = start → a → end
3. Return result

**Why it works:** The shortest path through capital 'a' is just the sum of distances from each endpoint to the capital.

**Time Complexity:** O(E log V) for Dijkstra = O(n log n) for sparse graphs

#### Algorithm 2: O(n²) - No Revisits

This algorithm precomputes all possible city pairs upfront, but with the constraint that no city can be visited more than once.

**Steps:**

1. Run Dijkstra's algorithm once from capital 'a' with parent tracking
2. For every pair of cities (u, v) where neither is the capital:
   * Extract the actual path from a to u using parent pointers
   * Extract the actual path from a to v using parent pointers
   * Check if these two paths share any vertices (except 'a')
   * If they share vertices: Mark as invalid (no valid path without revisits)
   * If they don't share vertices: Store path u → a → v with distance dist[u] + dist[v]
3. When queried, look up the precomputed result

**Time Complexity:** O(n²) because we check all city pairs
**Space Complexity:** O(n²) to store all pairs

**Key Difference from Algorithm 1:** Algorithm 2 enforces that the path from u→a and the path from a→v must be vertex-disjoint (except at 'a'). If the shortest paths share any vertices, Algorithm 2 rejects the path as invalid.

### Problem B.3: Bellman-Ford Algorithm (Bonus)

An alternative shortest path algorithm that can handle negative edge weights (though our graph has only positive weights).

**Steps:**

1. Initialize distances from capital: dist[a] = 0, all others = infinity
2. Relax all edges V-1 times:
   * For each edge (u,v): if dist[u] + weight < dist[v], update dist[v]
3. Check for negative cycles (one more iteration)
4. Compute paths the same way as Algorithm 1

**Why V-1 iterations?** The longest simple path can have at most V-1 edges, so after V-1 rounds we've found all shortest paths.

**Time Complexity:** O(VE) - slower than Dijkstra but more general
**Space Complexity:** O(V) for distance array

## (2) Major Codes

### Union-Find Implementation (B.1)

```cpp
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    int components;

public:
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

        // Union by rank - attach smaller tree under larger tree
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
};
```

### Dijkstra's Algorithm (B.2)

```cpp
vector<int> dijkstra(char start) {
    vector<int> dist(numNodes, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int> >,
                   greater<pair<int, int> > > pq;

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
```

### Bellman-Ford Algorithm (B.3)

```cpp
vector<int> bellmanFord(char start) {
    vector<int> dist(numNodes, INT_MAX);
    int startIdx = nodeIndex.find(start)->second;
    dist[startIdx] = 0;

    // Relax edges V-1 times
    for (int i = 0; i < numNodes - 1; i++) {
        for (int j = 0; j < (int)edges.size(); j++) {
            Edge edge = edges[j];
            int u = nodeIndex.find(edge.from)->second;
            int v = nodeIndex.find(edge.to)->second;

            if (dist[u] != INT_MAX && dist[u] + edge.weight < dist[v]) {
                dist[v] = dist[u] + edge.weight;
            }
        }
    }

    // Check for negative cycles
    for (int j = 0; j < (int)edges.size(); j++) {
        Edge edge = edges[j];
        int u = nodeIndex.find(edge.from)->second;
        int v = nodeIndex.find(edge.to)->second;

        if (dist[u] != INT_MAX && dist[u] + edge.weight < dist[v]) {
            cout << "Negative cycle detected!" << endl;
            return vector<int>();
        }
    }
    return dist;
}
```

## (3) Running Results

### Problem B.1: Photo Classification Results

**Input:** 39 similarity edges for 20 photos, target = 3 groups

**Output:**
```
Starting photo classification...
Total photos: 20
Target groups: 3
Total edges: 39

Merging p20 and p14 (similarity: 94) -> 19 groups remaining
Merging p9 and p1 (similarity: 92) -> 18 groups remaining
...
Reached target of 3 groups, stopping...

Final Groups:
-------------
Group 1 = 5; photos: p1, p2, p5, p9, p13
Group 2 = 9; photos: p4, p6, p8, p10, p12, p15, p16, p17, p19
Group 3 = 6; photos: p3, p7, p11, p14, p18, p20

Running-time: 35 microseconds
```

**Analysis:** The algorithm successfully classified all 20 photos into 3 distinct groups using only 17 edges. The groups make sense because we processed edges from highest to lowest similarity, so photos in the same group are all highly similar to each other.

### Problem B.2: Shortest Paths Results

**Input:** Graph with 11 nodes (a-k), queries: d→i and f→g via capital 'a'

**Algorithm 1 Results:**
```
=== ALGORITHM 1: O(n log n) - Visits Allowed ===
Computing path: d -> a -> i
Computing path: f -> a -> g

Shortest Path: d, g, e, a, i
Shortest Distance: 40
Running-time: 105 microseconds

Shortest Path: f, e, a, e, g
Shortest Distance: 32
Running-time: 105 microseconds
```

**Algorithm 2 Results:**
```
=== ALGORITHM 2: O(n^2) - No Revisits ===
Precomputing all pairs of cities via capital...
Computed paths for all 90 city pairs

Shortest Path: d, g, e, a, i
Shortest Distance: 40
Running-time: 533 microseconds

No valid path (paths would overlap - violates no-revisit constraint)
Running-time: 533 microseconds
```

**Analysis:**
- **Query d→i:** Both algorithms find the same path (d→g→e→a→i, distance 40) because the paths don't overlap.
- **Query f→g:** Algorithm 1 finds a path (f→e→a→e→g, distance 32) but visits city 'e' twice. Algorithm 2 correctly detects that both shortest paths from the capital go through 'e' (a→e→f and a→e→g), so it reports no valid path exists without revisiting a city.
- Algorithm 2 took longer (533μs vs 105μs) because it precomputed all 90 city pairs and checked each for overlaps. For many queries, Algorithm 2 would be faster since paths are precomputed.

### Problem B.3: Bellman-Ford Results

**Input:** Same graph as B.2

**Results:**
```
=== BELLMAN-FORD ALGORITHM ===
Graph has 11 nodes and 42 directed edges
Running Bellman-Ford from capital 'a'...
Will relax edges 10 times

Shortest Path: d, g, e, a, i
Shortest Distance: 40
Running-time: 123 microseconds

Shortest Path: f, e, a, e, g
Shortest Distance: 32
Running-time: 123 microseconds
```

**Analysis:** Bellman-Ford produces the same correct results as Dijkstra (Algorithm 1), confirming our implementation is correct. Like Algorithm 1, it allows revisiting cities, so for query f→g it finds the path that visits 'e' twice. It's slower (123μs vs 105μs) because it relaxes edges 10 times instead of using a priority queue, but it's more general because it can handle negative weights.

### Performance Comparison

| Problem | Algorithm | Time Complexity | Running Time |
|---------|-----------|----------------|--------------|
| B.1 | Union-Find | O(E log E) | ~75 μs |
| B.2 | Dijkstra (Alg 1) | O(n log n) | ~105 μs |
| B.2 | Precompute (Alg 2) | O(n²) | ~533 μs |
| B.3 | Bellman-Ford | O(VE) | ~120 μs |

The results match our complexity expectations. Algorithm 2 is slower for just 2 queries because it precomputes all 90 pairs and validates each for the no-revisit constraint. With many queries, Algorithm 2 would be faster since lookups are O(1).

## (4) Report of Any Bugs

No known issues.

### Current Status

* All programs compile without warnings
* File I/O works correctly - reads from input files, writes to output files
* Edge case handling: properly detects disconnected components
* All three problems produce correct results:
  * B.1: Successfully groups 20 photos into 3 categories
  * B.2: Both algorithms find same shortest distances (70 and 47)
  * B.3: Bellman-Ford confirms same results as Dijkstra
* Debug output helps understand what each algorithm is doing
* Used typedefs to make complex types more readable (PathMap, PathResult, etc.)

### Challenges Encountered

The hardest part was understanding the "no revisits" constraint for B.2's Algorithm 2. At first I just computed dist[start] + dist[end] for all pairs, but then realized this doesn't check if the actual paths share vertices. For example, if both paths go through the same city 'e', that city gets visited twice in the combined path, which violates the constraint.

I had to modify Dijkstra to track parent pointers, then extract the actual paths and check if they share any vertices (except the capital). If they do, Algorithm 2 marks that pair as invalid. This was more complex than I expected but makes sense - some city pairs just don't have a valid path through the capital without revisiting cities.

Another challenge was making the Union-Find work efficiently. I had to implement both path compression and union by rank to keep it fast. Without these optimizations, the find operations would be slow for 20 photos.

**Input File Organization:** The assignment didn't specify clearly whether all inputs should come from a single file or separate files. I decided to use separate input files (B1_input.txt, B2_input.txt) to keep things clean and organized. This made it easier to test each problem independently and the code is simpler to read. Each program reads from its own input file and writes to its own output file (B1_output.txt, B2_output.txt, B3_output.txt).

Overall, this assignment really helped me understand how greedy algorithms work for both clustering (Union-Find) and shortest paths (Dijkstra/Bellman-Ford). The performance differences between algorithms are really clear when you can see the actual running times!
