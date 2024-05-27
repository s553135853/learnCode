#include <stdio.h>
#include <stdlib.h>

struct binary_tree {
    int data;
    struct binary_tree *rch, *lch;
};

int init_binary_tree(struct binary_tree *tree);
int destory_biTree(struct binary_tree *tree);
struct binary_tree *create_binary_tree();
void pre_order_traverse(struct binary_tree *tree);
int max_depth(struct binary_tree *tree);
int tree_high(struct binary_tree *tree);
void leaf_node_num(struct binary_tree *tree, int *sum); // 求叶子节点的个数
int tree_node_num(struct binary_tree *tree);
struct binary_tree *search_node(struct binary_tree *tree, int value);
struct binary_tree *invert_tree(struct binary_tree *tree);

int main(int argc, char const *argv[])
{
    int depth = 0;
    struct binary_tree *init_tree = create_binary_tree();
    struct binary_tree *p_find = NULL;
    pre_order_traverse(init_tree);

    printf("depth:%d\n", max_depth(init_tree));
    printf("sum:%d\n", tree_node_num(init_tree));

    p_find = search_node(init_tree, 3);
    if (p_find != NULL) {
        printf("p_find data:%d\n", p_find->data);
    }

    init_tree = invert_tree(init_tree);
    pre_order_traverse(init_tree);
    return 0;
}

int init_binary_tree(struct binary_tree *tree)
{
    tree = NULL;
    return 0;
}

struct binary_tree *create_binary_tree()
{
    int data;
    struct binary_tree *tree = NULL;
    scanf("%d", &data);
    // printf("w:%d\n", data);
    if (data < 0)
        return NULL;
    else {
        tree = (struct binary_tree *)malloc(sizeof(struct binary_tree));
        if (tree == NULL)
            return NULL;
        tree->data = data;
        tree->lch = create_binary_tree();
        tree->rch = create_binary_tree();
    }

    return tree;
}

void pre_order_traverse(struct binary_tree *tree)
{
    if (tree == NULL)
        return;
    else {
        printf("%d\n", tree->data);
        pre_order_traverse(tree->lch);
        pre_order_traverse(tree->rch);
    }
}

int max_depth(struct binary_tree *tree)
{
    if (tree == NULL)
        return 0;
    int rl = max_depth(tree->rch);
    int ll = max_depth(tree->lch);

    return 1 + (rl > ll ? rl : ll);
}

int tree_high(struct binary_tree *tree)
{
}

void leaf_node_num(struct binary_tree *tree, int *sum)
{
    if (tree == NULL)
        return;
    if (tree->rch == NULL && tree->lch == NULL) {
        (*sum)++;
        return;
    } else {
        leaf_node_num(tree->lch, sum);
        leaf_node_num(tree->rch, sum);
    }
}

int tree_node_num(struct binary_tree *tree)
{
    if (tree == NULL)
        return 0;
    if (tree->rch == NULL && tree->lch == NULL) {
        return 1;
    }

    return tree_node_num(tree->rch) + tree_node_num(tree->lch) + 1; // 1指当前节点本身
}

struct binary_tree *search_node(struct binary_tree *tree, int value)
{
    struct binary_tree *pr = NULL;
    struct binary_tree *pl = NULL;
    if (tree == NULL)
        return NULL;

    if (tree->data == value)
        return tree;
    else {
        pr = search_node(tree->lch, value);
        pl = search_node(tree->rch, value);
        if (pr == NULL && pl == NULL)
            return NULL;
        else {
            return !!pr > 0 ? pr : pl;
        }
    }
}

// 翻转二叉树
struct binary_tree *invert_tree(struct binary_tree *tree)
{
    if (tree == NULL) {
        return NULL;
    }

    struct binary_tree *temp = tree->lch;
    tree->lch = invert_tree(tree->rch);
    tree->rch = invert_tree(temp);
    return tree;
}