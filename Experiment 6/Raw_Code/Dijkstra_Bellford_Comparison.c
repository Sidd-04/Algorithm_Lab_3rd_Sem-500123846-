#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define INF INT_MAX
#define REPEAT 10000  // Increase number of repetitions for better timing
int minDistance(int dist[], bool sptSet[], int V);
void dijkstra(int graph[20][20], int src, int V);
void bellmanFord(int graph[20][3], int V, int E, int src);
double calculateExecutionTimeDijkstra(void (*func)(int[][20], int, int), int graph[20][20], int src, int V);
double calculateExecutionTimeBellmanFord(void (*func)(int[][3], int, int, int), int graph[20][3], int V, int E, int src);

// Function to find the vertex with the minimum distance
int minDistance(int dist[], bool sptSet[], int V) {
    int min = INF, min_index;
    for (int v = 0; v < V; v++)
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    return min_index;
}

// Dijkstra's algorithm
void dijkstra(int graph[20][20], int src, int V) {
    int dist[V];
    bool sptSet[V];
    
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        sptSet[i] = false;
    }
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
}

// Bellman-Ford algorithm
void bellmanFord(int graph[20][3], int V, int E, int src) {
    int dist[V];
    for (int i = 0; i < V; i++)
        dist[i] = INF;
    dist[src] = 0;

    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = graph[j][0];
            int v = graph[j][1];
            int weight = graph[j][2];
            if (dist[u] != INF && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }
}

// Utility function to calculate execution time for Dijkstra
double calculateExecutionTimeDijkstra(void (*func)(int[][20], int, int), int graph[20][20], int src, int V) {
    clock_t start, end;
    start = clock();
    for (int i = 0; i < REPEAT; i++) {  // Repeat the algorithm
        func(graph, src, V);
    }
    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC * 1000 / REPEAT; // Average Time in ms
}

// Utility function to calculate execution time for Bellman-Ford
double calculateExecutionTimeBellmanFord(void (*func)(int[][3], int, int, int), int graph[20][3], int V, int E, int src) {
    clock_t start, end;
    start = clock();
    for (int i = 0; i < REPEAT; i++) {  // Repeat the algorithm
        func(graph, V, E, src);
    }
    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC * 1000 / REPEAT; // Average Time in ms
}

// Test function
void compareAlgorithms() {
    // Graphs input data for Dijkstra
    int graph1[20][20] = {
        {0, 4, 1, 0},
        {0, 0, 2, 5},
        {0, 0, 0, 3},
        {0, 0, 0, 0}
    };

    int graph2[20][20] = {
        {0, 3, 2, 0, 0, 0},
        {0, 0, 0, 7, 4, 0},
        {0, 0, 0, 1, 0, 5},
        {0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0}
    };

    int graph3[20][20] = {
        {0, 6, 2, 5, 0, 0, 0, 0},
        {0, 0, 0, 1, 4, 0, 0, 0},
        {0, 0, 0, 0, 0, 5, 0, 0},
        {0, 0, 0, 0, 0, 0, 3, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    // Graphs input data for Bellman-Ford (converted to edge list)
    int graph1_edges[20][3] = {
        {0, 1, 4}, {0, 2, 1}, {1, 2, 2}, {1, 3, 5}, {2, 3, 3}
    };
    int graph2_edges[20][3] = {
        {0, 1, 3}, {0, 2, 2}, {1, 3, 7}, {1, 4, 4}, {2, 3, 1}, {2, 5, 5}, {3, 5, 2}, {4, 5, 1}
    };
    int graph3_edges[20][3] = {
        {0, 1, 6}, {0, 2, 2}, {0, 3, 5}, {1, 3, 1}, {1, 4, 4}, {2, 5, 5}, {2, 4, 5}, {3, 6, 3}, {4, 5, 3}, {5, 6, 1}, {5, 7, 4}, {6, 7, 2}
    };

    // Time to run Dijkstra
    double dijkstra_time_1 = calculateExecutionTimeDijkstra(dijkstra, graph1, 0, 4);
    double dijkstra_time_2 = calculateExecutionTimeDijkstra(dijkstra, graph2, 0, 6);
    double dijkstra_time_3 = calculateExecutionTimeDijkstra(dijkstra, graph3, 0, 8);

    // Time to run Bellman-Ford
    double bellman_time_1 = calculateExecutionTimeBellmanFord(bellmanFord, graph1_edges, 4, 5, 0);
    double bellman_time_2 = calculateExecutionTimeBellmanFord(bellmanFord, graph2_edges, 6, 8, 0);
    double bellman_time_3 = calculateExecutionTimeBellmanFord(bellmanFord, graph3_edges, 8, 12, 0);

    // Output comparison results for Dijkstra's and Bellman-Ford algorithms
    printf("Dijkstra Time (ms) for Graph 1: %.5f\n", dijkstra_time_1);
    printf("Bellman-Ford Time (ms) for Graph 1: %.5f\n", bellman_time_1);

    printf("Dijkstra Time (ms) for Graph 2: %.5f\n", dijkstra_time_2);
    printf("Bellman-Ford Time (ms) for Graph 2: %.5f\n", bellman_time_2);

    printf("Dijkstra Time (ms) for Graph 3: %.5f\n", dijkstra_time_3);
    printf("Bellman-Ford Time (ms) for Graph 3: %.5f\n", bellman_time_3);
}

int main() {
    compareAlgorithms();
    return 0;
}

