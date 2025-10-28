#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>

using namespace std;

struct Gate {
    int id;
    int loc;
    int num;
};

int N;
Gate initial_gates[3];
Gate perm_gates[3];
int min_total_distance;

void solve(int k, int current_dist, bool occupied[]);
void place_fishermen_for_gate(int k, int current_dist, bool occupied[], int gate_loc, int remaining);

void solve(int k, int current_dist, bool occupied[]) {
    // All 3 gates processed
    if (k == 3) {
        min_total_distance = min(min_total_distance, current_dist);
        return;
    }

    // Prune unnecessary branches
    if (current_dist >= min_total_distance)
        return;

    // Process fishermen for current gate
    place_fishermen_for_gate(k, current_dist, occupied, perm_gates[k].loc, perm_gates[k].num);
}

void place_fishermen_for_gate(int k, int current_dist, bool occupied[], int gate_loc, int remaining) {
    if (remaining == 0) {
        // Move to next gate once all fishermen placed
        solve(k + 1, current_dist, occupied);
        return;
    }

    if (current_dist >= min_total_distance)
        return;

    int d = 0;
    while (true) {
        int left_spot = gate_loc - d;
        int right_spot = gate_loc + d;

        bool left_valid = (left_spot >= 1 && left_spot <= N);
        bool right_valid = (right_spot >= 1 && right_spot <= N);

        bool left_available = left_valid && !occupied[left_spot];
        bool right_available = right_valid && !occupied[right_spot];

        int dist_to_add = d + 1;

        if (left_available && right_available) {
            if (remaining == 1) {
                // Last fisherman → branch both choices
                bool occupied_A[N + 1];
                memcpy(occupied_A, occupied, sizeof(bool) * (N + 1));
                occupied_A[left_spot] = true;
                place_fishermen_for_gate(k, current_dist + dist_to_add, occupied_A, gate_loc, remaining - 1);

                bool occupied_B[N + 1];
                memcpy(occupied_B, occupied, sizeof(bool) * (N + 1));
                occupied_B[right_spot] = true;
                place_fishermen_for_gate(k, current_dist + dist_to_add, occupied_B, gate_loc, remaining - 1);
            } else {
                // Not last fisherman → choose one side arbitrarily (left)
                bool new_occupied[N + 1];
                memcpy(new_occupied, occupied, sizeof(bool) * (N + 1));
                new_occupied[left_spot] = true;
                place_fishermen_for_gate(k, current_dist + dist_to_add, new_occupied, gate_loc, remaining - 1);
            }
            break; // FIX: was 'return;' before
        } 
        else if (left_available) {
            bool new_occupied[N + 1];
            memcpy(new_occupied, occupied, sizeof(bool) * (N + 1));
            new_occupied[left_spot] = true;
            place_fishermen_for_gate(k, current_dist + dist_to_add, new_occupied, gate_loc, remaining - 1);
            break; // FIX: was 'return;'
        } 
        else if (right_available) {
            bool new_occupied[N + 1];
            memcpy(new_occupied, occupied, sizeof(bool) * (N + 1));
            new_occupied[right_spot] = true;
            place_fishermen_for_gate(k, current_dist + dist_to_add, new_occupied, gate_loc, remaining - 1);
            break; // FIX: was 'return;'
        }

        d++; // increase distance until we find a free spot
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    for (int t = 1; t <= T; ++t) {
        cin >> N;
        for (int i = 0; i < 3; ++i) {
            initial_gates[i].id = i;
            cin >> initial_gates[i].loc >> initial_gates[i].num;
        }

        int p[] = {0, 1, 2};
        min_total_distance = INT_MAX;

        // Try all permutations of gate entry orders
        do {
            for (int i = 0; i < 3; ++i)
                perm_gates[i] = initial_gates[p[i]];

            bool occupied[N + 1];
            memset(occupied, false, sizeof(occupied));

            solve(0, 0, occupied);
        } while (next_permutation(p, p + 3));

        cout << "#" << t << " " << min_total_distance << "\n";
    }

    return 0;
}
