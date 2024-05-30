#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * 题目描述：
 * 有一个二叉搜索树因为有两个节点更换了位置导致不是二叉搜索树了,要求恢复这个二叉搜索树 
 * 
 * 
 * 
 * 恢复二叉搜索树思路:
 * 1、中序遍历二叉树得到一个序列存到结构体数组里面
 * 2、寻找逆序对,如果只有一个逆序对,则只需要更换相邻节点即可
 * 3、如果存在两个逆序对,则需要更换第一个逆序对的第一个节点和第二个逆序对的第二个节点
*/


struct binary_tree {
    int data;
    struct binary_tree *rch, *lch;
};

struct TreeNode {
    int val;
    struct TreeNode *left, *right;
};

struct TreeNode_temp {
    int val;
    struct TreeNode_temp *node;
};

static int count = 0;
static int temp;
static int index1 = 0;
static int index2 = 0;
static int err_index_count = 0;

void mid_count_node(struct TreeNode *root)
{
    if (root == NULL)
        return;
    count++;
    mid_count_node(root->left);
    mid_count_node(root->right);
    return;
}

void mid_array_node(struct TreeNode *root, struct TreeNode_temp *p)
{
    if (p == NULL || root == NULL)
        return;
    mid_array_node(root->left, p);
    temp = count++;
    p[temp].val = root->val;
    p[temp].node = root;
    mid_array_node(root->right, p);
    return;
}

void find_err_node(struct TreeNode *root, struct TreeNode_temp *p)
{
    for (size_t i = 0; i < count - 1; i++) {
        if (p[i + 1].val < p[i].val) {
            if (index1 == -1) {
                index1 = i;
                err_index_count = 1;
            } else {
                index2 = i + 1;
                err_index_count = 2;
                break;
            }
        }
    }

    if (index1 != -1 && index2 != -1) {
        int tes = p[index1].node->val;
        p[index1].node->val = p[index2].node->val;
        p[index2].node->val = tes;
    } else {
        if (index1 != -1) {
            int tes = p[index1].node->val;
            p[index1].node->val = p[index1 + 1].node->val;
            p[index1 + 1].node->val = tes;

        } else {
            int tes = p[index1].node->val;
            p[index1].node->val = p[index1 + 1].node->val;
            p[index1 + 1].node->val = tes;
        }
    }
}

void recoverTree(struct TreeNode *root)
{
    mid_count_node(root);
    struct TreeNode_temp *p = (struct TreeNode_temp *)malloc(sizeof(struct TreeNode_temp) * count);
    count = 0;
    mid_array_node(root, p);
    index1 = -1;
    index2 = -1;
    find_err_node(root, p);
    return;
}

