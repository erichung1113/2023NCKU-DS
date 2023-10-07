#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAXN 100

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

void string_add(char *a, char *b, char *c) {
    strcat(b, c);
    strcat(a, b);
}

char *PostfixToPrefix(char *postfix) {
    char *stack[MAXN];
    int top = 0;
    for (int i = 0; postfix[i] != '\0'; i++) {
        char *a = calloc(MAXN, sizeof(char));
        a[0] = postfix[i];
        if (isOperator(postfix[i])) {
            char *c = stack[--top];
            char *b = stack[--top];
            string_add(a, b, c);
        }
        stack[top++] = a;
    }
    return stack[0];
}
int main() {
    char postfix[MAXN];
    scanf("%s", postfix);
    char *prefix = PostfixToPrefix(postfix);
    printf("%s", prefix);
}