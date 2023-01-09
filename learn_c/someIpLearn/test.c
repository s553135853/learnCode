#include <stdio.h>
#include <pthread.h>



struct Data_source
{
    int flag;
    const unsigned char* _data;
};
struct Queue_my
{
    struct Data_source data_str;
    struct Queue_my* next;
};

static struct Queue_my* _queue;
static struct Queue_my _elementOne;
static struct Queue_my _elementTwo;



void initQueue()
{
    _elementOne.data_str.flag = 0;
    _elementOne.data_str._data = NULL;
    _queue = &_elementOne;
    _elementTwo.data_str.flag = 0;
    _elementTwo.data_str._data = NULL;
    _queue->next = &_elementTwo;
}

size_t sizeQueue(struct Queue_my* _value)
{
    size_t count = 0;
    while (_value != NULL)
    {
        count ++;
        _value = _value->next;
    }
    return count;  
}

void showData(struct Queue_my* _value)
{
    while (_value != NULL)
    {
        printf("flag is [%d]\n",_value->data_str.flag);
        _value = _value->next;
    }
    
}

struct Queue_my* front()
{
    return _queue;
}
struct Queue_my* back()
{
    return _queue->next;
}


void push_back(int flag,const unsigned char* _data, struct Queue_my** souere)
{
    struct Queue_my* last =back();
    struct Queue_my* _head =front();
    struct Queue_my* _temp = NULL;
    if(!_head->data_str.flag)
    {
        _head->data_str.flag=flag;
        _head->data_str._data = NULL;
        _head->next = NULL;
        last->next=_head;
        *souere = last;
    }
    else
    {
        last->data_str.flag=flag;
        last->data_str._data = _data;
    }
}

void tryData()
{
    printf("enter thread\n");
    while (1)
    {
       sleep(1);
       push_back(0,NULL,&_queue);
       showData(_queue);
       printf("receive data\n");
    }
    
}


int main()
{
    initQueue();
    showData(_queue);
    pthread_t _pid;
    int ret=-1;
    ret = pthread_create(&_pid,NULL,(void *)tryData,NULL);
    sleep(15);
     _queue->data_str.flag=1;
    sleep(100);
}