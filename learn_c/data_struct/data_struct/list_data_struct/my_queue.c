#include "my_queue.h"

void dequeue(my_queue *queue, my_time_t *data)
{
    if (queue->queue_length <= 0 || queue == NULL)
        return;
    data = queue->head;
    queue->head = queue->head->next;
    queue->queue_length--;
}
void enqueue(my_queue *queue, my_time_t *add_time)
{
    if (queue == NULL | add_time == NULL | queue->queue_length >= queue->capacity)
    {
        return;
    }
    if (queue->head == NULL)
    {
        queue->head = add_time;
        queue->end = add_time;
        queue->end->next = NULL;
        queue->queue_length++;
    }
    else
    {
        queue->end->next = add_time;
        add_time->next = NULL;
        queue->end = add_time;
        queue->queue_length++;
    }
}

int get_queue_length(my_queue *queue)
{
    if (queue == NULL)
        return 0;
    return queue->queue_length;
}

void print_queue(my_queue *queue)
{
    if (queue == NULL)
    {
        return;
    }
    my_queue *temp = queue->head;
    while (queue->head != NULL)
    {
        printf("time:%ju\n", queue->head->time);
        queue->head = queue->head->next;
    }
    queue->head = temp;
}

void free_queue(my_queue *queue)
{
    if (queue == NULL)
        return;
    my_time_t *temp = NULL;
    while (queue->head != NULL)
    {
        temp = queue->head;
        queue->head = queue->head->next;

        free(temp);
        temp = NULL;
        queue->queue_length--;
    }
    free(queue);
    queue = NULL;
}
my_queue *create_queue(int capacity)
{
    my_queue *queue = (my_queue *)malloc(sizeof(my_queue));
    if (queue == NULL)
    {
        printf("malloc my_queue failed\n");
        return NULL;
    }
    queue->head = NULL;
    queue->end = NULL;
    queue->queue_length = 0;
    queue->capacity = capacity;
    return queue;
}

int queue_empty(my_queue *queue)
{

    if (queue == NULL)
        return 1;
    if (queue->queue_length > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
