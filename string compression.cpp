#include <iostream>
#include <cstring> // Included for potential use, though not strictly needed here

using namespace std;

// Function to calculate the length of a C-style string
int lengthOfString(char *str){
    int len = 0, i = 0;
    // Iterate until the null terminator ('\0') is reached
    while(str[i] != '\0'){
        len++;
        i++;
    }
    return len;
}

// Function to parse the string, aggregate counts, and print the results
void mergeAplha(char *str){
    int lenString = lengthOfString(str);
    // Hash array to store counts: index 0 for 'a', 1 for 'b', ..., 25 for 'z'
    int hash[26] = {0}; 

    // Main loop to iterate through the string
    for(int i=0; i<lenString; i++){
        // 1. Read the letter
        char letter = str[i];
        
        // Move to the character immediately following the letter (expected to be a digit)
        i++; 

        // 2. Read the subsequent number (count)
        int temp = 0;
        // Keep reading as long as the character is a digit and within string bounds
        while(i<lenString && str[i] >= '0' && str[i] <= '9'){
            // Convert digit character to integer value and append to temp
            temp = (temp * 10) + (str[i]-'0'); 
            i++;
        }
        
        // CRITICAL: Backtrack 'i' by 1 because the while loop's final 'i++' 
        // advanced the pointer past the number, and the for loop will increment it again.
        i--; 
        
        // 3. Aggregate the count
        // 'letter - 'a'' gives the 0-indexed position (e.g., 'a'->0, 'c'->2)
        hash[letter - 'a'] += temp; 
    }

    // Output the results: iterate through the hash array
    for(int i=0; i<26; i++){
        if(hash[i] != 0){
            // Convert index 'i' back to its corresponding character
            char temp = i + 'a'; 
            // Print the letter and its total accumulated count
            cout << temp << " " << hash[i] << " ";
        }
    }
}

int main() {
    // Declare a character array (C-style string) to hold input
    char s[100]; 
    // Read input (e.g., a2b3a5)
    cin >> s; 
    
    // Process the string
    mergeAplha(s);
    
    // Print a newline for clean output
    cout << endl;
    
    return 0;
}
