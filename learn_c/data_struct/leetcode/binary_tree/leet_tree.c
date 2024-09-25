#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *create_tree(int val);
struct TreeNode *insert_node(struct TreeNode *root, int num);
void pre_mid_traverse(struct TreeNode *tree);

int main(int argc, char const *argv[])
{
    struct TreeNode *tree = create_tree(2);
    insert_node(tree, 1);
    insert_node(tree, 3);
    insert_node(tree, 5);
    insert_node(tree, 7);
    pre_mid_traverse(tree);

    return 0;
}

struct TreeNode *create_tree(int val)
{
    struct TreeNode *tree = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    if (tree == NULL)
        return NULL;
    tree->val = val;
    tree->right = tree->left = NULL;
    return tree;
}

struct TreeNode *insert_node(struct TreeNode *root, int num)
{
    if (root == NULL)
        return create_tree(num);

    if (root->left == NULL)
        root->left = insert_node(root->left, num);
    else if (root->right == NULL)
        root->right = insert_node(root->right, num);
    else
        root->left = insert_node(root->left, num);

    return root;
}

void pre_mid_traverse(struct TreeNode *tree)
{
    if (tree == NULL)
        return;
    else {
        printf("%d\n", tree->val);
        pre_mid_traverse(tree->left);
        pre_mid_traverse(tree->right);
    }
}