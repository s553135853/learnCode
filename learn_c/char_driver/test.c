#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h> 



#define MY_DEVICE_MAGIC 'k'
#define MY_IOCTL_SET _IOW(MY_DEVICE_MAGIC, 1, int)
#define MY_IOCTL_GET _IOR(MY_DEVICE_MAGIC, 2, int)
#define FILE_PATH "/dev/test_node"


int main()
{
    int rc;
    int fd;
	int data = 0;
    unsigned char buf[32];
    fd = open(FILE_PATH, O_RDWR);
    if (fd < 0) {
        printf("open file failed!\n");
        return -1;
    } else {
        printf("open success!\n");
    }

	if (ioctl(fd, MY_IOCTL_SET, data) < 0) {
		printf("MY_IOCTL_SET data failed!\n");
	}

	if (ioctl(fd, MY_IOCTL_GET, &data) < 0) {
		printf("MY_IOCTL_GET data failed!\n");
	}

	#if 0
	rc = write(fd, &data, sizeof(int));
	if (rc < 0) {
        printf("write file data failed!\n");
    }

    rc = read(fd, buf, sizeof(int));
    if (rc < 0) {
        printf("read file data failed!\n");
    }

	memcpy(&data, buf, sizeof(int));
	#endif
	printf("data:%d\n", data);

    return 0;
}
