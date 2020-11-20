#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
int main()
{
	int fd = open("testfile", O_RDWR);
	char c[1];
	while (read(fd, c, 1) > 0)
	{
		for (int i = 0; i < 8; i++)
		{
			int a = (c[0] >> (7 - i)) & 1;
		}
	}
}
