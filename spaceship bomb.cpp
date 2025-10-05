#include <bits/stdc++.h>
using namespace std;

int R;
vector<string> grid;
vector<vector<vector<int>>> dp;

int dfs(int r, int c, int bombLeft) {
    // Out of bounds
    if (r < 0 || c < 0 || c >= 5)
        return 0;

    // Hit enemy
    if (grid[r][c] == '2')
        return -1000000;

    // Memoized
    if (dp[r][c][bombLeft] != -1)
        return dp[r][c][bombLeft];

    int coin = (grid[r][c] == '1') ? 1 : 0;
    int best = -1000000;

    // 1️⃣ Normal movement
    for (int dc : {-1, 0, 1})
        best = max(best, dfs(r - 1, c + dc, bombLeft));

    // 2️⃣ Use bomb (only if available)
    if (bombLeft == 1) {
        vector<string> backup = grid;
        // Detonate clears enemies in 5×5 region above current control zone
        for (int i = r; i >= max(0, r - 4); --i)
            for (int j = 0; j < 5; ++j)
                if (grid[i][j] == '2')
                    grid[i][j] = '0';

        // After bomb → continue with 0 bombs
        for (int dc : {-1, 0, 1})
            best = max(best, dfs(r - 1, c + dc, 0));

        // Restore grid
        grid = backup;
    }

    if (best > -1000000)
        best += coin;

    return dp[r][c][bombLeft] = best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (int tc = 1; tc <= T; ++tc) {
        cin >> R;
        grid.assign(R, "");
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < 5; ++j) {
                char x;
                cin >> x;
                grid[i] += x;
            }

        dp.assign(R, vector<vector<int>>(5, vector<int>(2, -1)));

        int ans = -1000000;
        // Start from row R-1 (bottom), center col=2, bomb=1
        for (int c : {1, 2, 3})
            ans = max(ans, dfs(R - 1, c, 1));

        cout << "#" << tc << " : " << max(0, ans) << "\n";
    }
}
