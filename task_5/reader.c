#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
int main()
{
	int fd = open("testfile2", O_RDWR);
	int counter = 0;
	int i;
	int d = 0;
	while ()//priem 1/0
	{
		d += i << (7 - counter); 
		counter++;
		if (counter == 8)
		{
			counter = 0;
			char dd[1];
			dd[0] = d;
			write(fd, dd, 1);
			d = 0;
		}
	}
}
