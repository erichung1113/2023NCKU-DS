#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXN 105
int edge[MAXN][MAXN];
int used[MAXN];

signed main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            scanf("%d", &edge[i][j]);
        }
    }
    int queue[MAXN], head = 0, tail = 0;
    queue[tail++] = 1;
    used[1] = true;

    while (head < tail) {
        int now = queue[head++];
        printf("%d ", now);
        for (int i = 1; i <= n; i++) {
            if (!used[i] && edge[now][i]) {
                queue[tail++] = i;
                used[i] = true;
            }
        }
    }
}