#include <stdio.h>
#include <stdlib.h>

// Item structure to hold value, weight, and value-to-weight ratio for the greedy approach
typedef struct {
    int value;
    int weight;
    double ratio;
} Item;

// Comparison function for sorting items by value-to-weight ratio in descending order
int compare(const void *a, const void *b) {
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;
    if (item2->ratio > item1->ratio) return 1;
    else if (item2->ratio < item1->ratio) return -1;
    return 0;
}

// Greedy approach function
int knapsackGreedy(int weights[], int values[], int n, int capacity) {
    // Create items array and calculate value-to-weight ratio for each item
    Item items[n];
    for (int i = 0; i < n; i++) {
        items[i].value = values[i];
        items[i].weight = weights[i];
        items[i].ratio = (double)values[i] / weights[i];
    }

    // Sort items by their value-to-weight ratio in descending order
    qsort(items, n, sizeof(Item), compare);

    int totalValue = 0;
    int remainingCapacity = capacity;

    // Add items to knapsack based on sorted ratio until capacity is reached
    for (int i = 0; i < n && remainingCapacity > 0; i++) {
        if (items[i].weight <= remainingCapacity) {
            totalValue += items[i].value;
            remainingCapacity -= items[i].weight;
        }
    }

    return totalValue;
}

// Dynamic Programming approach function
int knapsackDP(int weights[], int values[], int n, int capacity) {
    // Create DP table
    int dp[n + 1][capacity + 1];

    // Initialize the table
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (weights[i - 1] <= w)
                dp[i][w] = (dp[i - 1][w] > (dp[i - 1][w - weights[i - 1]] + values[i - 1])) ?
                            dp[i - 1][w] : (dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    // The maximum value that can be achieved with the given capacity
    return dp[n][capacity];
}

int main() {
    // Define dataset
    int weights[] = {1, 2, 3, 8, 7, 4};
    int values[] = {20, 5, 10, 40, 15, 25};
    int capacity = 10;
    int n = sizeof(weights) / sizeof(weights[0]);
    int greedyResult = knapsackGreedy(weights, values, n, capacity);
    int dpResult = knapsackDP(weights, values, n, capacity);

    printf("Greedy approach result: %d\n", greedyResult);
    printf("Dynamic Programming approach result: %d\n", dpResult);

    return 0;
}
