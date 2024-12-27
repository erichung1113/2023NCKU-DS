#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Node structure for Fibonacci Heap
typedef struct FibNode {
    int key;
    int degree;
    bool marked;
    struct FibNode* parent;
    struct FibNode* child;
    struct FibNode* left;
    struct FibNode* right;
} FibNode;

// Fibonacci Heap structure
typedef struct {
    FibNode* min;
    int n;  // Number of nodes
} FibHeap;

// Function declarations
FibHeap* createFibHeap();
void insert(FibHeap* heap, int key);
int extractMin(FibHeap* heap);
void decreaseKey(FibHeap* heap, FibNode* node, int value);
void delete(FibHeap* heap, FibNode* node);
void printHeap(FibHeap* heap);
FibNode* findNode(FibHeap* heap, int key);

// Helper function to create a new node
FibNode* createNode(int key) {
    FibNode* node = (FibNode*)malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->marked = false;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    return node;
}

// Create new Fibonacci Heap
FibHeap* createFibHeap() {
    FibHeap* heap = (FibHeap*)malloc(sizeof(FibHeap));
    heap->min = NULL;
    heap->n = 0;
    return heap;
}

// Helper function to link two nodes in circular doubly linked list
void addToRootList(FibNode* node, FibNode* root) {
    if (root == NULL) return;
    
    node->left = root->left;
    node->right = root;
    root->left->right = node;
    root->left = node;
}

// Insert a new key into the heap
void insert(FibHeap* heap, int key) {
    FibNode* node = createNode(key);
    
    if (heap->min == NULL) {
        heap->min = node;
    } else {
        addToRootList(node, heap->min);
        if (node->key < heap->min->key) {
            heap->min = node;
        }
    }
    
    heap->n++;
}

// Helper function to merge two trees of same degree
void link(FibHeap* heap, FibNode* y, FibNode* x) {
    // Remove y from root list
    y->left->right = y->right;
    y->right->left = y->left;
    
    // Make y a child of x
    if (x->child == NULL) {
        x->child = y;
        y->right = y;
        y->left = y;
    } else {
        // Insert y into child list in sorted order
        FibNode* current = x->child;
        FibNode* start = current;
        bool inserted = false;
        
        do {
            if (y->key < current->key) {
                // Insert before current
                y->left = current->left;
                y->right = current;
                current->left->right = y;
                current->left = y;
                
                if (current == x->child) {
                    x->child = y;  // Update child pointer if inserting at start
                }
                inserted = true;
                break;
            }
            current = current->right;
        } while (current != start);
        
        if (!inserted) {
            // Insert at end if larger than all existing children
            addToRootList(y, x->child);
        }
    }
    
    y->parent = x;
    x->degree++;
    y->marked = false;
}

// Consolidate the heap after extract-min
void consolidate(FibHeap* heap) {
    int max_degree = (int)(log(heap->n) / log(2)) + 1;
    FibNode** A = (FibNode**)calloc(max_degree, sizeof(FibNode*));
    
    // Create root list array and count roots
    FibNode* start = heap->min;
    FibNode* current = start;
    int root_count = 0;
    do {
        root_count++;
        current = current->right;
    } while (current != start);
    
    // Create and fill array of root nodes
    FibNode** roots = (FibNode**)malloc(root_count * sizeof(FibNode*));
    current = start;
    for (int i = 0; i < root_count; i++) {
        roots[i] = current;
        current = current->right;
    }
    
    // Sort roots by degree and key
    for (int i = 0; i < root_count - 1; i++) {
        for (int j = 0; j < root_count - i - 1; j++) {
            if (roots[j]->degree > roots[j + 1]->degree || 
                (roots[j]->degree == roots[j + 1]->degree && 
                 roots[j]->key > roots[j + 1]->key)) {
                FibNode* temp = roots[j];
                roots[j] = roots[j + 1];
                roots[j + 1] = temp;
            }
        }
    }
    
    // Process roots in sorted order
    for (int i = 0; i < root_count; i++) {
        current = roots[i];
        if (current == NULL) continue;  // Skip if already processed
        
        int d = current->degree;
        
        while (A[d] != NULL) {
            FibNode* other = A[d];
            if (other == NULL) break;  // Skip if already processed
            
            if (current->key > other->key) {
                FibNode* temp = current;
                current = other;
                other = temp;
            }
            
            link(heap, other, current);
            A[d] = NULL;
            d++;
        }
        
        A[d] = current;
    }
    
    // Rebuild root list and find new minimum
    heap->min = NULL;
    for (int i = 0; i < max_degree; i++) {
        if (A[i] != NULL) {
            if (heap->min == NULL) {
                heap->min = A[i];
                heap->min->left = heap->min;
                heap->min->right = heap->min;
            } else {
                addToRootList(A[i], heap->min);
                if (A[i]->key < heap->min->key) {
                    heap->min = A[i];
                }
            }
        }
    }
    
    free(A);
    free(roots);
}

