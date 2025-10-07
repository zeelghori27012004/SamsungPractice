#include <bits/stdc++.h>
using namespace std;

int main() {
    int x;          // Target depth
    string str;     // Tree string

    cin >> x >> str;

    vector<int> levels; // Node levels
    vector<int> values; // Node values

    int level = 0;      // Current depth
    int temp = 0;       // Current number being read
    bool commit = false; // True if number exists in temp
    bool neg = false;    // True if number is negative

    for (char c : str) {
        if (c == '(') {
            // Open bracket: store number if any
            if (commit) {
                if (neg) temp *= -1;
                levels.push_back(level);
                values.push_back(temp);
                temp = 0; neg = false; commit = false;
            }
            level++; // go down
        }
        else if (c == ')') {
            // Close bracket: store number if any (handles single-node like (6))
            if (commit) {
                if (neg) temp *= -1;
                levels.push_back(level);
                values.push_back(temp);
                temp = 0; neg = false; commit = false;
            }
            level--; // go up
        }
        else if (c == '-') {
            neg = true;
            commit = true;
        }
        else if (isdigit(c)) {
            temp = temp * 10 + (c - '0');
            commit = true;
        }
    }

    // Compute sum at level x+1
    long long sum = 0;
    int target_level = x + 1;
    for (size_t i = 0; i < levels.size(); i++) {
        if (levels[i] == target_level)
            sum += values[i];
    }

    cout << sum << endl;
    return 0;
}
