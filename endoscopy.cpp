#include <bits/stdc++.h>
using namespace std;
 
struct Node {
    bool left, right, up, down;
};
 
struct Point {
    int x, y;
};
 
int n, m, sX, sY, len;
int arr[1005][1005];
int vis[1005][1005], dis[1005][1005];
int result;
Node pipes[1005][1005];
Point q[1000005]; // queue for BFS
 
// Check if cell is inside grid
bool isValid(int i, int j) {
    return (i >= 0 && i < n && j >= 0 && j < m);
}
 
// BFS to count reachable cells
void bfs() {
    // Reset visited and distance
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            vis[i][j] = 0;
            dis[i][j] = 0;
        }
    }
 
    int front = 0, rear = 0;
 
    // Start position
    dis[sX][sY] = 1;
    vis[sX][sY] = 1;
 
    if (arr[sX][sY] == 0) { // no pipe at start
        result = 0;
        return;
    }
 
    q[rear++] = {sX, sY};
 
    while (front != rear) {
        int p = q[front].x;
        int r = q[front].y;
        front++;
 
        if (1 + dis[p][r] <= len) {
 
            // UP
            if (isValid(p - 1, r) && !vis[p - 1][r] && pipes[p - 1][r].down && pipes[p][r].up) {
                vis[p - 1][r] = 1;
                dis[p - 1][r] = dis[p][r] + 1;
                result++;
                q[rear++] = {p - 1, r};
            }
 
            // DOWN
            if (isValid(p + 1, r) && !vis[p + 1][r] && pipes[p + 1][r].up && pipes[p][r].down) {
                vis[p + 1][r] = 1;
                dis[p + 1][r] = dis[p][r] + 1;
                result++;
                q[rear++] = {p + 1, r};
            }
 
            // LEFT
            if (isValid(p, r - 1) && !vis[p][r - 1] && pipes[p][r - 1].right && pipes[p][r].left) {
                vis[p][r - 1] = 1;
                dis[p][r - 1] = dis[p][r] + 1;
                result++;
                q[rear++] = {p, r - 1};
            }
 
            // RIGHT
            if (isValid(p, r + 1) && !vis[p][r + 1] && pipes[p][r + 1].left && pipes[p][r].right) {
                vis[p][r + 1] = 1;
                dis[p][r + 1] = dis[p][r] + 1;
                result++;
                q[rear++] = {p, r + 1};
            }
        }
    }
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
 
    int t;
    cin >> t;
    while (t--) {
        result = 1;
        cin >> n >> m >> sX >> sY >> len;
 
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> arr[i][j];
 
                // Reset pipe directions
                pipes[i][j] = {false, false, false, false};
 
                if (arr[i][j] == 1) { // all directions
                    pipes[i][j] = {true, true, true, true};
                } 
                else if (arr[i][j] == 2) { // vertical
                    pipes[i][j].up = pipes[i][j].down = true;
                } 
                else if (arr[i][j] == 3) { // horizontal
                    pipes[i][j].left = pipes[i][j].right = true;
                } 
                else if (arr[i][j] == 4) { // up + right
                    pipes[i][j].up = pipes[i][j].right = true;
                } 
                else if (arr[i][j] == 5) { // down + right
                    pipes[i][j].down = pipes[i][j].right = true;
                } 
                else if (arr[i][j] == 6) { // down + left
                    pipes[i][j].down = pipes[i][j].left = true;
                } 
                else if (arr[i][j] == 7) { // up + left
                    pipes[i][j].up = pipes[i][j].left = true;
                }
            }
        }
 
        bfs();
        cout << result << "\n";
    }
    return 0;
}
Language: C++17
