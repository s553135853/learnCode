#include <stdio.h>
#include "max96.h"

static int add(int a,int b)
{
    return a+b;
}
static int reduce(int a,int b)
{
    return a-b;
}

static struct interface inteface_i = 
{
    ._add=&add,
    ._reduce=&reduce
};

static void initContext(struct contetx_light* _context)
{
    
}
struct interface* getInterfacePtr()
{
    return &inteface_i;
}