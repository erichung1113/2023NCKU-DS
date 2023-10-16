#include <stdio.h>
#include <stdbool.h>
#define max(a,b) (a>b?a:b)

const int MAXN = 1e4 + 5;

int dsu[MAXN], height[MAXN];
int find(int cur) {
    if (cur == dsu[cur]) return cur;
    return dsu[cur] = find(dsu[cur]);
}
void uni(int a, int b) {
    a = find(a), b = find(b);
    if(a == b) return;
    if (height[a] >= height[b]) {
        dsu[b] = a;
        height[a] = max(height[a], height[b] + 1);
    } else {
        dsu[a] = b;
        height[b] = max(height[b], height[a] + 1);
    }
}
bool same(int a, int b) {
    return (find(a) == find(b));
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        int n, m, a, b;
        scanf("%d %d", &n, &m);
        for (int i = 0; i < n; i++) dsu[i] = i, height[i] = 1;
        while (m--) {
            char ope[10];
            scanf("%s", ope);
            if (ope[0] == 'u') {
                scanf("%d %d", &a, &b);
                uni(a, b);
                continue;
            } else if (ope[0] == 'f') {
                scanf("%d", &a);
                printf("%d", find(a));
            } else {
                scanf("%d %d", &a, &b);
                printf("%s", same(a, b)?"true":"false");
            }
            if(m || t) printf("\n");
        }
    }
}