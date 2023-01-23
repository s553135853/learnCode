#include <stdio.h>
#include "max96.h"

#define MIN_DATA 4

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

struct interface* getInterfacePtrT()
{
    return &inteface_i;
}

void initContext(struct contetx_light* _context)
{
    _context->age = MIN_DATA;
    #if 0
    struct interface* p = NULL;
    p = &inteface_i;
    p->_add(3,2);
    #endif
    _context->_inter = getInterfacePtrT();
}
