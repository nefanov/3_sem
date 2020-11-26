#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int i;
int flag = 0;
void sig_handler(int signo)
{
	if (signo == SIGUSR1)
	{
		i = 0;
		flag = 1;
	}
	if (signo == SIGUSR2)
	{
		i = 1;
		flag = 1;
	}
}

int main()
{
	printf("My pid is %d\n", getpid());
	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
	{
		printf("Can't catch SIGUSR1\n");
	}
	if(signal(SIGUSR2, sig_handler) == SIG_ERR)
	{
		printf("Can't catch SIGUSR2\n");
	}
	int d = 0;
	int counter = 0;
	int fd = open("output", O_RDWR);
	while (1)//priem 1/0
	{
		if (flag == 1)
		{
			flag = 0;
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
}
