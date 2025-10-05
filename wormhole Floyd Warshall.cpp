#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

// Maximum number of wormholes N=30 -> V=62 vertices.
const int MAX_V = 65; 
// Use a large value for infinity
const long long INF = 1e15; 

// Wormhole structure for clarity: (x1, y1) -> (x2, y2) with cost C
struct Wormhole {
    int x1, y1, x2, y2, cost;
};

// Global distance matrix for Floyd-Warshall
long long dist_matrix[MAX_V][MAX_V];

// Manhattan distance calculator
long long manhattan_dist(long long x1, long long y1, long long x2, long long y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Function to solve the problem using Floyd-Warshall
long long solve() {
    int N; // Number of wormholes
    if (!(cin >> N)) return 0;
    
    long long sX, sY, tX, tY; // Source and Destination coordinates
    if (!(cin >> sX >> sY >> tX >> tY)) return 0;

    // V = 2 (Source/Dest) + 2*N (Wormhole endpoints)
    int V = 2 * N + 2; 

    // Store the coordinates of all V points
    vector<pair<long long, long long>> coords(V);
    
    // Store the wormholes
    vector<Wormhole> wormholes(N);

    // 1. Map all physical points to vertices (indices 0 to V-1)
    
    // Vertex 0: Source
    coords[0] = {sX, sY};
    
    // Read wormholes and map endpoints to vertices 1 to 2N
    for (int i = 0; i < N; ++i) {
        cin >> wormholes[i].x1 >> wormholes[i].y1 >> wormholes[i].x2 >> wormholes[i].y2 >> wormholes[i].cost;
        
        // Vertex 1 to N: Start points of wormholes
        coords[i + 1] = {wormholes[i].x1, wormholes[i].y1};
        // Vertex N+1 to 2N: End points of wormholes
        coords[i + N + 1] = {wormholes[i].x2, wormholes[i].y2};
    }

    // Vertex 2N+1: Destination
    coords[V - 1] = {tX, tY};


    // 2. Initialize the distance matrix with Manhattan distances (initial direct paths)
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (i == j) {
                dist_matrix[i][j] = 0;
            } else {
                dist_matrix[i][j] = manhattan_dist(coords[i].first, coords[i].second, coords[j].first, coords[j].second);
            }
        }
    }

    // 3. Apply wormhole costs (shortcuts)
    for (int i = 0; i < N; ++i) {
        int u = i + 1;       // Start point vertex index
        int v = i + N + 1;   // End point vertex index
        int cost = wormholes[i].cost;

        // Wormhole is bi-directional: u to v and v to u cost 'cost'
        dist_matrix[u][v] = min(dist_matrix[u][v], (long long)cost);
        dist_matrix[v][u] = min(dist_matrix[v][u], (long long)cost);
    }

    // 4. Run Floyd-Warshall Algorithm (O(V^3))
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                // Relaxation step: Is it cheaper to go i -> k -> j?
                if (dist_matrix[i][k] != INF && dist_matrix[k][j] != INF) {
                    dist_matrix[i][j] = min(dist_matrix[i][j], dist_matrix[i][k] + dist_matrix[k][j]);
                }
            }
        }
    }

    // The result is the minimum distance from Source (index 0) to Destination (index V-1)
    return dist_matrix[0][V - 1];
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t; // Number of test cases
    if (!(cin >> t)) return 0;
    
    while (t--) {
        cout << solve() << "\n";
    }

    return 0;
}
