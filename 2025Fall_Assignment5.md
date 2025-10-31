# Design and Analysis of Algorithms

**CS375-A0/B1 Assignment 5**

**Part A: Due on Nov. 12 (by 11:59pm)**

## Objectives
1) Enhance the concept of greedy algorithms  
2) Design a program for greedy algorithm implementation.

---

## Part A — 75%

### 1. (7%) Minimum Spanning Tree via Prim's Algorithm
Find the minimum spanning tree (MST) using Prim’s algorithm for the given graph, starting at node **A**.  
- Show the order that the nodes are added into the MST.  
- Draw the MST and indicate the total weight of the MST.

> _[Graph from the original document not embedded here.]_

---

### 2. (8%) Conference Session Room Scheduling
We need to schedule a series of sessions across a minimal number of hotel conference rooms (to reduce budget). For example, schedule the following sessions:

- Session 1 (Education): 9:30–10:30am  
- Session 2 (Economics): 9:30–10:30am  
- Session 3 (Security): 1:30–2:30pm  
- Session 4 (Med-Care): 12:00–1:00pm  
- Session 5 (Insurance): 10:30–11:30am  
- Session 6 (Science): 11:30am–12:30pm  
- Session 7 (Technology): 11:30am–12:30pm  
- Session 8 (Transportation): 1:00–2:00pm

Design an efficient algorithm to determine how many conference rooms are needed. Your algorithm must be no worse than **O(n^2)** in the worst case. Show your solution using an **active selection strategy**. What is the time complexity if **k** rooms are needed?

---

### 3. (6%) Greedy Coin Change Counterexample
Show a counterexample demonstrating that if the 5-cent coin is removed from U.S. denominations (i.e., denominations are {25, 10, 1}), the greedy algorithm for coin change does **not** guarantee a minimal number of coins.

---

### 4. (7%) MST Edge Uniqueness Property
Explain why, if there exists one edge with a weight smaller than all other edges, that edge will be part of **every** minimum spanning tree.

---

### 5. (7%) Prim vs. BFS Ordering
Explain whether it is **always**, **never**, or **sometimes** true that the order in which nodes are added to the MST by **Prim's algorithm** is the same as the order in which they are encountered in a **breadth-first search** (BFS).

#### i) (7%) Dijkstra’s Algorithm
Run Dijkstra’s algorithm on the given graph starting at node **A**.  
(a) Draw a table showing the intermediate distance values of all nodes at each iteration.  
(b) Show the final shortest-path tree.

> _[Graph from the original document not embedded here.]_

#### ii) (7%) Bellman–Ford Algorithm
Apply the Bellman–Ford algorithm to the **same graph** (starting at node **A**).  
- Draw the relaxation table showing distances and paths.

---

### 6. (7%) Kruskal’s Algorithm for Road Network Planning
To construct a road network with minimum total cost, use **Kruskal’s algorithm** to find the MST.  
- Show intermediate steps and the final MST.  
- Indicate the **total length** of the road network.

---

### 7. (8%) Packing Files onto CDs
You are downloading **n** files (songs/programs) and want to store them on the minimum number of CDs. Each CD can store exactly **B** bytes. More than one file can be stored on a CD, but a single file **cannot** be split across CDs. Each file *i* has size *s<sub>i</sub>* bytes (assume *s<sub>i</sub>* < *B*).  
- Describe an **O(n log n)** algorithm to solve this problem.  
- If your algorithm can guarantee an **optimal** solution, prove it. Otherwise, provide a counterexample.

---

### 9. (5%) Prim’s Algorithm and Connectivity
The notion of an MST applies to a **connected** weighted graph. Do we have to check a graph’s connectivity **before** applying Prim’s algorithm, or can the algorithm detect this by itself? Explain your answer.

---

### 10. (6%) Maximum Spanning Tree
Design an algorithm for finding a **maximum** spanning tree—a spanning tree with the **largest** possible total edge weight—for a weighted connected graph.

---

## Part B — Algorithm Design and Programming (25%)

### 1. (10%) Object Classification via Disjoint Sets
Object classification is a key task in pattern analysis (e.g., clustering by shape or color). In this assignment, use **disjoint-set (union–find)** to classify a set of objects into categories.

**a.** The similarity between a pair of objects is a value from **0** to **100**. Sort similarities in **non-increasing** order. (Higher value = higher similarity.)

**b.** Given a set of objects and pairwise similarities, design an algorithm to find a number of **disjoint subsets** (i.e., create **multiple maximum spanning trees**, one per category). Each category corresponds to a single **maximum spanning tree** (we say “maximum” because we want to maximize similarity).

**c.** Suppose the number of categories is known. Design a **greedy** algorithm (time complexity no more than **O(n log n)**) using appropriate data structures for efficient time and space.  
**Problem:** There are 20 photos with three categories (trees, animals, buildings). Assume photos within the same category have higher similarity than across categories. Classify 20 photos \(G = (p_1, p_2, \dots, p_{20})\) into **three disjoint subsets**, and print:  
- the number of photos in each subset,  
- the names of photos in each subset, and  
- the running time.

**39 similarity edges** (S(x, y)) are provided:

```
S(p1, p2) = 80
S(p5, p9) = 73
S(p9, p13) = 69
S(p2, p3) = 10
S(p5, p7) = 20
S(p5, p2) = 74
S(p9, p18) = 15
S(p13, p14) = 21
S(p9, p1) = 92
S(p13, p2) = 59
S(p20, p4) = 6
S(p18, p17) = 17
S(p11, p3) = 74
S(p7, p18) = 83
S(p7, p12) = 27
S(p1, p10) = 16
S(p5, p15) = 47
S(p20, p14) = 94
S(p14, p18) = 77
S(p2, p4) = 30
S(p11, p14) = 62
S(p3, p20) = 85
S(p11, p7) = 62
S(p6, p10) = 87
S(p12, p15) = 81
S(p3, p19) = 39
S(p9, p8) = 45
S(p19, p4) = 65
S(p8, p16) = 70
S(p15, p17) = 64
S(p1, p11) = 32
S(p11, p6) = 43
S(p12, p6) = 87
S(p12, p19) = 70
S(p7, p16) = 39
S(p13, p6) = 22
S(p4, p16) = 68
S(p17, p10) = 56
S(p19, p15) = 71
```

---

### 2. (15%) All-Pairs Shortest Paths via a Given Capital
We are given a connected, undirected graph \(G = (V, E)\) with **positive** edge weights representing distances between cities. A particular city is the **capital** \(a \in V\). Give efficient algorithms for finding shortest paths between **all pairs of cities**, with the restriction that these paths must **all pass through the capital**.

1) **Algorithm 1 (O(n log n))**: Allowed to visit cities more than once.  
2) **Algorithm 2 (O(n^2) or better)**: No city is allowed to be visited more than once.  
3) **Implementation/Validation:** Implement both algorithms and test using the provided map (assume the capital is node “a”).  
   - Print the shortest path from **d** to **i** via **a** for each algorithm.  
   - Print the shortest path from **f** to **g** via **a** for each algorithm.  
   - Print the **running time** for both implementations.  
   - Ensure your implementations are efficient in **memory** and **time**.

> _[Maps/graphs from the original document not embedded here.]_

---

### 3. (Optional, +10%) Bellman–Ford for Part B.2
Implement the **Bellman–Ford** algorithm to solve problem **B.2**.

> _[Graph figures and labels (a–j, etc.) from the original document are not embedded here.]_
