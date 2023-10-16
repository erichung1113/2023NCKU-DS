#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int MAXN = 1e6 + 5;
const int MAXE = 2e6 + 5;

typedef struct Edge {
    int u, v, c;
} Edge;
Edge edge[MAXE];

int compare(const void *a, const void *b) {
    return (((Edge *)a)->c - ((Edge *)b)->c);
}

int dsu[MAXN];
int fa(int cur) {
    if (cur == dsu[cur]) return cur;
    return dsu[cur] = fa(dsu[cur]);
}
void uni(int a, int b) {
    dsu[fa(a)] = fa(b);
}
bool same(int a, int b) {
    return (fa(a) == fa(b));
}

signed main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) dsu[i] = i;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &edge[i].u, &edge[i].v, &edge[i].c);
    }
    qsort(edge, m, sizeof(edge[0]), compare);
    long long MST = 0;
    for (int i = 0; i < m; i++) {
        if (!same(edge[i].u, edge[i].v)) {
            MST += edge[i].c;
            uni(edge[i].u, edge[i].v);
        }
    }
    printf("%lld\n", MST);
}