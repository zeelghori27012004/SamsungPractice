#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

// Structure to hold coordinates
struct Point {
    int x, y;
};

/**
 * @brief Calculates the Manhattan distance between two points.
 * @param p1 First point.
 * @param p2 Second point.
 * @return The Manhattan distance: |x1 - x2| + |y1 - y2|.
 */
int manhattan_dist(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

/**
 * @brief Recursive Depth-First Search (Backtracking) to explore all permutations of customer visits.
 * * @param N The total number of customers.
 * @param D The pre-calculated distance matrix (D[i][j] = distance between location i and j).
 * @param current_loc_index Index of the last location visited (0: Office, 1..N: Customer, N+1: Home).
 * @param visited_count Number of customers visited so far.
 * @param current_distance Distance accumulated up to current_loc_index.
 * @param min_total_cost Reference to the global minimum path length found.
 * @param visited_customers Boolean array to track visited customers (indices 1 to N).
 */
void find_shortest_path_dfs(int N, const vector<vector<int>>& D, 
                            int current_loc_index, int visited_count, 
                            int current_distance, int& min_total_cost, 
                            vector<bool>& visited_customers) {

    // --- Optimization (Pruning) ---
    // If the current path is already longer than the best path found so far, stop.
    if (current_distance >= min_total_cost) {
        return;
    }

    // --- Base Case: All customers visited ---
    if (visited_count == N) {
        // The final leg is from the current location to the Home (index N + 1)
        int total_cost = current_distance + D[current_loc_index][N + 1];
        min_total_cost = min(min_total_cost, total_cost);
        return;
    }

    // --- Recursive Step: Try to visit the next unvisited customer ---
    // Customer indices range from 1 to N
    for (int next_customer_index = 1; next_customer_index <= N; ++next_customer_index) {
        
        // Check if the customer has NOT been visited yet
        if (!visited_customers[next_customer_index]) {
            
            // 1. Mark as visited
            visited_customers[next_customer_index] = true;
            
            // 2. Calculate the cost to reach this new customer
            int cost_to_next = D[current_loc_index][next_customer_index];
            
            // 3. Recursive Call (DFS)
            find_shortest_path_dfs(
                N, D, 
                next_customer_index,                // New current location
                visited_count + 1,                  // One more customer visited
                current_distance + cost_to_next,    // New accumulated distance
                min_total_cost, 
                visited_customers
            );
            
            // 4. Backtrack (Unmark as visited for the next permutation/path)
            visited_customers[next_customer_index] = false;
        }
    }
}

/**
 * @brief Solves the Kim's Refrigerator Delivery problem using Recursive Backtracking (DFS).
 */
void solve_test_case(int t) {
    int N; // Number of customers (5 <= N <= 10)
    if (!(cin >> N)) return;

    // Total number of locations: Office (0), N Customers (1 to N), Home (N+1)
    const int M = N + 2;
    vector<Point> locations(M);

    // Read Office (index 0) and Home (index N+1) coordinates
    // Input order is Office x, y, then Home x, y.
    cin >> locations[0].x >> locations[0].y;
    cin >> locations[N + 1].x >> locations[N + 1].y;

    // Read Customer coordinates (indices 1 to N)
    for (int i = 1; i <= N; ++i) {
        cin >> locations[i].x >> locations[i].y;
    }

    // --- 1. Pre-calculate all inter-location distances ---
    // D[i][j] stores the distance between location i and location j.
    vector<vector<int>> D(M, vector<int>(M));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            D[i][j] = manhattan_dist(locations[i], locations[j]);
        }
    }

    // --- 2. Initialization for DFS ---
    int min_total_cost = INT_MAX;
    
    // We only track the N customers (indices 1 to N). Size N+1 for 1-based indexing.
    vector<bool> visited_customers(N + 1, false); 

    // Start the DFS from the Office (index 0). 
    // Initial visited count is 0, initial distance is 0.
    find_shortest_path_dfs(
        N, D, 
        0,                  // Start at Office (index 0)
        0,                  // 0 customers visited initially
        0,                  // 0 distance accumulated initially
        min_total_cost,     // Reference to the answer
        visited_customers
    );

    // Output the result
    cout << "#" << t << " " << min_total_cost << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int tCases;
    // Read the total number of test cases
    if (!(cin >> tCases)) return 0;

    for (int t = 1; t <= tCases; ++t) {
        solve_test_case(t);
    }

    return 0;
}




Recursion + memoiztion


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

// Structure to hold coordinates
struct Point {
    int x, y;
};

// Define a large constant for initialization of the memoization table
const int INF = 1000000000; 

