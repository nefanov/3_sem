#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/select.h>
int main()
{
	char fifonamerx[20], fifonametx[20];
	printf("Give me name of fifo.(len<10)\n");
	scanf("%s", fifonamerx);
	strcpy(fifonametx, fifonamerx);
	strcat(fifonametx, "_tx");
	strcat(fifonamerx, "_rx");
	char reg[30] = "REGISTER";
	strcat(reg, " ");
	strcat(reg, fifonametx);
	strcat(reg, " ");
	strcat(reg, fifonamerx);
	printf("%s\n", reg);
	char fifoserver[20] = "server_rx";
	unlink(fifonametx);	
	if (mkfifo(fifonametx, 0777) == -1)
		printf("Can't make tx fifo.\n");
	unlink(fifonamerx);
	if (mkfifo(fifonamerx, 0777) == -1)
		printf("Can't make rx fifo.\n");
	int tx_fifo;
	int rx_fifo;
       	if ((tx_fifo = open(fifonametx, O_NONBLOCK|O_RDWR)) == -1)
		printf("Can't open tx fifo.\n");
	if ((rx_fifo = open(fifonamerx, O_NONBLOCK|O_RDWR)) == -1)
		printf("Can't open rx fifo.\n");

	int sv_rx_fifo;
	if ((sv_rx_fifo = open("server_rx",O_NONBLOCK|O_RDWR)) == -1)
		printf("Can't open rx server fifo.\n");
	if (write(sv_rx_fifo,reg, sizeof(reg)) == -1)
		printf("Can't register. \n");
	
	fd_set sth;
	FD_ZERO(&sth);
	FD_SET(rx_fifo, &sth);
	int retval;
	struct timeval tv;
	tv.tv_sec = 30;
	tv.tv_usec = 0;
	retval = select(rx_fifo + 1, &sth, NULL, NULL, &tv);
	if (retval)
	{
		char ans[10];
		if (read(rx_fifo, ans, sizeof(ans) - 1) == -1)
			printf("Can't receive answer. \n");
		if (strcmp(ans, "ACK") != 0)
			printf("Wrong reaction.\n");
		else
			printf("Receive ACK.\n");
	}
	else
		printf("Didn't receive ACK. \n");
	while(1)
	{
		char message[30] = "GET";
		char hv[20];
		printf("Give me name of file (file1 or file2)\n");
		scanf("%s", hv);
		strcat(message, " ");
		strcat(message, hv);
		if(write(tx_fifo, message, sizeof(message)) == -1)
			perror("lol1");
			//printf("Can't send message.\n");
		int size;
		char buf[256];
		tv.tv_sec = 30;
		tv.tv_usec = 0;
		FD_ZERO(&sth);
		FD_SET(rx_fifo, &sth);
		retval = select(rx_fifo + 1, &sth, NULL, NULL, &tv);
		if (retval)
		{
			while ((size = read(rx_fifo, buf, sizeof(buf) - 1)) > 0)
			{
				buf[size] = '\0';
				printf("%s", buf);
			}
			printf("\n");
		}
		else
			printf("Can't receive file.\n");
	}
}
