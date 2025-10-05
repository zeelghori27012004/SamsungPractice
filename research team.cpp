#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>

using namespace std;

// Maximum size constraint N <= 20
const int MAX_N = 22;

// Directions for BFS (Up, Down, Left, Right)
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

// Structure to hold a cell coordinate and distance
struct Cell {
    int r, c, dist;
};

/**
 * @brief Performs Breadth-First Search (BFS) starting from (start_r, start_c)
 * and returns the maximum shortest distance to any rare-element location.
 *
 * @param N Grid size.
 * @param C Number of rare-element locations.
 * @param region The grid map (1=road, 0=no road).
 * @param locations Vector of rare-element coordinates.
 * @param start_r Row of the potential research center.
 * @param start_c Column of the potential research center.
 * @return The maximum shortest distance to a rare-element location. Returns INT_MAX if any element is unreachable.
 */
int run_bfs(int N, int C, const vector<vector<int>>& region, 
            const vector<pair<int, int>>& locations, 
            int start_r, int start_c) {

    // visited array stores the shortest distance from (start_r, start_c).
    // Initialized to -1 (unvisited).
    vector<vector<int>> visited(N + 1, vector<int>(N + 1, -1));
    queue<Cell> q;

    // Start BFS from the potential research center
    q.push({start_r, start_c, 0});
    visited[start_r][start_c] = 0;

    // Standard iterative BFS
    while (!q.empty()) {
        Cell current = q.front();
        q.pop();

        int r = current.r;
        int c = current.c;
        int dist = current.dist;

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check boundaries (1-based indexing) and if unvisited
            if (nr >= 1 && nr <= N && nc >= 1 && nc <= N && visited[nr][nc] == -1) {
                // Check if the neighbor is a road
                if (region[nr][nc] == 1) {
                    visited[nr][nc] = dist + 1;
                    q.push({nr, nc, dist + 1});
                }
            }
        }
    }

    // 2. Calculate the maximum shortest distance (D_max) to the rare-elements
    int max_dist = 0;
    for (const auto& loc : locations) {
        int r_loc = loc.first;
        int c_loc = loc.second;

        // The distance is stored in visited[r_loc][c_loc]
        int dist_to_element = visited[r_loc][c_loc];

        // If any rare-element location is unreachable, this center is invalid
        if (dist_to_element == -1) {
            return INT_MAX; // Use a very large value
        }
        
        // Update the maximum distance
        max_dist = max(max_dist, dist_to_element);
    }

    return max_dist;
}

void solve() {
    int T;
    if (!(cin >> T)) return; // Read number of test cases

    for (int t = 1; t <= T; ++t) {
        int N, C;
        if (!(cin >> N >> C)) return; // Read N (size) and C (count)

        // 1-based indexing for input: locations and grid size N+1
        vector<pair<int, int>> locations(C);
        for (int i = 0; i < C; ++i) {
            cin >> locations[i].first >> locations[i].second;
        }

        vector<vector<int>> region(N + 1, vector<int>(N + 1));
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                cin >> region[i][j];
            }
        }

        int min_max_distance = INT_MAX;

        // 1. Iterate through every cell as a potential research center (r, c)
        for (int r = 1; r <= N; ++r) {
            for (int c = 1; c <= N; ++c) {
                
                // The center MUST be built on a road
                if (region[r][c] == 1) {
                    
                    // 2. Run BFS from this road cell
                    int current_max_dist = run_bfs(N, C, region, locations, r, c);
                    
                    // 3. Find the minimum of all D_max values
                    min_max_distance = min(min_max_distance, current_max_dist);
                }
            }
        }

        // Output the result for the current test case
        cout << min_max_distance << endl;
    }
}

// int main() {
//     // For faster input/output
//     ios_base::sync_with_stdio(false);
//     cin.tie(NULL);
//     solve();
//     return 0;
// }
