#include "export_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int str_copy(char *dest, char *source);
int str_empty(const char *source);
int str_compare(const char *value1, const char *value2);
int str_length(const char *source);
int clear_str(char *source);
char *str_concat(char *dest, char *source);
int sub_str(const char *dest, const char *value);
int str_index(const char *dest, char source);

int str_length(const char *source)
{
    if (source == NULL)
        return -1;
    int count = 0;
    while (source[count] != '\0') {
        count++;
    }
    return count;
}

int str_copy(char *dest, char *source)
{
    if (dest == NULL || source == NULL)
        return -1;
    for (size_t i = 0; i < str_length(source); i++) {
        dest[i] = source[i];
    }
    dest[str_length(source)] = '\0';
    return 0;
}

int str_empty(const char *source)
{
    if (source == NULL)
        return 1;

    return str_length(source) > 0 ? 0 : 1;
}

int clear_str(char *source)
{
    if (source == NULL)
        return -1;
    for (size_t i = 0; i < str_length(source); i++) {
        source[i] = '\0';
    }
    return 0;
}
// 某个字符在dest串中第一次出现的位置
int str_index(const char *dest, char source)
{
    if (dest == NULL)
        return -1;
    for (size_t i = 0; i < str_length(dest) + 1; i++) {
        if (dest[i] == source)
            return i;
    }
    return -1;
}

char *str_concat(char *dest, char *source)
{
    char *return_value = (char *)malloc(str_length(dest) + str_length(source) + 1);
    str_copy(return_value, dest);
    str_copy(&return_value[str_length(dest)], source);
    return_value[str_length(dest) + str_length(source)] = '\0';
    return return_value;
}

int sub_str(const char *dest, const char *value)
{
    if (dest == NULL || value == NULL)
        return -1;

    return 0;
}

int str_compare(const char *value1, const char *value2)
{
    if (value1 == NULL || value2 == NULL)
        return -1;
    if (str_length(value1) > str_length(value2))
        return -1;
    else if (str_length(value1) < str_length(value2))
        return 1;
    int i = 0;
    while (value1[i] != '\0') {
        if (value1[i] > value2[i]) {
            return -1;
        } else if (value1[i] < value2[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}
int main(int argc, char const *argv[])
{
#if 0
// str length
    char* test = "sdwe32";
    printf("%d\n", str_length(test));
#endif
#if 0
    // str empty
    char* test = "sdwe32";
    printf("%d\n", str_empty(test));
#endif
#if 0
    // str clear
    char test[] = {'s', 'd', 'w', 'e', '3', '2'};
    printf("clear before:%s\n", test);
    printf("clear end:%s\n", clear_str(test));
#endif
#if 0
    // str index
    char test[] = {'s', 'd', 'w', 'e', '3', '2','\0'};
    printf("index:%d\n", str_index(test, '\0'));
#endif
#if 0
    //str copy
    char source[] = {'s', 'd', 'w', 'e', '3', '2', '\0'};
    char dest[20];
    str_copy(dest, source);
    printf("%s\n", dest);
#endif
#if 0
    //str concat
    char* source = "ewse1";
    char* dest = "te1s";
    char* result = str_concat(dest, source);
    printf("%s\n", result);
    free(result);
#endif
#if 0
    char *source = "ted";
    char *dest = "te1";
    printf("%d\n", str_compare(dest, source));
#endif

    return 0;
}
