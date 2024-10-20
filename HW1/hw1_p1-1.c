#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

void string_add(char *a, char *b, char *c) {
    strcat(b, c);
    strcat(a, b);
}

char *Prefix_To_Infix(char *prefix) {
    char *stack[MAXN];
    int top = 0;
    for (int i = strlen(prefix) - 1; i >= 0; i--) {
        char *b = calloc(MAXN, sizeof(char));
        b[0] = prefix[i];
        if (isOperator(prefix[i])) {
            char *a = stack[--top];
            char *c = stack[--top];
            string_add(a, b, c);
            stack[top++] = a;
        } else {
            stack[top++] = b;
        }
    }
    return stack[0];
}

int main() {
    char prefix[MAXN];
    scanf("%s", prefix);
    char *infix = Prefix_To_Infix(prefix);
    printf("%s", infix);
}
