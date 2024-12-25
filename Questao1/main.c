#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4         // Number of disks
#define Pins 3
#define MAX 81 // Pins ^ N

const int INF = 0x3f3f3f3f;

typedef struct {
    int conf[N];
} vertex;

int adj[MAX][MAX];
//-----------------------------------------------
void configuration(vertex *graph) {
    for (int i = 0; i < MAX; ++i) {
        int num = i;
        for (int j = 0; j < N; ++j) {
            graph[i].conf[j] = num % Pins + 1;
            num /= Pins;
        }
    }
}
//-----------------------------------------------
int edge(vertex v1, vertex v2) {
    int diff = 0, Altered_disc = -1;

    for (int i = 0; i < N && diff <= 1; ++i) {
        if (v1.conf[i] != v2.conf[i]) {
            diff++;
            Altered_disc = i;
        }
    }

    int invalid = 1;

    for (int i = 0; i < N && invalid; ++i) {
        if (i != Altered_disc && ((v1.conf[i] == v1.conf[Altered_disc] && i < Altered_disc) || (v2.conf[i] == v2.conf[Altered_disc] && i < Altered_disc))) {
            invalid = 0;
        }
    }

    return (diff > 1 || !invalid) ? 0 : 1;
}
//-----------------------------------------------
void adjacency(vertex *graph) {
    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            adj[i][j] = edge(graph[i], graph[j]) ? 1 : INF;
}
//-----------------------------------------------
void minimumpath(int *p, int s, int f) {
    printf("Minimum path between settings %d e %d:\n", s, f);

    int path[f + 1], temp = p[f], pos = 0;

    while(temp != -1) temp = p[path[pos++] = temp];

    for (int i = pos - 1; ~i; i--) printf("%d ", path[i]);

    printf("%d\n", f);
}
//-----------------------------------------------
void fordMooreBellman(int s, int end) {
    int dp[MAX], p[MAX];

    memset(p, -1, sizeof(p));

    for (int i = 0; i < MAX; i++) dp[i] = INF;
    dp[s] = 0;
    
    for (int i = 0; i < MAX - 1; ++i) {
        int relaxed = 0;
        for (int j = 0; j < MAX; ++j) {
            if (dp[j] != INF) {
                for (int k = 0; k < MAX; k++) {
                    if (adj[j][k] != INF && dp[j] + adj[j][k] < dp[k]) {
                        dp[k] = dp[j] + adj[j][k];
                        p[k] = j;
                        relaxed = 1;
                    }
                }
            }
        }
        if (!relaxed) break;
    }
    minimumpath(p, s, end);
}
//-----------------------------------------------
void print(vertex *graph) {
    for (int i = 0; i < MAX; ++i) {
        printf("vertex %d: ", i);
        for (int j = 0; j < N; ++j)
            printf("%d ", graph[i].conf[j]);
        printf("\n");
    }
}
//-----------------------------------------------
int main() {
    vertex graph[MAX];

    configuration(graph);
    print(graph);
    adjacency(graph);

    int init, end;

    printf("Enter the initial configuration:\n"); scanf("%d", &init);
    printf("Enter the final configuration:\n"); scanf("%d", &end); 

    fordMooreBellman(init, end);

    return 0;
}