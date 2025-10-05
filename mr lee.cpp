#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int T, N;
const int MAXN = 12;        // Maximum number of cities
int cost[MAXN][MAXN];       // Cost matrix
int dp[1 << MAXN][MAXN];    // DP table: dp[mask][i] = min cost to visit cities in mask, ending at city i

int tsp(int mask, int pos) {
    // Base Case: All cities visited
    if(mask == (1 << N) - 1) {
        // Return to starting city (0)
        return cost[pos][0] != 0 ? cost[pos][0] : INT_MAX;
    }

    // Already computed
    if(dp[mask][pos] != -1) return dp[mask][pos];

    int ans = INT_MAX;

    for(int next = 0; next < N; next++) {
        // If next city not visited and path exists
        if(!(mask & (1 << next)) && cost[pos][next] != 0) {
            int temp = tsp(mask | (1 << next), next);
            if(temp != INT_MAX)
                ans = min(ans, cost[pos][next] + temp);
        }
    }

    return dp[mask][pos] = ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> T;
    while(T--) {
        cin >> N;

        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++)
                cin >> cost[i][j];

        // Initialize DP table with -1
        for(int mask = 0; mask < (1 << N); mask++)
            for(int i = 0; i < N; i++)
                dp[mask][i] = -1;

        int ans = tsp(1, 0); // Start from city 0 (mask = 1<<0)

        cout << (ans != INT_MAX ? ans : -1) << "\n";
    }
    return 0;
}



#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int N; // Number of offices
int min_fare; // Stores the minimum total airfare found

/**
 * @brief Performs a Depth-First Search (Backtracking) to find the minimum cost Hamiltonian cycle.
 *
 * @param arr The adjacency matrix (airfares).
 * @param visited A boolean array to track visited offices.
 * @param count The number of offices visited so far (excluding the starting office 0).
 * @param current_cost The accumulated airfare for the current path.
 * @param current_city The index of the city Mr. Lee is currently at.
 */
void findMinCost(const vector<vector<int>>& arr, vector<bool>& visited, int count, int current_cost, int current_city) {
    // Optimization: If the current cost is already greater than or equal to the best found result, prune the branch.
    if (current_cost >= min_fare) {
        return;
    }

    // Base Case: All N-1 other offices have been visited.
    if (count == N - 1) {
        // Check if a path exists back to the company (office 0)
        if (arr[current_city][0] != 0) {
            // Update the global minimum result
            min_fare = min(min_fare, current_cost + arr[current_city][0]);
        }
        return;
    }

    // Recursive Step: Try to move to any unvisited city
    for (int next_city = 1; next_city < N; ++next_city) { // Start from 1, as 0 is the destination only in the base case
        // Check if the next_city hasn't been visited AND a path exists
        if (!visited[next_city] && arr[current_city][next_city] != 0) {
            // 1. Mark as visited
            visited[next_city] = true;

            // 2. Recurse (DFS)
            findMinCost(arr, visited, count + 1, current_cost + arr[current_city][next_city], next_city);

            // 3. Backtrack (Unmark)
            visited[next_city] = false;
        }
    }
}

void solve() {
    if (!(cin >> N)) return;

    // Use std::vector for robust memory management
    vector<vector<int>> arr(N, vector<int>(N));
    vector<bool> visited(N, false);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> arr[i][j];
        }
    }

    min_fare = INT_MAX;

    // Start the search from the company (office 0)
    visited[0] = true;
    findMinCost(arr, visited, 0, 0, 0);

    // Output the result. If min_fare is still INT_MAX, no complete tour was found.
    if (min_fare != INT_MAX) {
        cout << min_fare << "\n";
    } else {
        cout << "-1\n";
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        solve();
    }
    return 0;
}