/**
 * @brief Calculates the Manhattan distance between two points.
 * @param p1 First point.
 * @param p2 Second point.
 * @return The Manhattan distance: |x1 - x2| + |y1 - y2|.
 */
int manhattan_dist(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

/**
 * @brief Recursive function with memoization to find the shortest path 
 * from current_loc_index to Home, visiting all unvisited customers.
 *
 * DP State: memo[mask][i] stores the minimum total distance required to 
 * visit all customers NOT yet in 'mask', starting from location 'i' (current_loc_index), 
 * and ending the path at Home.
 *
 * @param N Total number of customers (1 to N).
 * @param D Pre-calculated distance matrix.
 * @param current_loc_index Index of the current location (0=Office, 1..N=Customer).
 * @param mask Bitmask indicating which customers (1..N) have been visited (bit j set means customer j+1 visited).
 * @param memo Memoization table (DP table).
 * @return The minimum distance from current_loc_index to Home, visiting the remaining customers.
 */
int tsp_memo(int N, const vector<vector<int>>& D, 
             int current_loc_index, int mask, 
             vector<vector<int>>& memo) {

    // --- Base Case: All customers visited ---
    // The target mask is where all N bits are set: (1 << N) - 1
    if (mask == (1 << N) - 1) {
        // All customers visited. The remaining path is just the distance to Home (N+1).
        return D[current_loc_index][N + 1];
    }

    // --- Check Memoization Table ---
    if (memo[mask][current_loc_index] != INF) {
        return memo[mask][current_loc_index];
    }

    int min_remaining_distance = INF;

    // --- Recursive Step: Try to visit the next unvisited customer ---
    // Customer indices range from 1 to N
    for (int next_customer_index = 1; next_customer_index <= N; ++next_customer_index) {
        
        // (next_customer_index - 1) is the 0-based bit index for customer 'next_customer_index'
        int bit_index = next_customer_index - 1;

        // Check if the next customer has NOT been visited yet (bit is NOT set)
        if (!(mask & (1 << bit_index))) {
            
            // Calculate the next mask (turn on the bit for the next customer)
            int next_mask = mask | (1 << bit_index);
            
            // Cost of the current leg: from current_loc_index to next_customer_index
            int cost_to_next = D[current_loc_index][next_customer_index];
            
            // Recursive call to find the minimum cost for the remaining path
            int remaining_path_cost = tsp_memo(N, D, next_customer_index, next_mask, memo);
            
            // We must check if the remaining path is reachable before summing to prevent INF+cost_to_next overflow/corruption
            if (remaining_path_cost != INF) {
                int total_branch_cost = cost_to_next + remaining_path_cost;
                min_remaining_distance = min(min_remaining_distance, total_branch_cost);
            }
        }
    }

    // Store the result in the memo table before returning
    return memo[mask][current_loc_index] = min_remaining_distance;
}

/**
 * @brief Solves the Kim's Refrigerator Delivery problem using Recursive Backtracking with Memoization.
 */
void solve_test_case(int t) {
    int N; // Number of customers (5 <= N <= 10)
    if (!(cin >> N)) return;

    // Total number of locations: Office (0), N Customers (1 to N), Home (N+1)
    const int M = N + 2;
    vector<Point> locations(M);

    // Read Office (index 0) and Home (index N+1) coordinates
    // Input order is Office x, y, then Home x, y.
    cin >> locations[0].x >> locations[0].y;
    cin >> locations[N + 1].x >> locations[N + 1].y;

    // Read Customer coordinates (indices 1 to N)
    for (int i = 1; i <= N; ++i) {
        cin >> locations[i].x >> locations[i].y;
    }

    // --- 1. Pre-calculate all inter-location distances ---
    // D[i][j] stores the distance between location i and location j.
    vector<vector<int>> D(M, vector<int>(M));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            D[i][j] = manhattan_dist(locations[i], locations[j]);
        }
    }

    // --- 2. Memoization Table Initialization ---
    // Size: [2^N masks] x [N+1 locations (0=Office, 1..N=Customer)]
    const int num_masks = 1 << N;
    vector<vector<int>> memo(num_masks, vector<int>(N + 1, INF)); 

    // --- 3. Initial Call ---
    // Start the DP from the Office (index 0), with mask 0 (no customers visited).
    // The result is the minimum total cost of the path.
    int min_total_cost = tsp_memo(
        N, D, 
        0,                  // Start at Office (index 0)
        0,                  // Mask 0 (no customers visited)
        memo
    );

    // Output the result
    cout << "#" << t << " " << min_total_cost << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int tCases;
    // Read the total number of test cases
    if (!(cin >> tCases)) return 0;

    for (int t = 1; t <= tCases; ++t) {
        solve_test_case(t);
    }

    return 0;
}
