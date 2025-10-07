#include <bits/stdc++.h>
using namespace std;

int main() {
    // NOTE: For robust code, use std::vector and long long (LL) for values/sum
    int arr_level[100] = {0}; // store levels (int is fine)
    int arr_value[100] = {0}; // store node values (int used as per original)
    int size = 0;             // number of nodes stored

    int x;       // input level
    string str;  // tree string

    if (!(cin >> x >> str)) return 1;

    int level = 0;      // current depth, starts at 0, root value stored at level 1
    int temp = 0;       // current number being read
    bool commit = false; // true if a number is in progress/just finished
    bool neg = false;    // true if number is negative

    for (int i = 0; i < str.length(); i++) {
        char c = str[i];

        if (c == '(') {
            // 1️⃣ Handle open bracket '('
            
            // If a number was just read (e.g., '0(' or '5(')
            if (commit) {
                // Finalize and store the value with the current level
                if (neg) temp *= -1;
                
                arr_level[size] = level; // Store the current level
                arr_value[size] = temp;
                size++;
                
                // Reset for the next node's value
                temp = 0;
                neg = false;
                commit = false;
            }

            level++; // Always go down a level
        }
        else if (c == ')') {
            // 2️⃣ Handle close bracket ')'
            
            // A closing bracket signifies the end of a subtree.
            // If the closing bracket immediately follows a value (which only 
            // happens if the value is part of the original error), it's a structural 
            // issue. In a valid tree string (V(L)(R)), the value V is followed 
            // by '('. We simply move up the tree structure.
            
            // This is the CRITICAL change: ONLY DECREMENT LEVEL.
            // No storage of value should happen here.
            
            // If the node was a null child represented by "()", commit is false.
            if (commit) {
                // This block should ideally not be reached in a correctly structured loop,
                // but we keep it clean.
                if (neg) temp *= -1;
                // Since this is the end of the recursion, the node value must have 
                // been read previously. No storage here.
            }
            
            level--; // Always go up a level
            commit = false;
        }
        else if (c == '-') {
            // 3️⃣ Handle negative sign
            neg = true;
            commit = true;
        }
        else if (isdigit(c)) {
            // 4️⃣ Handle digit
            temp = temp * 10 + (c - '0');
            commit = true;
        }
    }

    // Calculate sum at level x + 1
    int sum = 0;
    int target_level = x + 1;

    for (int i = 0; i < size; i++) {
        if (arr_level[i] == target_level)
            sum += arr_value[i];
    }

    cout << sum << endl;
    return 0;
}