// Extract minimum node from the heap
int extractMin(FibHeap* heap) {
    if (heap->min == NULL) return -1;
    
    FibNode* z = heap->min;
    int min_key = z->key;
    
    // Add all children to root list
    if (z->child != NULL) {
        FibNode* child = z->child;
        do {
            FibNode* next = child->right;
            addToRootList(child, heap->min);
            child->parent = NULL;
            child = next;
        } while (child != z->child);
    }
    
    // Remove z from root list
    if (z->right == z) {
        heap->min = NULL;
    } else {
        z->left->right = z->right;
        z->right->left = z->left;
        heap->min = z->right;
    }
    
    if (heap->min != NULL) {
        consolidate(heap);
    }
    
    heap->n--;
    free(z);
    return min_key;
}

// Cut a node from its parent
void cut(FibHeap* heap, FibNode* child, FibNode* parent) {
    // Remove child from parent's child list
    if (parent->child == child) {
        if (child->right == child) {
            parent->child = NULL;
        } else {
            parent->child = child->right;
        }
    }
    
    child->left->right = child->right;
    child->right->left = child->left;
    parent->degree--;
    
    // Add child to root list
    addToRootList(child, heap->min);
    child->parent = NULL;
    child->marked = false;
}

// Cascading cut operation
void cascadingCut(FibHeap* heap, FibNode* node) {
    FibNode* parent = node->parent;
    if (parent != NULL) {
        if (!node->marked) {
            node->marked = true;
        } else {
            cut(heap, node, parent);
            cascadingCut(heap, parent);
        }
    }
}

// Decrease key operation
void decreaseKey(FibHeap* heap, FibNode* node, int value) {
    if (node == NULL || value >= node->key) return;
    
    node->key = value;
    FibNode* parent = node->parent;
    
    if (parent != NULL && node->key < parent->key) {
        cut(heap, node, parent);
        cascadingCut(heap, parent);
    }
    
    if (node->key < heap->min->key) {
        heap->min = node;
    }
}

// Find a node with given key
FibNode* findNodeHelper(FibNode* node, int key) {
    if (node == NULL) return NULL;
    
    FibNode* start = node;
    FibNode* result = NULL;
    
    do {
        if (node->key == key) return node;
        
        if (node->child != NULL) {
            result = findNodeHelper(node->child, key);
            if (result != NULL) return result;
        }
        
        node = node->right;
    } while (node != start);
    
    return NULL;
}

FibNode* findNode(FibHeap* heap, int key) {
    if (heap->min == NULL) return NULL;
    return findNodeHelper(heap->min, key);
}

// Delete a node
void delete(FibHeap* heap, FibNode* node) {
    decreaseKey(heap, node, -2147483648);  // Using INT_MIN
    extractMin(heap);
}

// Helper function to print a single tree in level order
void printSingleTree(FibNode* root) {
    if (root == NULL) return;
    
    // Use a queue to perform level-order traversal
    FibNode** queue = malloc(10000 * sizeof(FibNode*));
    int front = 0, rear = 0;
    
    // Add root to queue
    queue[rear++] = root;
    
    // Process nodes in queue
    while (front < rear) {
        FibNode* current = queue[front++];
        printf("%d ", current->key);
        
        // Add children to queue in left-to-right order
        if (current->child != NULL) {
            FibNode* child = current->child;
            do {
                queue[rear++] = child;
                child = child->right;
            } while (child != current->child);
        }
    }
    
    free(queue);
}

void printHeap(FibHeap* heap) {
    if (heap->min == NULL) return;
    
    // Create array of root nodes
    FibNode* current = heap->min;
    int root_count = 0;
    do {
        root_count++;
        current = current->right;
    } while (current != heap->min);
    
    FibNode** roots = malloc(root_count * sizeof(FibNode*));
    current = heap->min;
    for (int i = 0; i < root_count; i++) {
        roots[i] = current;
        current = current->right;
    }
    
    // Sort roots by degree
    for (int i = 0; i < root_count - 1; i++) {
        for (int j = 0; j < root_count - i - 1; j++) {
            if (roots[j]->degree > roots[j + 1]->degree) {
                FibNode* temp = roots[j];
                roots[j] = roots[j + 1];
                roots[j + 1] = temp;
            }
        }
    }
    
    // Print each tree in level order
    for (int i = 0; i < root_count; i++) {
        printSingleTree(roots[i]);
        printf("\n");
    }
    
    free(roots);
}

int main() {
    FibHeap* heap = createFibHeap();
    char command[20];
    int key, value;
    
    while (1) {
        scanf("%s", command);
        
        if (strcmp(command, "insert") == 0) {
            scanf("%d", &key);
            insert(heap, key);
        }
        else if (strcmp(command, "extract-min") == 0) {
            extractMin(heap);
        }
        else if (strcmp(command, "decrease") == 0) {
            scanf("%d %d", &key, &value);
            FibNode* node = findNode(heap, key);
            if (node != NULL) {
                decreaseKey(heap, node, key - value);
            }
        }
        else if (strcmp(command, "delete") == 0) {
            scanf("%d", &key);
            FibNode* node = findNode(heap, key);
            if (node != NULL) {
                delete(heap, node);
            }
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    
    printHeap(heap);
    
    // Clean up
    free(heap);
    
    return 0;
}
