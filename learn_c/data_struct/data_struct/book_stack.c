#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char name;
} element_data;

typedef struct
{
    element_data data;
    struct element *next;
} element;

typedef struct
{
    element *bottom;
    element *top;

} book_stack;

int init_stack(book_stack *s);
int destroy_stack(book_stack *s);
int clear_stack(book_stack *s);
int stack_empty(book_stack *s);
int stack_length(book_stack *s);
int get_top(book_stack *s, element **e);
int push_stack(book_stack *s, element *e);
int pop_stack(book_stack *s);
void print_stack(book_stack *s);

int init_stack(book_stack *s)
{
    if (s == NULL)
        return -1;
    s->top = NULL;
    s->bottom = NULL;
    return 0;
}

int destroy_stack(book_stack *s)
{
    if (s == NULL)
        return -1;
    while (s->bottom) {
        s->top = s->bottom->next;
        printf("free id:%d\n", s->bottom->data.id);
        free(s->bottom);
        s->bottom = s->top;
    }
    free(s);
    return 0;
}

int clear_stack(book_stack *s)
{
    if (s == NULL)
        return -1;
    element *e = s->bottom;
    while (e) {
        e->data.id = 0;
        e->data.name = '\0';
        e = e->next;
    }
    return 0;
}

int stack_empty(book_stack *s)
{
    if (s == NULL)
        return -1;
    return s->top == NULL;
}

int stack_length(book_stack *s)
{
    int count = 0;
    if (s == NULL)
        return -1;
    element *temp = s->bottom;
    while (temp) {
        count++;
        temp = temp->next;
    }

    return count;
}

int get_top(book_stack *s, element **e)
{
    if (s == NULL || s->top == NULL)
        return -1;
    *e = s->top;
    return 0;
}

int push_stack(book_stack *s, element *e)
{
    if (s == NULL)
        return -1;
    if (s->top == NULL) {
        s->bottom = e;
        s->top = e;
    } else {
        s->top->next = e;
        s->top = e;
    }
    return 0;
}

int pop_stack(book_stack *s)
{
    if (s == NULL || s->top == NULL)
        return -1;
    element *temp = s->bottom;
    while (temp->next) {
        if (temp->next == s->top) {
            free(s->top);
            s->top = temp;
            s->top->next = NULL;
            return 0;
        }
        temp = temp->next;
    }

    return -1;
}

void print_stack(book_stack *s)
{
    int count = 1;
    if (s == NULL || s->top == NULL)
        return;
    element *temp = s->bottom;
    while (temp) {
        printf("count: %d\n", count++);
        printf("%d, %c\n", temp->data.id, temp->data.name);
        temp = temp->next;
    }
}