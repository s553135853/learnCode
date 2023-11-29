#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
topic 1:
创建一个程序，提示用户输入字符串，然后输出这个字符串，以及其
中包含的字符数

约束
确保输出中包含原始的字符串。
使用一个输出语句来构造输出。
使用所用编程语言中的一个内置函数来确定字符串的长度。

示例输出
What is the input string? Homer
Homer has 5 characters.

挑战
如果用户什么都没输入，提示用户输入内容
注：需考虑空白字符和回车符的情况

*/

int main(int argc, char const *argv[])
{
// topic 1 code
#if 1
    char rcv[10];
    int s;
    printf("What is the input string?\n");
    do {
        memset(rcv, 0, sizeof(rcv));
        if (fgets(rcv, sizeof(rcv), stdin) == NULL) {
            return -1;
        }
        size_t len = strlen(rcv);

        if (len > 0 && (strspn(rcv[len - 1], "\t\n\r\f\v")) <= 0) {
            printf("len:%d, strings%s", len, rcv);
            break;
        }
        printf("input somethings\n");
    } while (1);

    printf("%d:%s\n", strlen(rcv) - 1, rcv);
#endif

    return 0;
}
