#include "simple_tree.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Queue *create_queue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    if (tree_node == NULL) return;
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->tree_node = tree_node;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

TreeNode *dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    QueueNode *temp = q->front;
    TreeNode *res = temp->tree_node;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return res;
}

bool is_empty(Queue *q) { return q->front == NULL; }

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode *build_tree_by_level(int *level_order, int size) {
    if (size == 0 || level_order[0] == INT_MIN) return NULL;

    TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = root->right = NULL;

    Queue *q = create_queue();
    enqueue(q, root);

    int i = 1;
    while (!is_empty(q) && i < size) {
        TreeNode *curr = dequeue(q);

        // Left child
        if (i < size && level_order[i] != INT_MIN) {
            curr->left = (TreeNode *)malloc(sizeof(TreeNode));
            curr->left->val = level_order[i];
            curr->left->left = curr->left->right = NULL;
            enqueue(q, curr->left);
        }
        i++;

        // Right child
        if (i < size && level_order[i] != INT_MIN) {
            curr->right = (TreeNode *)malloc(sizeof(TreeNode));
            curr->right->val = level_order[i];
            curr->right->left = curr->right->right = NULL;
            enqueue(q, curr->right);
        }
        i++;
    }
    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) return;
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) return;

    // Use a simple stack for preorder traversal
    TreeNode *stack[100];
    int top = -1;

    stack[++top] = root;
    while (top >= 0) {
        TreeNode *curr = stack[top--];
        printf("%d ", curr->val);

        if (curr->right) stack[++top] = curr->right;
        if (curr->left) stack[++top] = curr->left;
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
