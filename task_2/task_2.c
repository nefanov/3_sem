/*
Write a duplex pipe implementation.
typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;
1) This pipe should connect some process with his child, for continuous communication.
2) Be careful with opened descriptors.
3) Monitor the integrity of transmitted data.
4) When one process is terminated, the other should also exit.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct 
{
  int txd[2]; 
  int rxd[2];
} 
dpipe_t;

int main()
{
	char buf[10];
  	int size;
	dpipe_t fd;
	if (pipe(fd.txd) < 0) 
	{
		printf("First pipe creation is failed!\n");
    	return -1;
  	}
	if (pipe(fd.rxd) < 0)
	{
		printf("Second pipe creation is failed!\n");
	}
  	const pid_t pid = fork();
  	if (pid < 0) 
	{
    	printf("fork failed!\n");
    	return -1;
  	}
  	if (pid) 
	{ //parent transmits the data to child
    	close(fd.txd[0]);
		close(fd.rxd[1]);
		while(1)
		{
    		while((size = read(0, buf, sizeof(buf)-1)) > 0)
			{
				buf[size] = '\0'; // the text string data is expected
      			printf("Send to child: %s\n", buf);
				write(fd.txd[1],buf, size);
				size = read(fd.rxd[0], buf, sizeof(buf) - 1);
				buf[size] = '\0';
				printf("Received from child: %s\n", buf);
			}
		}
  	}
	else 
	{
		close(fd.txd[1]);
		close(fd.rxd[0]);
		while(1)
		{
			while((size = read(fd.txd[0], buf, sizeof(buf)-1)) > 0)
			{
				buf[size] = '\0'; // the text string data is expected
      			printf("Received from parent: %s\n", buf);
				printf("Send to parent: %s\n", buf);
				write(fd.rxd[1], buf, size);
			}
    	}
  	}
return 0;
}
