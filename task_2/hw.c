#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;

int main()
{
	char buf[4096];
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
			printf("Ready for sending(Parent)\n");
    			size = read(0, buf, sizeof(buf)-1); 
      			buf[size] = '\0'; // the text string data is expected
      			write(fd.txd[1], buf, size);
      			printf("Send: %s\n", buf);
			size = read(fd.rxd[0], buf, sizeof(buf) - 1);
			buf[size] = '\0';
			printf("Received from child: %s\n", buf);

		}
  	}
	else 
	{
		close(fd.txd[1]);
		close(fd.rxd[0]);
		while(1)
		{
			size = read(fd.txd[0], buf, sizeof(buf)-1);
      			buf[size] = '\0'; // the text string data is expected
      			printf("Received from parent: %s\n", buf);
			printf("Ready for sending(Child)\n");
			size = read(0, buf, sizeof(buf) - 1);
			buf[size] = '\0';
			write(fd.rxd[1], buf, size);
			printf("Send: %s\n", buf);
    		}
  	}
  return 0;
}
