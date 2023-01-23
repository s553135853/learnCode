#include "max96.h"
#include <stdio.h>
extern void test();
extern void initContext(struct contetx_light* _context); 
static struct contetx_light _context={};
int main()
{
    #if 0
    test();
    struct interface* p =getInterfacePtr();
    printf("add result is [%d]\n",p->_add(3,4));
    printf("add reduce is [%d]\n",p->_reduce(3,4));
    #endif
    initContext(&_context);
    printf("context age is [%d]\n",_context.age);
    printf("test add,num 3 + 5 = %d\n",_context._inter->_add(3,5));
}
