#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RED, BLACK } NodeColor;
typedef struct Node Node;

struct Node {
    int val;
    NodeColor color;
    Node *left, *right, *parent;
    bool NIL;
};
Node *Root;

Node* NIL_Node(){
    Node* nil = malloc(sizeof(Node));
    nil->NIL = true;
    nil->color = BLACK;
    return nil;
}

Node *search(int target) {
    Node *cur = Root;
    while (!cur->NIL) {
        if (cur->val == target) {
            return cur;
        } else if (target < cur->val) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return NULL;
}

Node *Uncle(Node *node) {
    if (node->parent->parent->left == node->parent) {
        return node->parent->parent->right;
    } else {
        return node->parent->parent->left;
    }
}

Node *Sibling(Node *node) {
    if (node->parent->left == node) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

bool is_triangle(Node *node) {
    if (node->parent->parent->left == node->parent) {
        if (node->parent->right == node)
            return true;
        else
            return false;
    } else {
        if (node->parent->left == node)
            return true;
        else
            return false;
    }
}

bool is_left_child(Node *node) {
    if (node->parent->left == node) return true;
    return false;
}

void left_rotate(Node *root) { //待確認
    Node *ori_root = root;
    Node *new_root = root->right;
    
    new_root->parent = ori_root->parent;
    if (ori_root == Root) {
        Root = new_root;
    } else {
        if (ori_root->parent->right == ori_root) {
            ori_root->parent->right = new_root;
        } else {
            ori_root->parent->left = new_root;
        }
    }

    ori_root->right = new_root->left;
    if (new_root->left) new_root->left->parent = ori_root;
    
    new_root->left = ori_root;
    ori_root->parent = new_root;
}

void right_rotate(Node *root) { //待確認
    Node *ori_root = root;
    Node *new_root = root->left;

    new_root->parent = ori_root->parent;
    if (ori_root == Root) {
        Root = new_root;
    } else {
        if (ori_root->parent->right == ori_root) {
            ori_root->parent->right = new_root;
        } else {
            ori_root->parent->left = new_root;
        }
    }

    ori_root->left = new_root->right;
    if (new_root->right) new_root->right->parent = ori_root;
    
    new_root->right = ori_root;
    ori_root->parent = new_root;
}

void fix_line(Node *cur){
    if (cur->parent->right == cur) {
        left_rotate(cur->parent->parent);
        cur->parent->left->color = RED;
        cur->parent->color = BLACK;
    } else {
        right_rotate(cur->parent->parent);
        cur->parent->right->color = RED;
        cur->parent->color = BLACK;
    }
}

void fix_triangle(Node *cur){
    if (cur->parent->right == cur) {
        left_rotate(cur->parent);
        fix_line(cur->left);
    } else {
        right_rotate(cur->parent);
        fix_line(cur->right);
    }
}

void insert_fix(Node *cur) {
    if(cur == Root) {
        cur->color = BLACK;
        return;
    }

    if(cur->parent->color == BLACK) return;
    
    Node *uncle = Uncle(cur);
    if (uncle && uncle->color == RED) {
        uncle->color = BLACK;
        cur->parent->color = BLACK;
        cur->parent->parent->color = RED;
        insert_fix(cur->parent->parent);
    } else if(is_triangle(cur)) {
        fix_triangle(cur);
    }else {
        fix_line(cur);
    }
}

void insert(int val) {
    Node *new_node = calloc(1, sizeof(Node));
    new_node->val = val;
    new_node->color = RED;
    new_node->NIL = false;
    new_node->left = NIL_Node();
    new_node->right = NIL_Node();

    if (Root->NIL) {
        free(Root);
        Root = new_node;
    }else {
        Node *cur = Root, *nxt = Root;
        do{
            cur = nxt;
            if (new_node->val < cur->val) nxt = cur->left;
            else nxt = cur->right;
        }while(!nxt->NIL);
        new_node->parent = cur;
        if (new_node->val < cur->val) {
            free(cur->left);
            cur->left = new_node;
        }else {
            free(cur->right);
            cur->right = new_node;
        }
    }
    insert_fix(new_node);
}

void connect_up(Node* cur, Node* child){ // connect child to parent
    if(cur == Root) {
        Root = child;
        child->parent = NULL;
    }else{
        if(cur->parent->right == cur) cur->parent->right = child;
        else cur->parent->left = child;
        child->parent = cur->parent;
    }
}

void delete_fix(Node* cur){
    while(cur->color == BLACK && cur != Root){
        Node* sibling=Sibling(cur);
        if(sibling->color == RED){ 
            sibling->color = BLACK;
            cur->parent->color = RED;
            if(is_left_child(cur)) left_rotate(cur->parent);
            else right_rotate(cur->parent);
        }else{ //siblings color is black
            if(sibling->left->color == BLACK && sibling->right->color == BLACK){
                sibling->color = RED;
                cur = cur->parent;
            }else{ // one red one black
                if(is_left_child(cur)){
                    if(sibling->right->color == RED){  
                        sibling->color = cur->parent->color;
                        sibling->parent->color = BLACK;
                        sibling->right->color = BLACK;
                        left_rotate(cur->parent);
                        cur = Root;
                    }else{
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        right_rotate(sibling);
                    }
                }else{
                    if(sibling->left->color == RED){
                        sibling->color = cur->parent->color;
                        sibling->parent->color = BLACK;
                        sibling->left->color = BLACK;
                        right_rotate(cur->parent);
                        cur = Root;
                    }else{
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        left_rotate(sibling);
                    }
                }
            }
        }
    }
    cur->color = BLACK;
}

void delete(int val) {
    Node * cur = search(val);
    if(cur == NULL) return;
    NodeColor delete_color = cur->color;
    
    if(!cur->left->NIL && !cur->right->NIL){
        Node * right_subtree_min = cur->right;
        while(!right_subtree_min->left->NIL) right_subtree_min = right_subtree_min->left;
        cur->val = right_subtree_min->val;
        cur = right_subtree_min;
    }

    Node* delete_node = cur;
    if(cur->left->NIL){ // link cur->right
        connect_up(cur, cur->right);
        cur = cur->right;
        free(delete_node->left);
        free(delete_node);
    }else{ // cur->right->NIL
        connect_up(cur, cur->left);
        cur = cur->left;
        free(delete_node->right);
        free(delete_node);
    }
    if(delete_color == BLACK) delete_fix(cur);
    
}

int main() {
    Root = NIL_Node();
    char command[100];
    int val;
    // FILE *file = fopen("testcase", "r");
    // while (fscanf(file, "%s", command) != EOF && command[0] != 'q') {
    //     fscanf(file, "%d", &val);
    
    while (scanf("%s", command) != EOF && command[0] != 'q') {
        scanf("%d", &val);
        if (strcmp(command, "search") == 0) {
            Node *node = search(val);
            if (node) printf("%s\n", node->color == RED ? "red" : "black");
            else printf("Not found\n");

        } else if (strcmp(command, "insert") == 0) {
            insert(val);

        } else if (strcmp(command, "delete") == 0) {
            delete (val);
        }else{
            search(13)->color = BLACK;
            search(15)->color = RED;
            search(23)->color = BLACK;
        }
    }

}
