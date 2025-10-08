#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// Recursive function to explore all possible partitions
// 'idx': The index of the element currently being considered
// 'p1', 'p2': The current heights of the two pillars
// 'a': The array of available pillar heights
// 'ans': A reference to the maximum equal height found so far
void solve_corrected_recursive(int idx, int p1, int p2, const vector<int>& a, int& ans) {
    int n = a.size();
    
    // ✅ Base Case: All elements have been considered
    if (idx == n) {
        if (p1 == p2) {
            // Found a complete partition where pillars are equal
            ans = max(ans, p1);
        }
        return;
    }
    
    int current_height = a[idx];

    // 1. Option: Add a[idx] to Pillar 1
    solve_corrected_recursive(idx + 1, p1 + current_height, p2, a, ans);

    // 2. Option: Add a[idx] to Pillar 2
    solve_corrected_recursive(idx + 1, p1, p2 + current_height, a, ans);
    
    // 3. Option: Skip a[idx] (Unused)
    solve_corrected_recursive(idx + 1, p1, p2, a, ans);
}

void run_recursive() {
    int t;
    if (!(cin >> t)) return; // Read T: number of test cases

    while (t--) {
        int n;
        if (!(cin >> n)) return; // Read n: number of different pillars

        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            if (!(cin >> a[i])) return; // Read available heights
        }
        
        int ans = 0; // Initialize max equal height to 0
        
        solve_corrected_recursive(0, 0, 0, a, ans);
        
        cout << ans << endl;
    }
}

// NOTE: The main provided in the question was simpler, so I'll structure the final output
// to fit a single test case logic as requested, but a full competitive programming solution 
// requires handling 'T' test cases.
/*
int main() {
    // If you were to submit a full solution:
    // run_recursive();
    // return 0;
}
*/












#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Function to find the maximum height of two equal pillars
int maxEqualPillars(const vector<int>& a) {
    int n = a.size();
    int total_sum = 0;
    for (int h : a) total_sum += h;

    // dp[d] = maximum height of the smaller pillar when difference = d
    vector<int> dp(total_sum + 1, -1);
    dp[0] = 0; // Base case: difference 0 with smaller pillar 0

    for (int h : a) {
        vector<int> dp_new = dp; // Temporary copy to avoid reusing the same 'h'

        for (int d = 0; d <= total_sum; ++d) {
            if (dp[d] == -1) continue; // skip unreachable states

            int shorter = dp[d];
            int taller = shorter + d;

            // 1. Add 'h' to the shorter pillar
            int new_shorter1, new_diff1;
            int p2_new = shorter + h;
            if (p2_new <= taller) {
                new_shorter1 = p2_new;
                new_diff1 = taller - p2_new;
            } else {
                new_shorter1 = taller;
                new_diff1 = p2_new - taller;
            }
            dp_new[new_diff1] = max(dp_new[new_diff1], new_shorter1);

            // 2. Add 'h' to the taller pillar
            int new_diff2 = taller + h - shorter;
            if (new_diff2 <= total_sum) {
                dp_new[new_diff2] = max(dp_new[new_diff2], shorter);
            }
        }
        dp = move(dp_new); // update dp after processing this height
    }

    return dp[0]; // maximum height of equal pillars
}

int main() {
    int t;
    cin >> t; // number of test cases

    while (t--) {
        int n;
        cin >> n; // number of available pillar heights
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        cout << maxEqualPillars(a) << "\n";
    }

    return 0;
}
