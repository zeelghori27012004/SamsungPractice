#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include <queue>
#include <tuple>

using namespace std;

// Use a large value for infinity (long long is necessary for distances)
const long long INF = 1e18; 

// Wormhole structure
struct Wormhole {
    long long x1, y1, x2, y2, cost;
};

// Represents a weighted edge (destination vertex, weight/cost)
using Edge = pair<int, long long>;

// Manhattan distance calculator
long long manhattan_dist(long long x1, long long y1, long long x2, long long y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Function to solve the problem using Dijkstra's algorithm
long long solve_dijkstra() {
    int N; // Number of wormholes
    if (!(cin >> N)) return 0;
    
    long long sX, sY, tX, tY; // Source and Destination coordinates
    if (!(cin >> sX >> sY >> tX >> tY)) return 0;

    // V = 2 (Source/Dest) + 2*N (Wormhole endpoints)
    int V = 2 * N + 2; 

    // Store the coordinates of all V points (indexed 0 to V-1)
    vector<pair<long long, long long>> coords(V);
    
    // Store the wormholes
    vector<Wormhole> wormholes(N);

    // 1. Map all physical points to vertices
    
    // Vertex 0: Source
    coords[0] = {sX, sY};
    
    // Read wormholes and map endpoints to vertices
    for (int i = 0; i < N; ++i) {
        cin >> wormholes[i].x1 >> wormholes[i].y1 >> wormholes[i].x2 >> wormholes[i].y2 >> wormholes[i].cost;
        
        // Vertex 1 to N: Start points (index i + 1)
        coords[i + 1] = {wormholes[i].x1, wormholes[i].y1};
        // Vertex N+1 to 2N: End points (index i + N + 1)
        coords[i + N + 1] = {wormholes[i].x2, wormholes[i].y2};
    }

    // Vertex 2N+1: Destination
    coords[V - 1] = {tX, tY};

    // 2. Build the Adjacency List
    // Since the graph is dense (fully connected by Manhattan distance), 
    // it's easier to calculate the edge weight on the fly in Dijkstra's.
    // If we pre-calculate the graph, it's O(V^2) edges.
    
    // An initial distance matrix to store the minimum cost between any two points
    // (either Manhattan distance or wormhole shortcut)
    vector<vector<long long>> adj_dist(V, vector<long long>(V, INF));
    
    // a) Initialize with Manhattan Distance
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (i == j) {
                adj_dist[i][j] = 0;
            } else {
                adj_dist[i][j] = manhattan_dist(coords[i].first, coords[i].second, coords[j].first, coords[j].second);
            }
        }
    }

    // b) Apply Wormhole Costs
    for (int i = 0; i < N; ++i) {
        int u = i + 1;       // Start point vertex index
        int v = i + N + 1;   // End point vertex index
        long long cost = wormholes[i].cost;

        // Wormhole is bi-directional: update with the minimum of current cost (Manhattan) and wormhole cost.
        adj_dist[u][v] = min(adj_dist[u][v], cost);
        adj_dist[v][u] = min(adj_dist[v][u], cost);
    }
    
    // 3. Dijkstra's Algorithm
    
    // Priority Queue: pair<long long, int> = {cost, vertex_index}
    // min-heap is used to extract the path with the smallest cost first
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    // Distance vector: dist[i] stores the shortest cost from source (0) to vertex i
    vector<long long> dist(V, INF);
    
    int source = 0;
    dist[source] = 0;
    pq.push({0, source}); // {cost, source_vertex}
    
    int destination = V - 1;

    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Check for stale entry
        if (d > dist[u]) continue;

        // Found the destination, can stop early
        if (u == destination) return dist[u];

        // Relax edges to all other vertices (dense graph)
        for (int v = 0; v < V; ++v) {
            if (u == v) continue;
            
            long long weight = adj_dist[u][v]; // Weight of edge u -> v

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    // If the loop finishes, the shortest distance is in dist[destination]
    return dist[destination];
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t; // Number of test cases
    if (!(cin >> t)) return 0;
    
    while (t--) {
        cout << solve_dijkstra() << "\n";
    }

    return 0;
}
