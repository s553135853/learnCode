#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
struct jili_list_node {
    struct jili_list_node *next;
    int flag;
    unsigned char *_data;
};

struct jili_list_node *init_list();
void push_data_to_list(struct jili_list_node *head, unsigned char *_data);
void *show_data(void *avg);
void *test_jili_list_node(void *avg);

int main(int argc, char const *argv[])
{
    struct jili_list_node *head = init_list();
    pthread_t pid;
    int ret;
    ret = pthread_create(&pid, NULL, (void *)test_jili_list_node, head);
    if (ret < 0)
        return -1;

    ret = pthread_create(&pid, NULL, (void *)show_data, head);
    if (ret < 0)
        return -1;

    sleep(5);
    // head->next->flag = 1;
    head->flag = 1;

    pthread_join(pid, NULL);
    return 0;
}

struct jili_list_node *init_list()
{
    struct jili_list_node *head = (struct jili_list_node *)malloc(sizeof(struct jili_list_node));
    head->flag = 0;
    head->_data = NULL;
    head->next = NULL;
    return head;
}

void push_data_to_list(struct jili_list_node *head, unsigned char *_data)
{
    if (head == NULL)
        return;

    if (!head->flag && head->_data == NULL) {
        head->_data = _data;
    } else if (head->next == NULL) {
        struct jili_list_node *node = (struct jili_list_node *)malloc(sizeof(struct jili_list_node));
        node->_data = _data;
        node->flag = 0;
        node->next = NULL;
        head->next = node;
    } else if (head->next->flag){
        head->_data = _data;
    } else {
        head->next->_data = _data;
    }
}

void *show_data(void *avg)
{
    struct jili_list_node *head = avg;
    while (1) {
        if (head == NULL)
            continue;
        if (head->_data != NULL)
            printf("head:%c\n", head->_data[0]);
        if (head->next != NULL && head->next->_data != NULL)
            printf("last:%c\n", head->next->_data[0]);
        sleep(1);
    }
    return NULL;
}

void *test_jili_list_node(void *avg)
{
    struct jili_list_node *head = avg;
    unsigned char *_data;
    int count = 0;
    while (1) {
        _data = malloc(sizeof(unsigned char) * 5);
        if (count % 2 == 0) {
            _data[0] = 'a';
        } else {
            _data[0] = 'b';
        }
        push_data_to_list(head, _data);
        count++;
        sleep(1);
    }

    return NULL;
}
