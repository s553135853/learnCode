#include "my_stack.h"

my_stack* create_stack()
{
    my_stack* stack = (my_stack*)calloc(1,sizeof(my_stack));
    if(stack == NULL)
    {
        printf("create stack failed\n");
        return NULL;
    }
    return stack;
}
void push_stack(my_stack* stack,my_element* value)
{
    if(stack == NULL) 
        return;   
    if(stack->head = NULL)
    {
        stack->head  = value;       
    }
    else
    {
        value->next = stack->head;    
        stack->head = value; 
    }
}

void pop_stack(my_stack* stack,my_element** value)
{
    if(stack == NULL || stack->head == NULL) 
    {
        printf("stack or head is null\n");
        return;
    } 
    *value = stack->head;
    stack->head = stack->head->next;
    
}

void free_stack(my_stack* stack)
{
    if (stack == NULL)
        return; 
    my_stack* temp = NULL;
    while (stack->head != NULL)
    {
        temp = stack->head;
        stack->head = stack->head->next;
        free(temp);
        temp =NULL;
    }
    free(stack);
    stack = NULL;
}

void print_stack(my_stack* stack)
{
    if(stack == NULL)
        return; 
    my_stack* temp = NULL;
    temp = stack->head;
    while (stack->head != NULL)
    {
        printf("address:%p\n",stack->head->value);
        stack->head = stack->head->next;
    }
    stack->head = temp;
}


int get_stack_length(my_stack* stack)
{
    if(stack == NULL)
        return 0; 
    int size = 0;    
    my_stack* temp = NULL;
    temp = stack->head;
    while (stack->head != NULL)
    {
        size++;
        stack->head = stack->head->next;
    }
    stack->head = temp;
    return size;
}