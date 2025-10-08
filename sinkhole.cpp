#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple> // For returning the best square info
#include <climits> // Use standard INT_MAX

using namespace std;

// Global variables to store the coordinates of the best square found
// (side, sinkholes, xb, yb, xt, yt)
tuple<int, int, int, int, int, int> best_square = {0, 2, 0, 0, 0, 0};

/**
 * @brief Checks if a square of side length 'k' is feasible (i.e., at least one 
 * k x k square has <= 1 sinkhole).
 * Also updates the global 'best_square' if a better or equal square of size 'k' is found.
 * * @param dp The 2D prefix sum array.
 * @param N Plot height.
 * @param M Plot width.
 * @param k Side length of the square to check.
 * @return true if a k x k square with <= 1 sinkhole exists, false otherwise.
 */
bool check(const vector<vector<int>>& dp, int N, int M, int k){
    if (k == 0) return true;
    
    // Minimum number of sinkholes found for a k x k square
    int min_ones = INT_MAX; 
    
    // Iterate over all possible top-left corners (r, c) in 0-based indexing
    for(int r = 0; r <= N-k; r++){
        for(int c = 0; c <= M-k; c++){
            
            // Calculate sum of sinkholes in the k x k square starting at (r, c)
            // Top-Left corner of square (0-indexed A-array): (r, c)
            // Bottom-Right corner of square (0-indexed A-array): (r+k-1, c+k-1)
            // DP indices are 1-based size indices, so (r+k, c+k) is the bottom-right corner of the DP rectangle
            int sum = dp[r+k][c+k] - dp[r][c+k] - dp[r+k][c] + dp[r][c];
            
            min_ones = min(min_ones, sum);

            // If a feasible square is found (0 or 1 sinkhole)
            if (sum <= 1) {
                // Convert 0-indexed corners (r, c) to 1-indexed output coordinates
                int xb_new = r + k;     // Bottom-Left X (Row is X)
                int yb_new = c + 1;     // Bottom-Left Y (Col is Y)
                int xt_new = r + 1;     // Top-Right X
                int yt_new = c + k;     // Top-Right Y
                
                // Update best_square based on priority:
                // 1. Maximize side length (k)
                // 2. Minimize sinkholes (sum)
                
                // Get current best side and sinkholes
                auto [current_k, current_ones, _, __, ___, ____] = best_square;

                // Check for a better square
                if (k > current_k || (k == current_k && sum < current_ones)) {
                    best_square = {k, sum, xb_new, yb_new, xt_new, yt_new};
                }
                // If area and sinkholes are equal, just update (as required by "print anyone")
                else if (k == current_k && sum == current_ones) {
                    best_square = {k, sum, xb_new, yb_new, xt_new, yt_new};
                }
            }
        }
    }
    
    return min_ones <= 1;
}

void solve(int test_case_num) {
    int N, M; // N=Rows (X-coordinate), M=Columns (Y-coordinate)
    if (!(cin >> N >> M)) return;

    // 0-indexed grid for sinkhole presence
    vector<vector<int>> A(N, vector<int>(M, 0)); 
    
    int K;
    cin >> K;
    for(int i=0; i<K; i++){
        int x, y;
        cin >> x >> y;
        // Convert 1-based input coordinates (x, y) to 0-based grid indices (x-1, y-1)
        A[x-1][y-1] = 1; 
    }
    
    // 2D Prefix Sum array (1-indexed for easy sum calculation)
    vector<vector<int>> dp(N+1, vector<int>(M+1, 0));
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            dp[i][j] = dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1] + A[i-1][j-1];
        }
    }

    // Reset best square for this test case
    best_square = {0, 2, 0, 0, 0, 0}; // side=0, sinkholes=2 (invalid), coords=0

    // Binary Search for the maximum side length L
    int L = 1, R = min(N, M);
    int max_L = 0;
    
    while(L <= R){
        int mid = L + (R-L)/2;
        
        // check(mid) also updates best_square if a size 'mid' square is better than current
        if(check(dp, N, M, mid)){
            max_L = mid; // 'mid' is a possible answer
            L = mid + 1; // Try for an even larger square
        } else {
            R = mid - 1; // Square size 'mid' is too large
        }
    }

    // After BS, max_L holds the maximum possible side length.
    // The 'best_square' variables should already hold the best square found during the BS process.
    if (max_L > 0) {
        // Find the absolute best square of size max_L, prioritizing 0 sinkholes
        // We call check(max_L) one more time to ensure the coordinates for the optimal
        // square (0 sinkholes) of size max_L are correctly saved, in case the BS logic
        // skipped over the coordinate update during a non-maximal L update.
        check(dp, N, M, max_L); 
    }

    // Extract the final results
    auto [side, sinkholes, xb_final, yb_final, xt_final, yt_final] = best_square;
    
    // Edge case: if max_L is 0 (e.g., N=0 or M=0), output should handle it, though N,M >= 1
    if (max_L == 0 && N >= 1 && M >= 1) {
        // The smallest possible square is 1x1. Since K <= N+M, 1x1 with <= 1 sinkhole is guaranteed.
        // We set the result to the 1x1 square at (1, 1).
        xb_final = 1; yb_final = 1; xt_final = 1; yt_final = 1;
    } else if (max_L == 0) {
        // If N=0 or M=0, print default 0s (although constraints say N, M >= 1)
        xb_final = 0; yb_final = 0; xt_final = 0; yt_final = 0;
    }


    // Output the result in the required format
    cout << "#" << test_case_num << " " 
         << xb_final << " " << yb_final << " " 
         << xt_final << " " << yt_final << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T; // Number of test cases
    if (!(cin >> T)) return 0;
    
    for(int i=1; i<=T; i++){
        solve(i);
    }
    
    return 0;
}
