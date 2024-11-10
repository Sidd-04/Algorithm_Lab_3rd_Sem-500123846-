#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

// Global variables
int n, capacity;
int weights[100], values[100];

// Backtracking Approach
int knapsackBacktrack(int i, int remainingCapacity) {
    if (i == n || remainingCapacity == 0) 
        return 0;
    
    // If current item exceeds remaining capacity, skip it
    if (weights[i] > remainingCapacity) 
        return knapsackBacktrack(i + 1, remainingCapacity);
    
    // Return maximum of including or excluding the current item
    int include = values[i] + knapsackBacktrack(i + 1, remainingCapacity - weights[i]);
    int exclude = knapsackBacktrack(i + 1, remainingCapacity);
    return max(include, exclude);
}

// Branch & Bound Node Structure
typedef struct {
    int level;
    int profit;
    int weight;
    float bound;
} Node;

// Bound Calculation for Branch & Bound
float bound(Node u) {
    if (u.weight >= capacity) 
        return 0;

    float result = (float)u.profit;
    int j = u.level + 1;
    int totalWeight = u.weight;

    while (j < n && totalWeight + weights[j] <= capacity) {
        totalWeight += weights[j];
        result += values[j];
        j++;
    }

    if (j < n) 
        result += (capacity - totalWeight) * (float)values[j] / weights[j];
    
    return result;
}

// Branch & Bound Approach
int knapsackBranchAndBound() {
    Node u, v;
    u.level = -1;
    u.profit = 0;
    u.weight = 0;
    u.bound = bound(u);
    
    int maxProfit = 0;

    Node queue[100];
    int front = 0, rear = 0;

    queue[rear++] = u;

    while (front < rear) {
        u = queue[front++];
        
        if (u.level == n - 1) 
            continue;

        v.level = u.level + 1;
        v.weight = u.weight + weights[v.level];
        v.profit = u.profit + values[v.level];

        if (v.weight <= capacity && v.profit > maxProfit)
            maxProfit = v.profit;

        v.bound = bound(v);

        if (v.bound > maxProfit)
            queue[rear++] = v;

        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v);

        if (v.bound > maxProfit)
            queue[rear++] = v;
    }

    return maxProfit;
}

// Dynamic Programming Approach
int knapsackDP() {
    int dp[capacity + 1];
    for (int i = 0; i <= capacity; i++)
        dp[i] = 0;

    for (int i = 0; i < n; i++) {
        for (int w = capacity; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[capacity];
}

int main() {
    n = 4;
    int weightsTemp[] = {2, 3, 4, 5};
    int valuesTemp[] = {3, 4, 5, 6};
    capacity = 5;

    for (int i = 0; i < n; i++) {
        weights[i] = weightsTemp[i];
        values[i] = valuesTemp[i];
    }

    clock_t start, end;

    // Run and time Backtracking approach
    start = clock();
    int backtrackResult = knapsackBacktrack(0, capacity);
    end = clock();
    double timeBacktrack = (double)(end - start) / CLOCKS_PER_SEC;

    // Run and time Branch & Bound approach
    start = clock();
    int branchBoundResult = knapsackBranchAndBound();
    end = clock();
    double timeBranchBound = (double)(end - start) / CLOCKS_PER_SEC;

    // Run and time Dynamic Programming approach
    start = clock();
    int dpResult = knapsackDP();
    end = clock();
    double timeDP = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Backtracking Result: %d, Time: %f seconds\n", backtrackResult, timeBacktrack);
    printf("Branch & Bound Result: %d, Time: %f seconds\n", branchBoundResult, timeBranchBound);
    printf("Dynamic Programming Result: %d, Time: %f seconds\n", dpResult, timeDP);

    // Save times to file for graphing in Python
    FILE *fptr = fopen("knapsack_times.txt", "w");
    fprintf(fptr, "Backtracking %f\n", timeBacktrack);
    fprintf(fptr, "Branch & Bound %f\n", timeBranchBound);
    fprintf(fptr, "Dynamic Programming %f\n", timeDP);
    fclose(fptr);

    return 0;
}
