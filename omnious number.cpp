#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

// Function to count the number of valid product numbers in the range [a, b]
int numberOminous(int a, int b, int k, const vector<int>& delNos) {
    int count = 0; // Initialize the count of valid numbers
    int n = delNos.size(); // Number of forbidden digits

    // 1. Iterate through every number i in the range [a, b]
    for (int i = a; i <= b; i++) {
        int temp = i;
        // Array to store the count of each digit (0 to 9) in the current number i
        int digitArray[10] = {0}; 

        // 2. Count the occurrences of each digit in the current number
        if (temp == 0) {
            digitArray[0]++;
        } else {
            while (temp) {
                digitArray[temp % 10]++;
                temp /= 10;
            }
        }
        
        // 3. Calculate the total count of forbidden digits (rougeK)
        int rougeK = 0;
        // Iterate through the list of forbidden digits (delNos)
        for (int j = 0; j < n; j++) {
            // Sum the counts of all forbidden digits
            rougeK += digitArray[delNos[j]];
        }

        // 4. Check Validity: A number is valid if the total forbidden count is < k
        if (rougeK < k) {
            count++; // Increment count if the number is valid
        }
    }
    return count;
}

int main() {
    int a, b, k;
    
    // Read range [a, b] and the threshold k
    if (!(cin >> a >> b >> k)) return 1;

    int n;
    // Read the number of forbidden digits
    if (!(cin >> n)) return 1; 

    // Use a vector instead of a raw pointer/array for better memory management
    vector<int> delNos(n); 
    
    // Read the forbidden digits
    for (int i = 0; i < n; i++) {
        if (!(cin >> delNos[i])) return 1;
    }

    // Output the final count
    cout << numberOminous(a, b, k, delNos) << "\n";

    return 0;
}






#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <set>

using namespace std;

// Maximum number of digits (for a number up to 10^18)
const int MAX_DIGITS = 20; 
// DP table: dp[index][count][is_leading_zero]
// Note: 'tight' state is not memoized, only non-tight states.
long long dp[MAX_DIGITS][MAX_DIGITS][2]; 
string num_str; // String representation of the upper limit X
set<int> forbidden_digits; // Set for O(1) lookup of forbidden digits
int K_limit; // The threshold K

// Helper function to convert the list of forbidden digits to a set
void setup_forbidden_digits(const vector<int>& delNos) {
    forbidden_digits.clear();
    for (int d : delNos) {
        forbidden_digits.insert(d);
    }
}

// Check if a digit is forbidden
bool is_forbidden(int digit) {
    return forbidden_digits.count(digit);
}

// Digit DP function: Counts valid numbers <= X
// idx: current digit position (from left, 0-indexed)
// count: total count of forbidden digits used so far
// tight: true if we are restricted by the digits of X
// is_leading_zero: true if all digits placed so far are 0
long long solve_dp(int idx, int count, bool tight, bool is_leading_zero) {
    // Base case: Finished processing all digits
    if (idx == num_str.length()) {
        // If we processed an actual number (not all zeros), it is a valid number.
        // The check (count < K_limit) is implicitly handled during the recursion.
        // We return 1 for a valid number formed.
        return (count < K_limit) ? 1 : 0;
    }

    // Memoization: Only for non-tight states
    // The tight state needs to be recomputed as it depends on the actual digits of X
    if (!tight && !is_leading_zero && dp[idx][count][0] != -1) {
        return dp[idx][count][0];
    }
    if (!tight && is_leading_zero && dp[idx][count][1] != -1) {
        return dp[idx][count][1];
    }

    long long result = 0;
    // Determine the upper limit for the current digit
    int limit = tight ? (num_str[idx] - '0') : 9;

    for (int dig = 0; dig <= limit; ++dig) {
        
        bool new_tight = tight && (dig == limit);
        bool new_is_leading_zero = is_leading_zero && (dig == 0);
        
        int new_count = count;
        
        // Count the forbidden digit only if it's NOT a leading zero
        if (!new_is_leading_zero && is_forbidden(dig)) {
            new_count++;
        }
        
        // Pruning: If the count exceeds the limit K, this path is invalid
        if (new_count >= K_limit) {
            continue; 
        }

        // Recursive call
        result += solve_dp(idx + 1, new_count, new_tight, new_is_leading_zero);
    }

    // Memoization update: Only for non-tight states
    if (!tight && !is_leading_zero) {
        dp[idx][count][0] = result;
    }
    if (!tight && is_leading_zero) {
        dp[idx][count][1] = result;
    }

    return result;
}

// Function to calculate the count of valid numbers in [0, X]
long long ValidCount(long long X) {
    if (X < 0) return 0;
    
    num_str = to_string(X);
    
    // Initialize DP table with -1
    memset(dp, -1, sizeof(dp)); 
    
    // Start DP from the first digit (index 0), count 0, tight=true (restricted by X), 
    // and is_leading_zero=true (haven't placed a non-zero digit yet)
    return solve_dp(0, 0, true, true);
}


// Main function: Calculates the count in the range [a, b]
long long numberOminous_DP(long long a, long long b, int k, const vector<int>& delNos) {
    setup_forbidden_digits(delNos);
    K_limit = k;

    // Calculate ValidCount(B) - ValidCount(A-1)
    long long count_b = ValidCount(b);
    long long count_a_minus_1 = ValidCount(a - 1);
    
    return count_b - count_a_minus_1;
}

int main() {
    // Note: Using long long for a and b to handle larger ranges
    long long a, b;
    int k;
    
    // Read range [a, b] and the threshold k
    if (!(cin >> a >> b >> k)) return 1;

    int n;
    // Read the number of forbidden digits
    if (!(cin >> n)) return 1; 

    vector<int> delNos(n); 
    
    // Read the forbidden digits
    for (int i = 0; i < n; i++) {
        if (!(cin >> delNos[i])) return 1;
    }

    // Output the final count using the Digit DP approach
    cout << numberOminous_DP(a, b, k, delNos) << "\n";

    return 0;
}
