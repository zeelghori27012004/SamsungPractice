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
