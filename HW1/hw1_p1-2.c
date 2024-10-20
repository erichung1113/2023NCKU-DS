#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXN 100

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int Priority(char op) {
    if (op == '+' || op == '-') return 1;
    else if (op == '*' || op == '/') return 2;
    else return 0;
}

char *Infix_To_Postfix(char *infix) {
    char *postfix = calloc(MAXN,sizeof(char));
    char stack[MAXN];
    int top = -1, j = 0;

    for (int i = 0; infix[i] != '\0'; ++i) {
        if (isalpha(infix[i])) {
            postfix[j++] = infix[i];
        } else if (infix[i] == '(') {
            stack[++top] = infix[i];
        } else if (infix[i] == ')') {
            while (top != -1 && stack[top] != '(') {
                postfix[j++] = stack[top--];
            }
            if (top != -1) top--;
        } else if (isOperator(infix[i])) {
            while (top != -1 && Priority(stack[top]) >= Priority(infix[i])) {
                postfix[j++] = stack[top--];
            }
            stack[++top] = infix[i];
        }
    }

    while (top != -1)  postfix[j++] = stack[top--];
    return postfix;
}

int main() {
    char infix[MAXN];
    scanf("%s", infix);
    char *postfix = InfixToPostfix(infix);
    printf("%s", postfix);
}
