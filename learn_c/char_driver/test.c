#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int rc;
	char buf[32];
	int fd = open("/dev/test_node", O_RDWR);
	if (fd < 0) {
		printf("open file failed!\n");
		return -1;
	}
	else {
		printf("open success!\n");
	}
	return 0;
}
