#include <stdio.h>
#include <stdlib.h>

struct binary_tree {
    int data;
    struct binary_tree *rch, *lch;
};

struct binary_tree *create_binary_tree(int data);
struct binary_tree *insert_node(struct binary_tree *root, int num);
void pre_mid_traverse(struct binary_tree *tree);
void find_node(struct binary_tree *tree, int num);
void destory_binary_tree(struct binary_tree *tree);

int main(int argc, char const *argv[])
{
    struct binary_tree *new_tree = create_binary_tree(5);
    printf("data:%d\n", new_tree->data);
    insert_node(new_tree, 3);
    insert_node(new_tree, 9);
    insert_node(new_tree, 1);

    pre_mid_traverse(new_tree);

    find_node(new_tree, 1);
    destory_binary_tree(new_tree);
    new_tree = NULL;
    pre_mid_traverse(new_tree);

    return 0;
}

struct binary_tree *create_binary_tree(int data)
{
    struct binary_tree *tree = NULL;
    tree = (struct binary_tree *)malloc(sizeof(struct binary_tree));
    if (tree == NULL)
        return NULL;
    tree->data = data;
    tree->rch = tree->lch = NULL;
    return tree;
}

struct binary_tree *insert_node(struct binary_tree *root, int num)
{
    struct binary_tree *node = NULL;

    if (root == NULL) {
        return create_binary_tree(num);
    }

    if (num < root->data) {
        root->lch = insert_node(root->lch, num);
    } else {
        root->rch = insert_node(root->rch, num);
    }

    return root;
}

void pre_mid_traverse(struct binary_tree *tree)
{
    if (tree == NULL)
        return;
    else {
        pre_mid_traverse(tree->lch);
        printf("%d\n", tree->data);
        pre_mid_traverse(tree->rch);
    }
}

void find_node(struct binary_tree *tree, int num)
{
    if (tree == NULL)
        return;

    if (num == tree->data)
        printf("find data:%d\n", num);
    else {
        if (num < tree->data)
            find_node(tree->lch, num);
        else
            find_node(tree->rch, num);
    }
}

void destory_binary_tree(struct binary_tree *tree)
{
    if (tree == NULL) 
        return;
    
    destory_binary_tree(tree->lch);
    destory_binary_tree(tree->rch);
    free(tree);
}