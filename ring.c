#include <stdio.h>
#include <stdbool.h>

#define N 8  // Change this to desired ring order

// Given an addition table (abelian group structure)
// int addition_table[N][N] = {
//     {0, 1, 2, 3},  // Example: Klein 4-group
//     {1, 0, 3, 2},
//     {2, 3, 0, 1},
//     {3, 2, 1, 0}
// };

// Z8
int addition_table[N][N] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {1, 2, 3, 4, 5, 6, 7, 0},
    {2, 3, 4, 5, 6, 7, 0, 1},
    {3, 4, 5, 6, 7, 0, 1, 2},
    {4, 5, 6, 7, 0, 1, 2, 3},
    {5, 6, 7, 0, 1, 2, 3, 4},
    {6, 7, 0, 1, 2, 3, 4, 5},
    {7, 0, 1, 2, 3, 4, 5 ,6}
};

// Multiplication table to generate
int multiplication_table[N][N];

// Function to check associativity: (a * b) * c == a * (b * c)
bool is_associative() {
    for (int a = 0; a < N; a++) {
        for (int b = 0; b < N; b++) {
            for (int c = 0; c < N; c++) {
                if (multiplication_table[multiplication_table[a][b]][c] !=
                    multiplication_table[a][multiplication_table[b][c]]) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Function to check distributivity
bool is_distributive() {
    for (int a = 0; a < N; a++) {
        for (int b = 0; b < N; b++) {
            for (int c = 0; c < N; c++) {
                int a_plus_b = addition_table[a][b];

                // Left distributivity: (a + b) * c == a * c + b * c
                if (multiplication_table[a_plus_b][c] != 
                    addition_table[multiplication_table[a][c]][multiplication_table[b][c]]) {
                    return false;
                }

                // Right distributivity: c * (a + b) == c * a + c * b
                if (multiplication_table[c][a_plus_b] != 
                    addition_table[multiplication_table[c][a]][multiplication_table[c][b]]) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Function to check for a multiplicative identity
bool has_identity(int *identity) {
    for (int e = 0; e < N; e++) {
        bool is_identity = true;
        for (int a = 0; a < N; a++) {
            if (multiplication_table[a][e] != a || multiplication_table[e][a] != a) {
                is_identity = false;
                break;
            }
        }
        if (is_identity) {
            *identity = e;
            return true;
        }
    }
    return false;
}

// Function to print a multiplication table
void print_table() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", multiplication_table[i][j]);
        }
        printf("\n");
    }
    printf("------------------\n");
}

// Recursive function to generate multiplication tables and check ring properties
void generate_tables(int row, int col) {
    if (row == N) {
        // Table is fully filled; check ring properties
        if (is_associative() && is_distributive()) {
            int identity;
            if (has_identity(&identity)) {  // If you require an identity
                printf("Valid Ring with Identity %d:\n", identity);
                print_table();
            }
        }
        return;
    }

    // Next cell to fill
    int next_row = (col == N - 1) ? row + 1 : row;
    int next_col = (col + 1) % N;

    // Try all possible values for multiplication_table[row][col]
    for (int value = 0; value < N; value++) {
        multiplication_table[row][col] = value;
        generate_tables(next_row, next_col);
    }
}

// Main function
int main() {
    printf("Generating all possible rings of order %d...\n", N);
    generate_tables(0, 0);
    return 0;
}
