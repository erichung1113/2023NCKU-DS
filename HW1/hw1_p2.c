#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

int heap[MAX_SIZE];
int heapSize = 1;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(int index) {
    while (index > 1 && heap[index] < heap[index / 2]) {
        swap(&heap[index], &heap[index / 2]);
        index = index / 2;
    }
}

void heapifyDown(int index) {
    int smallest = index;
    int leftChild = 2 * index;
    int rightChild = 2 * index + 1;

    if (leftChild < heapSize && heap[leftChild] < heap[smallest]) {
        smallest = leftChild;
    }

    if (rightChild < heapSize && heap[rightChild] < heap[smallest]) {
        smallest = rightChild;
    }

    if (smallest != index) {
        swap(&heap[index], &heap[smallest]);
        heapifyDown(smallest);
    }
}

void insert(int value) {
    heap[heapSize] = value;
    heapifyUp(heapSize);
    heapSize++;
}

void deleteElement(int value) {
    int index = 0;
    for (int i = 1; i < heapSize; i++) {
        if (heap[i] == value) {
            index = i;
            break;
        }
    }

    heap[index] = heap[--heapSize];
    heapifyDown(index);
}

void printHeap() {
    for (int i = 1; i < heapSize; i++) {
        printf("%d ", heap[i]);
    }
    printf("\n");
}

int main() {
    char command[10];
    int value;

    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "insert") == 0) {
            scanf("%d", &value);
            insert(value);
        } else if (strcmp(command, "delete") == 0) {
            scanf("%d", &value);
            deleteElement(value);
        }
        // for(int i=0;i<heapSize;i++) printf("%d ", heap[i]);
        // printf("\n");
    }

    printHeap();
    return 0;
}