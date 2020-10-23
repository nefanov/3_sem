#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>

#define FIFO "fifo_example"
int main()
{
  	char buf[1000010];
  	int size;
	int n;
	printf("Choose the size:");
	scanf("%d", &n);
	printf("\n");
  	const pid_t pid = fork();
  	if (pid < 0) 
 	{
    		printf("fork failed!\n");
    		return -1;
  	}
  
  	if (pid) 
  	{ //parent
		size = n;
		for (int i = 0; i < size; i++)
		{
			buf[i] = 'a';
		}
		buf[size] = '\0';

		int time1 = clock();

		mknod(FIFO, S_IFIFO | 0666, 0);
		//printf("Waiting for a reader\n");
		int fd = open(FIFO, O_WRONLY);
		//printf("A reader is connected\n");

		write(fd, buf, size);

		int status;
		waitpid(pid, &status, 0);
		int time2 = clock();
		printf("End time: %d %ld\n", time2-time1, CLOCKS_PER_SEC);
  	}
  	else 
	{ //child
	    	mknod(FIFO, S_IFIFO | 0666, 0);
		//printf("Waiting for a writer\n");
		int fd = open(FIFO, O_RDONLY);
		//printf("A writer is connected\n");

		size = read(fd, buf, sizeof(buf) - 1);
		buf[size] = '\0';
		//printf("Received: %s\n", buf);
  	}
  
  	return 0;
}
