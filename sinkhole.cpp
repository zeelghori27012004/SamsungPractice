#include <iostream>
#include <algorithm>
using namespace std;

const int MAX = 1001;
int A[MAX][MAX];
int dp[MAX][MAX];
int xb, yb, xt, yt;

// Returns min number of ones in any k×k submatrix
int fun(int dp[][MAX], int N, int M, int k) {
    int msum = 2; // At most 1 is allowed
    for (int i = 0; i <= N - k; i++) {
        for (int j = 0; j <= M - k; j++) {
            int sum = dp[i + k][j + k] - dp[i + k][j] - dp[i][j + k] + dp[i][j];
            if (sum <= 1 && sum <= msum) {
                msum = sum;
                xb = i + 1;
                yb = j + 1;
                xt = i + k;
                yt = j + k;
            }
        }
    }
    return msum;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int N, M;
    cin >> N >> M;

    // Initialize matrix
    for (int i = 0; i < N; i++)
        fill(A[i], A[i] + M, 0);

    int K;
    cin >> K;
    for (int i = 0; i < K; i++) {
        int x, y;
        cin >> x >> y;
        A[x - 1][y - 1] = 1; // Convert to 0-based index
    }

    // Build prefix sum dp
    for (int i = 0; i <= N; i++)
        fill(dp[i], dp[i] + M + 1, 0);

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
                       - dp[i - 1][j - 1] + A[i - 1][j - 1];
        }
    }

    // Binary search on square size
    int l = 1, r = min(N, M), ans = 0;
    while (l <= r) {
        int mid = (l + r) / 2;
        int ones = fun(dp, N, M, mid);
        if (ones <= 1) {
            ans = mid; // save best
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    // Output result
    cout << xb << " " << yb << " " << xt << " " << yt << endl;

    return 0;
}










#include <iostream>
#include <algorithm>
#include <tuple>
#include <climits>

using namespace std;

// Constants for matrix size (N, M <= 1000)
const int MAX_SIZE = 1001; 

// Global arrays for grid (A) and prefix sums (dp)
int A[MAX_SIZE][MAX_SIZE];
int dp[MAX_SIZE][MAX_SIZE];

// Global tuple to store the best square found according to priority rules.
// Format: (side_length, sinkhole_count, xb, yb, xt, yt)
// Initialized to an invalid state (side 0, 2 sinkholes)
tuple<int, int, int, int, int, int> best_square = {0, 2, 0, 0, 0, 0};

/**
 * @brief Checks if a square of side length 'k' is feasible (<= 1 sinkhole exists).
 * * This function also updates the global 'best_square' by prioritizing:
 * 1. Larger side length (implicitly handled by Binary Search in main).
 * 2. Fewer sinkholes (0 over 1).
 * 3. Any coordinates if area/sinkholes are equal.
 * * @param N Plot height.
 * @param M Plot width.
 * @param k Side length of the square to check.
 * @return true if at least one k x k square has <= 1 sinkhole, false otherwise.
 */
bool check(int N, int M, int k) {
    if (k == 0) return true;
    
    int min_ones_for_k = 2; // Check if 0 or 1 sinkhole exists for size k

    // Iterate over all possible 0-indexed top-left corners (i, j)
    for (int i = 0; i <= N - k; i++) {
        for (int j = 0; j <= M - k; j++) {
            
            // Calculate sum of sinkholes in the k x k square starting at 0-indexed (i, j)
            // Uses the 1-based DP array:
            int sum = dp[i + k][j + k] - dp[i + k][j] - dp[i][j + k] + dp[i][j];
            
            // Update the minimum sinkholes found for this size k
            min_ones_for_k = min(min_ones_for_k, sum);
            
            // If the current square is a valid candidate (0 or 1 sinkhole)
            if (sum <= 1) {
                // Get the current best stored values
                auto [current_k, current_ones, _, __, ___, ____] = best_square;
                
                // Convert 0-indexed top-left (i, j) to 1-indexed output coordinates
                // (Rows are X, Columns are Y)
                int xb_new = i + k; // Bottom-Left X (Row)
                int yb_new = j + 1; // Bottom-Left Y (Column)
                int xt_new = i + 1; // Top-Right X (Row)
                int yt_new = j + k; // Top-Right Y (Column)
                
                // Priority Check:
                // Since the Binary Search ensures 'k' is the largest known valid size,
                // we only need to prioritize minimum sinkholes for the current 'k'.

                // Is the current side 'k' larger than the best saved side?
                // OR is the current side 'k' equal to the best saved side AND 
                // the current sinkhole count is better (lower)?
                if (k > current_k || (k == current_k && sum < current_ones)) {
                    best_square = {k, sum, xb_new, yb_new, xt_new, yt_new};
                } 
                // If area and sinkholes are equal, we update to any valid square 
                // (as required by "print anyone").
                else if (k == current_k && sum == current_ones) {
                    best_square = {k, sum, xb_new, yb_new, xt_new, yt_new};
                }
            }
        }
    }
    
    return min_ones_for_k <= 1;
}

void solve(int test_case_num) {
    int N, M; // N=Rows(X), M=Columns(Y)
    if (!(cin >> N >> M)) return;

    // Reset A and dp for the new test case (only necessary parts)
    for (int i = 0; i <= N; i++) {
        fill(A[i], A[i] + M, 0);
        fill(dp[i], dp[i] + M + 1, 0);
    }

    int K;
    cin >> K;
    for (int i = 0; i < K; i++) {
        int x, y;
        cin >> x >> y;
        // Convert 1-based input coordinates (x, y) to 0-based grid indices (x-1, y-1)
        A[x - 1][y - 1] = 1; 
    }

    // 1. Build Prefix Sum dp array (1-based logic)
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1] - dp[i - 1][j - 1] + A[i - 1][j - 1];
        }
    }

    // Reset best square for this test case
    best_square = {0, 2, 0, 0, 0, 0}; 

    // 2. Binary Search for the maximum side length L
    int L = 1, R = min(N, M);
    int max_L = 0;
    
    while (L <= R) {
        int mid = L + (R - L) / 2;
        
        // check(mid) determines feasibility AND updates the global best_square if needed
        if (check(N, M, mid)) {
            max_L = mid; // 'mid' is a feasible side length
            L = mid + 1; // Try for a larger square
        } else {
            R = mid - 1; // Square size 'mid' is too large
        }
    }
    
    // 3. Final Coordinate Selection
    // If the loop finished, max_L is the largest side.
    // The check(max_L) was the last time the coordinates were correctly updated for the max size.
    
    if (max_L == 0) {
        // If no square (e.g., N=0, M=0) or if max_L=0, the smallest valid square is 1x1.
        // Since N, M >= 1, a 1x1 square is always possible with <= 1 sinkhole.
        // We ensure a default 1x1 result if no better square was found.
        if (N >= 1 && M >= 1) {
            // Find the best 1x1 square
            check(N, M, 1);
        } else {
            // Default 0s if constraints allow N or M to be 0 (though not explicitly here)
            best_square = {0, 2, 0, 0, 0, 0};
        }
    }

    // Extract the final results
    auto [side, sinkholes, xb_final, yb_final, xt_final, yt_final] = best_square;
    
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
