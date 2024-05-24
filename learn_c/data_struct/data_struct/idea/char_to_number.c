#include "func_idea.h"

#define TEN 10


static int my_qurt_for_ten(int size)
{
 
    int result = 1;
    if (size == 0)
    {
        return result;
    }
    else
    {
        for (size_t i = 0; i < size-1; i++)
        {
            result *= TEN;
        }
    }
    return result;
}

int my_atoi(char *value)
{
    __uint8_t data[16];
#if 1
    int count = 0;
    int result = 0;
    while (*value != '\0')
    {
        if(*value >=48 && *value <= 57)
        {
            data[count] = *value - 48;
        }
        else if (*value >=65 && *value <= 70)
        {
            data[count] = *value - 65;
        }
        
        value++;
        count++;
    }
    for (size_t i = 0; i < count; i++)
    {
        printf("%d\n",data[i]);
    }
    
    for (size_t i = count; i > 0; --i)
    {
        result += data[count-i] * my_qurt_for_ten(i);    
    }
    printf("result:%d\n",result);
    return result;
#endif
}