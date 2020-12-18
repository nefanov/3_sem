#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//void* ptrwr = write;
//void* ptrrd = read;

typedef struct {
	
	int txd[2];
	int rxd[2];
	
	// int (*wr)(int, char *, int) = (int (*)(int, char *, int))ptrwr;
	// int (*rd)(int, char *, int) = (int (*)(int, char *, int))ptrrd;
}

dpipe_t;

int main(){

	char buf[10];
	int size;
	dpipe_t fd;

	if (pipe(fd.txd) < 0)
	{
		printf("First pipe creation is failed!\n");
		return -1;
	}

	if (pipe(fd.rxd) < 0)
		printf("Second pipe creation is failed!\n");

	const pid_t pid = fork();

	if (pid < 0)
	{
		printf("fork failed!\n");
		return -1;
	}

	if (pid)
	{
		cloee(fd.txd[0]);
		close(fd.rxd[1]);
		
		while(1)
		{
			printf("Ready for sending(Parent)\n");
			
			while((size = read(0, buf, sizeof(buf)-1)) > 0)
			{
				buf[size] = '\0'; 

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
				buf[size] = '\0'; 

				printf("Received from parent: %s\n", buf);
				printf("Send to parent: %s\n", buf);
				write(fd.rxd[1], buf, size);
			}
		}
	}

return 0;
}
