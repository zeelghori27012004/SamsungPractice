#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int solve_bitmask(int R, int C, int K, const vector<vector<int>>& grid) {
    unordered_map<int, int> freq;
    int max_rows = 0;

    for (int i = 0; i < R; ++i) {
        int mask = 0;
        int zero_count = 0;

        for (int j = 0; j < C; ++j) {
            if (grid[i][j] == 1) {
                mask |= (1 << j);
            } else {
                zero_count++;
            }
        }

        if (zero_count <= K && (K - zero_count) % 2 == 0) {
            freq[mask]++;
            max_rows = max(max_rows, freq[mask]);
        }
    }

    return max_rows;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;

    for (int testcase = 1; testcase <= t; ++testcase) {
        int R, C, K;
        cin >> R >> C >> K;

        vector<vector<int>> grid(R, vector<int>(C));
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j)
                cin >> grid[i][j];

        int result = solve_bitmask(R, C, K, grid);
        cout << "#" << testcase << " " << result << "\n";
    }

    return 0;
}
