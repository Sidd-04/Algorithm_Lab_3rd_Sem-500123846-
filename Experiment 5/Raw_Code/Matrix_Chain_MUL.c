#include <stdio.h>
#include <limits.h>

void print_optimal_parens(int , int , int n, int s[n][n], char *);
void matrix_chain_order(int p[], int );


// Function to print the optimal parenthesization
void print_optimal_parens(int i, int j, int n, int s[n][n], char *name) {
    if (i == j) {
        printf("A%c", *name);  // Print matrix name (e.g., A1, A2, etc.)
        (*name)++;
        return;
    }
    printf("(");
    print_optimal_parens(i, s[i][j], n, s, name);
    print_optimal_parens(s[i][j] + 1, j, n, s, name);
    printf(")");
}

// Function to find the minimum cost of matrix chain multiplication
void matrix_chain_order(int p[], int n) {
    int m[n][n];  // Table to store minimum multiplications
    int s[n][n];  // Table to store split points

    // Initialize number of multiplications for a single matrix as 0
    for (int i = 1; i < n; i++)
        m[i][i] = 0;

    // L is the chain length
    for (int L = 2; L < n; L++) {
        for (int i = 1; i < n - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = INT_MAX;  // Initialize with a large value

            // Test all positions to split the product
            for (int k = i; k <= j - 1; k++) {
                // Calculate cost of scalar multiplications
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];

                // Update minimum cost and store split point
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    // Output the minimum number of scalar multiplications
    printf("Minimum number of multiplications is: %d\n", m[1][n - 1]);

    // Output the optimal parenthesization
    printf("Optimal parenthesization: ");
    char name = '1';  // Start naming matrices as A1, A2, ...
    print_optimal_parens(1, n - 1, n, s, &name);
    printf("\n");
}

// Main function
int main() {
    // Matrix dimensions: A1(30x35), A2(35x15), A3(15x5), A4(5x10), A5(10x20), A6(20x25)
    int p[] = {30, 35, 15, 5, 10, 20, 25};  // Array of matrix dimensions
    int n = sizeof(p) / sizeof(p[0]);  // Number of matrices is n-1

    // Call the function to calculate the minimum multiplications and print the result
    matrix_chain_order(p, n);

    return 0;
}
