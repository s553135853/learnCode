#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FILE_PATH "/dev/misc_test"

int main(int argc, char const *argv[])
{
    int fd;
    int rc;
    int data;
    unsigned char buf[10];
    fd = open(FILE_PATH, O_RDWR);
    if (fd < 0) {
        printf("open %s failed\n", FILE_PATH);
    }

    while (1) {
        rc = read(fd, buf, sizeof(int));
        if (rc < 0) {
            printf("read %s failed\n", FILE_PATH);
        }
        memcpy(&data, buf, sizeof(int));
        printf("%d\n", data);
        sleep(1);
    }

    
    

    return 0;
}
