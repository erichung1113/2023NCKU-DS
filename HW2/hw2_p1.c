#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEGREE 20
#define MAX_HASH 1000000
#define inf INT_MAX

typedef struct Node Node;
struct Node {
    int key, val, degree;
    bool mark;
    Node *parent, *child, *left_sib, *right_sib;
};

//------------------------Hash Map-------------------------
typedef struct HASH HASH;
struct HASH {
    int key, val;
    Node* node_ptr;
    HASH* nxt;
} HashMap[MAX_HASH];

void insert_node_to_HashMap(Node* node) {
    HASH* HashHode = malloc(sizeof(HASH));
    HashHode->val = node->val;
    HashHode->key = node->key;
    HashHode->node_ptr = node;
    int key = ((long long)HashHode->key + INT_MAX) % MAX_HASH;
    HashHode->nxt = HashMap[key].nxt;
    HashMap[key].nxt = HashHode;
}
void delete_node_in_HashMap(Node* node) {
    int key = ((long long)node->key + INT_MAX) % MAX_HASH;
    HASH* cur = &HashMap[key];
    while (cur->nxt->node_ptr != node) cur = cur->nxt;
    HASH* DeletedHashNode = cur->nxt;
    cur->nxt = cur->nxt->nxt;
    free(DeletedHashNode);
}
Node* find_node_in_HashMap(int node_key, int node_val) {
    int key = ((long long)node_key + INT_MAX) % MAX_HASH;
    HASH* cur = HashMap[key].nxt;
    while (cur->key != node_key || cur->val != node_val) cur = cur->nxt;
    return cur->node_ptr;
}
//------------------------Hash Map-------------------------

Node* Min_Root;
Node* degree_matching[MAX_DEGREE];

void insert_to_left(Node* root, Node* left_node) {
    left_node->left_sib = root->left_sib;
    left_node->right_sib = root;
    root->left_sib->right_sib = left_node;
    root->left_sib = left_node;
}

void insert(int x, int val) {
    Node* new_node = calloc(1, sizeof(Node));
    new_node->key = x;
    new_node->val = val;
    new_node->left_sib = new_node;
    new_node->right_sib = new_node;
    insert_node_to_HashMap(new_node);

    if (Min_Root == NULL) {
        Min_Root = new_node;
    } else {
        insert_to_left(Min_Root, new_node);
        if (new_node->key < Min_Root->key) Min_Root = new_node;
    }
}

Node* merge(Node* a, Node* b) {
    if (a->key > b->key) {
        Node* temp = a;
        a = b;
        b = temp;
    }
    // merge b to a's child
    b->parent = a;
    if (a->child)
        insert_to_left(a->child, b);
    else
        a->child = b;
    a->degree++;
    return a;
}

void Consolidate() {
    Node* root_list_head = Min_Root;
    Node* cur = root_list_head;
    do {  // iterate root list
        Node* next_node = cur->right_sib;
        cur->left_sib = cur;
        cur->right_sib = cur;
        int d = cur->degree;
        while (degree_matching[cur->degree]) {
            Node* match = degree_matching[cur->degree];
            degree_matching[cur->degree] = NULL;
            cur = merge(match, cur);
        }
        degree_matching[d] = cur;

        cur = next_node;
    } while (cur != root_list_head);
    // merge root list
    Min_Root = NULL;
    for (int i = 0; i < MAX_DEGREE; i++) {
        if (degree_matching[i]) {
            if (Min_Root) {
                insert_to_left(Min_Root, degree_matching[i]);
                if (Min_Root->left_sib->key < Min_Root->key) Min_Root = Min_Root->left_sib;
            } else {
                Min_Root = degree_matching[i];
            }
            degree_matching[i] = NULL;
        }
    }
}

void extract() {
    if (Min_Root->right_sib == Min_Root && Min_Root->child == NULL) {  // only one Node
        free(Min_Root);
        Min_Root = NULL;
        return;
    }
    if (Min_Root->child) {
        Node* child_head = Min_Root->child;
        Node* cur = child_head;
        do {  // add to root list
            Node* next_node = cur->right_sib;
            cur->parent = NULL;
            insert_to_left(Min_Root, cur);
            cur = next_node;
        } while (cur != child_head);
    }

    // extract min
    Min_Root->left_sib->right_sib = Min_Root->right_sib;
    Min_Root->right_sib->left_sib = Min_Root->left_sib;
    Node* min_root = Min_Root;
    Min_Root = Min_Root->left_sib;
    delete_node_in_HashMap(min_root);
    free(min_root);

    Consolidate();
}

void cut(Node* node) {
    if (node->right_sib != node) {  // multiple children
        node->left_sib->right_sib = node->right_sib;
        node->right_sib->left_sib = node->left_sib;
        node->parent->child = node->left_sib;
    } else {
        node->parent->child = NULL;
    }
    node->parent = NULL;
    if (node->mark) node->mark = false;

    // add to root list
    insert_to_left(Min_Root, node);
    if (node->key < Min_Root->key) Min_Root = node;
}

void Cascading_Cut(Node* node) {
    if (node->parent == NULL) return;

    if (node->mark) {
        node->mark = true;
    } else {
        Cascading_Cut(node->parent);
        cut(node);
    }
}

void decrease(int x, int val, int y) {
    Node* node = find_node_in_HashMap(x, val);

    delete_node_in_HashMap(node);
    node->key -= y;
    if (y == inf) node->key = -inf;
    insert_node_to_HashMap(node);

    if (node->parent == NULL) {  // root list's node
        if (node->key < Min_Root->key) Min_Root = node;
    } else {
        if (node->key < node->parent->key) {
            Cascading_Cut(node->parent);
            cut(node);
        }
    }
}

void delete(int x, int val) {
    decrease(x, val, inf);
    extract();
}

int main() {
    int x, y, val;
    char opt[100];
    while (scanf("%s", opt) != EOF && opt[0] != 'q') {
        if (!strcmp(opt, "insert")) {
            scanf("%d %d", &x, &val);
            insert(x, val);

        } else if (!strcmp(opt, "extract")) {
            if (Min_Root == NULL) continue;
            printf("(%d)%d\n", Min_Root->key, Min_Root->val);
            extract();

        } else if (!strcmp(opt, "delete")) {
            scanf("%d %d", &x, &val);
            delete (x, val);

        } else {
            scanf("%d %d %d", &x, &val, &y);
            decrease(x, val, y);
        }
    }
}
