#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


typedef struct 
{
   clock_t time;     
   struct my_time_t* next;  
}my_time_t;

typedef struct 
{
   my_time_t* head; 
   my_time_t* end;
   size_t queue_length; 
   size_t capacity;
}my_queue;


void dequeue(my_queue* queue,my_time_t* data);
void enqueue(my_queue* queue,my_time_t* add_time);
int get_queue_length(my_queue* queue);
void print_queue(my_queue* queue);
void free_queue(my_queue* queue);
my_queue* create_queue(int capacity);
int queue_empty(my_queue* queue);