#include <bits/stdc++.h>
using namespace std;

int n;
vector<string> arr;
// pair<int, int> stores {max_sum, path_count}
vector<vector<pair<int,int>>> dp;

// Define the "No Path" sentinel: Sum = -1 (impossible for valid paths, as digits are 0-9), Count = 0
const pair<int, int> NO_PATH = {-1, 0};

// Recursive function to find the max sum and path count from cell (i, j)
pair<int,int> solve(int i, int j) {
    // --- 1. Base Cases / Sentinel Checks ---
    
    // Out of bounds or obstacle ('x'). Corrected return value.
    if (i >= n || j >= n || arr[i][j] == 'x')  
        return NO_PATH;

    // Base case: bottom-right cell (destination)
    if (i == n-1 && j == n-1)  
        return {arr[i][j]-'0', 1}; 

    // Check memoization table: count = -1 indicates unvisited
    if (dp[i][j].second != -1)  
        return dp[i][j];

    // --- 2. Recursive Step and Move Evaluation ---
    
    int val = arr[i][j]-'0'; // Current cell's value
    int maxSum = -1;         // Initialize maxSum to -1 (the NO_PATH sum)
    int pathCount = 0;

    // --- Move Right ---
    auto right = solve(i, j+1);
    if (right.second > 0) {
        if (right.first > maxSum) {
            maxSum = right.first;
            pathCount = right.second;
        } else if (right.first == maxSum) {
            pathCount += right.second;
        }
    }

    // --- Move Down ---
    auto down = solve(i+1, j);
    if (down.second > 0) {
        if (down.first > maxSum) {
            maxSum = down.first;
            pathCount = down.second;
        } else if (down.first == maxSum) {
            pathCount += down.second;
        }
    }

    // --- Move Diagonal ---
    auto diag = solve(i+1, j+1);
    if (diag.second > 0) {
        if (diag.first > maxSum) {
            maxSum = diag.first;
            pathCount = diag.second;
        } else if (diag.first == maxSum) {
            pathCount += diag.second;
        }
    }

    // --- 3. Memoization and Return (Corrected Logic) ---
    
    // If maxSum is still -1, it means all moves led to NO_PATH.
    if (maxSum == -1) {
        dp[i][j] = NO_PATH;
    } else {
        // A valid path exists (maxSum >= 0). 
        // Current cell's max sum is its value + max sum from the best next step.
        dp[i][j] = {val + maxSum, pathCount};
    }

    return dp[i][j];
}

int main() {
    // Optimization for faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    while (t--) {
        cin >> n;
        
        // Read the grid
        arr.resize(n);
        for (int i = 0; i < n; i++) cin >> arr[i];

        // Ensure start and end are not obstacles (standard competitive programming assumption)
        if (n > 0) {
            if (arr[0][0] == 'x') arr[0][0] = '0';
            if (arr[n-1][n-1] == 'x') arr[n-1][n-1] = '0';
        }
        
        // Initialize DP table: Count = -1 acts as "unvisited".
        dp.assign(n, vector<pair<int,int>>(n, {-1, -1}));

        auto ans = solve(0, 0);

        // Final check: If pathCount is 0 (i.e., NO_PATH was returned), output "0 0"
        if (ans.second == 0) {
            cout << "0 0" << "\n";
        } else {
            cout << ans.first << " " << ans.second << "\n";
        }
    }
    return 0;
}
