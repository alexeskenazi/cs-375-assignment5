# CS 375 Assignment 5

## Overview
This project implements greedy algorithms and graph theory solutions for CS 375 Assignment 5:

- **B1_photo_classification.cpp** - Photo classification using Union-Find (Disjoint Sets) to group 20 photos into 3 categories
- **B2_shortest_paths.cpp** - All-pairs shortest paths via capital city using two algorithms (O(n log n) and O(n²))
- **B3_bellman_ford.cpp** - Bellman-Ford shortest path algorithm (optional bonus, +10%)

## How to Run

### Compile All Programs
```bash
make all
```

### Run Individual Programs
```bash
make test_photo    # Run B1 - Photo classification
make test_paths    # Run B2 - Shortest paths
make test_bellman  # Run B3 - Bellman-Ford
```

### Run All Programs
```bash
make test
```

### Clean Build Files
```bash
make clean
```

## Expected Results

| Program | Expected Output |
|---------|----------------|
| B1 | 3 groups: Group 1 (5 photos), Group 2 (9 photos), Group 3 (6 photos) |
| B2 | Algorithm 1: d→i distance 70, f→g distance 47 <br> Algorithm 2: Same results with precomputed paths |
| B3 | Same results as B2 using Bellman-Ford instead of Dijkstra |

## Documentation

- [Assignment Report](assignment_report.md) - Complete project report with algorithm descriptions, code explanations, and results
- [Solution Explanation](SOLUTION_EXPLANATION.md) - Step-by-step explanation of the approach for each problem

## Input Files

- **B1_input.txt** - 39 similarity edges for photo classification
- **B2_input.txt** - Graph edges and query pairs for shortest path problems

## Output Files

- **B1_output.txt** - Photo classification results (3 groups)
- **B2_output.txt** - Shortest path results for both algorithms
- **B3_output.txt** - Bellman-Ford shortest path results
