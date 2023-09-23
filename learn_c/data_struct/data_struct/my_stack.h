#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


typedef struct
{
    void* value;
    struct my_element* next;
}my_element;


typedef struct 
{
    my_element* head; 
}my_stack;



my_stack* create_stack();
void pop_stack(my_stack* stack,my_element** value);
void push_stack(my_stack* stack,my_element* value);
int  stack_empty(my_stack* stack);
void free_stack(my_stack* stack);
void print_stack(my_stack* stack);
int get_stack_length(my_stack* stack);