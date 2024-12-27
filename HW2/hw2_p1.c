#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

int table[105][25];
int bucket_size[105];

int main() {
    memset(table, -1, sizeof(table));
    char cmd[10];
    int n, m;
    scanf("%s %d %s %d", cmd, &n, cmd, &m);

    int val;
    while (scanf("%s %d", cmd, &val) && strcmp(cmd, "exit") != 0) {
        int key = val % n;
        if (strcmp(cmd, "insert") == 0) {
            while (bucket_size[key] == m) {
                key++;
                if (key == n) key = 0;
            }
            for (int i = 0; i < m; i++) {
                if (table[key][i] == -1) {
                    table[key][i] = val;
                    bucket_size[key]++;
                    break;
                }
            }
        }else if(strcmp(cmd, "delete") == 0){
            int find = 0;
            while(!find){
                for (int i = 0; i < m; i++) {
                    if (table[key][i] == val) {
                        table[key][i] = -1;
                        bucket_size[key]--;
                        find = 1;
                        break;
                    }
                }
                key++;
                if(key == n) key = 0;
            }
        }else{
            int find = 0;
            while(!find){
                for (int i = 0; i < m; i++) {
                    if (table[key][i] == val) {
                        printf("%d %d\n", key, i);
                        find = 1;
                        break;
                    }
                }
                key++;
                if(key == n) key = 0;
            }
        }
    }
}
