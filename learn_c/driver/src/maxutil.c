#include<stdio.h>


static void test_internal()
{
    printf("call fun is [%s]\n",__FILE__);
}

void test()
{
    test_internal();
}




