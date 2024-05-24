#include <stdio.h>
#include "my_stack.h"
#include <stdlib.h>
#include <string.h>
#include "func_idea.h"
typedef struct
{
    int  age;
    char *name;
}User;




int main()
{   
    #if 0
    my_stack* stack = create_stack();
    my_element *element =(my_element *)calloc(2,sizeof(my_element));
    if(element == NULL)
        return 0;
    User* users = (User *)calloc(2,sizeof(User));
    if(users == NULL)
        return 0;
    users[0].age = 10;
    users[0].name = "aaaa";
    element[0].value = (void *)&users[0];
    element[0].next = NULL;

    users[1].age = 12;
    users[1].name = "bbbb";
    element[1].value = (void *)&users[1];
    element[1].next = NULL;



    push_stack(stack,&element[0]);
    push_stack(stack,&element[1]);
    my_element *element2 = NULL; 
    pop_stack(stack,&element2); 
    if(element2 == NULL)
    {
        printf("element2 is NULL\n");
        return 0;
    }
    else
    {
        User* user2 =  NULL;
        user2 = (User *)element2->value;
        if(user2 != NULL)
            printf("name:%s\n",user2->name);
    }
    #endif
    char temp[] = {'2','1'};
    my_atoi(temp);

    #if 0
    free(users);
    free(element2);
    free(element);
    free(stack);
    #endif

    
}

