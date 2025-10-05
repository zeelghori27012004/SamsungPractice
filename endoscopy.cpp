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





#include <iostream>
#include <queue>
using namespace std;

struct Node {
    bool left, right, up, down;
};

struct Point {
    int x, y;
};

// Global arrays (acceptable for competitive programming but less safe than vectors)
int n, m, sX, sY, len;
int arr[1005][1005];
int vis[1005][1005], dis[1005][1005];
Node pipes[1005][1005];

bool isValid(int i, int j) {
    return (i >= 0 && i < n && j >= 0 && j < m);
}

int bfs() {
    // 1. Initialization
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            vis[i][j] = 0;
            dis[i][j] = 0;
        }

    if (arr[sX][sY] == 0) return 0;

    queue<Point> q;
    q.push({sX, sY});
    vis[sX][sY] = 1;
    dis[sX][sY] = 1;
    int result = 1;

    // 2. BFS traversal
    while (!q.empty()) {
        Point cur = q.front();
        q.pop();
        int p = cur.x, qy = cur.y;

        // Pruning: Stop exploring from a cell if it's already at the max distance
        if (dis[p][qy] >= len) continue;

        // Directions: dR = {-1, 1, 0, 0}, dC = {0, 0, -1, 1}
        // UP: dR=-1, dC=0
        if (isValid(p - 1, qy) && !vis[p - 1][qy] && pipes[p][qy].up && pipes[p - 1][qy].down) {
            vis[p - 1][qy] = 1;
            dis[p - 1][qy] = dis[p][qy] + 1;
            result++;
            q.push({p - 1, qy});
        }

        // DOWN: dR=1, dC=0
        if (isValid(p + 1, qy) && !vis[p + 1][qy] && pipes[p][qy].down && pipes[p + 1][qy].up) {
            vis[p + 1][qy] = 1;
            dis[p + 1][qy] = dis[p][qy] + 1;
            result++;
            q.push({p + 1, qy});
        }

        // LEFT: dR=0, dC=-1
        if (isValid(p, qy - 1) && !vis[p][qy - 1] && pipes[p][qy].left && pipes[p][qy - 1].right) {
            vis[p][qy - 1] = 1;
            dis[p][qy - 1] = dis[p][qy] + 1;
            result++;
            q.push({p, qy - 1});
        }

        // RIGHT: dR=0, dC=1
        if (isValid(p, qy + 1) && !vis[p][qy + 1] && pipes[p][qy].right && pipes[p][qy + 1].left) {
            vis[p][qy + 1] = 1;
            dis[p][qy + 1] = dis[p][qy] + 1;
            result++;
            q.push({p, qy + 1});
        }
    }

    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        cin >> n >> m >> sX >> sY >> len;
        sX--; sY--; // Convert 1-based to 0-based

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                cin >> arr[i][j];
                // Reset connections (default is all false)
                pipes[i][j] = {false, false, false, false};

                // CRITICAL CORRECTION: Ensure correct mapping based on struct order (left, right, up, down)
                if (arr[i][j] == 1) // All
                    pipes[i][j] = {true, true, true, true};
                else if (arr[i][j] == 2) // Vertical (Up, Down)
                    pipes[i][j] = {false, false, true, true}; // left, right, UP, DOWN
                else if (arr[i][j] == 3) // Horizontal (Left, Right)
                    pipes[i][j] = {true, true, false, false}; // LEFT, RIGHT, up, down
                else if (arr[i][j] == 4) // Up, Right
                    pipes[i][j] = {false, true, true, false}; // left, RIGHT, UP, down
                else if (arr[i][j] == 5) // Down, Right
                    pipes[i][j] = {false, true, false, true}; // left, RIGHT, up, DOWN
                else if (arr[i][j] == 6) // Down, Left
                    pipes[i][j] = {true, false, false, true}; // LEFT, right, up, DOWN
                else if (arr[i][j] == 7) // Up, Left
                    pipes[i][j] = {true, false, true, false}; // LEFT, right, UP, down
            }

        cout << bfs() << "\n";
    }

    return 0;
}
