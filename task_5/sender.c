#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
int main()
{
	struct timespec tw = {0, 1000000};

	int id;
	printf("Give me id:");
	scanf("%d", &id);
	printf("Thx\n");
	int fd = open("input", O_RDWR);
	char c[1];
	while (read(fd, c, 1) > 0)
	{
		for (int i = 0; i < 8; i++)
		{
			int a = (c[0] >> (7 - i)) & 1;
			//printf("%d", a);
			if (a == 0)
			{
				kill(id, SIGUSR1);
			}
			else
			{
				kill(id, SIGUSR2);
			}
			nanosleep(&tw, NULL);
		}
	}
	kill(id, SIGINT);
}
