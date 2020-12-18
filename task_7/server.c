#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/select.h>
void *thread(void *args)
{
	void ** kek = (void **)args;
	int *desc = (int *)kek[0];
	char * file = (char *)kek[1];
	int fd = open(file, O_RDWR);
	char buf[256];
	int size;
	while ((size = read(fd, buf, sizeof(buf) - 1)) > 0)
	{
		buf[size] = '\0';
		write(*desc, buf, size);
	}	
	close(fd);
}
int main()
{
	char fifonamerx[20] = "server_rx";
	unlink(fifonamerx);
	if (mkfifo(fifonamerx, 0777) == -1)
		printf("Can't make my rx fifo.\n");
	int sv_rx_fifo;
	if ((sv_rx_fifo = open(fifonamerx,O_NONBLOCK|O_RDWR)) == -1)
		perror("kek");
		//printf("Can't open my rx fifo.\n");

	fd_set set_desc_cl;
	struct timeval tv;
	int num_of_desc = 0;
	int maxnumb = 1;
	int desc[64][2];//desc[i][0] - tx, desc[i][1] - rx.
	fd_set set_of_sv;
	int retval;
	
	pthread_t* id;
	id = (pthread_t*)malloc(100 * sizeof(pthread_t));
	int idnumb = 0;

	while(1)
	{
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		FD_ZERO(&set_desc_cl);
		for (int i = 0; i < num_of_desc; i++)
		{
			FD_SET(desc[i][0], &set_desc_cl);
		}
		retval = select(maxnumb, &set_desc_cl, NULL, NULL, &tv);
		if (retval)
		{
			printf("One desc is ready. Reading.\n");
			for (int i = 0; i < num_of_desc; i++)
			{
				if (FD_ISSET(desc[i][0], &set_desc_cl))
				{
					char str[50];
					if (read(desc[i][0], str, sizeof(str) - 1) == -1)
						printf("Can't read from desc %d. \n", i);
					char * h = strtok(str, " ");
					if (strcmp(h, "GET") != 0)
						printf("Bad request\n");
					else
					{
						h = strtok(NULL, " ");
						void * section[2];
						section[1] = h;
						section[0] = &desc[i][1];
						pthread_create(&id[idnumb], NULL, thread, section);
						idnumb++;
					}
				}
			}
		}
		else
			printf("Zero desc with message.\n");
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		FD_ZERO(&set_of_sv);
		FD_SET(sv_rx_fifo, &set_of_sv);
		retval = select(sv_rx_fifo + 1, &set_of_sv, NULL, NULL, &tv);
		if (retval)
		{
			printf("Some client want to connect.\n");
			if (FD_ISSET(sv_rx_fifo, &set_of_sv))
			{
				char str[50];
				if(read(sv_rx_fifo, str, sizeof(str) - 1) == -1)
					printf("Can't receive registration.\n");
				char * h = strtok(str, " ");
				if (strcmp(h, "REGISTER") != 0)
					printf("Bad registration\n");
				else
				{
					h = strtok(NULL, " ");
					desc[num_of_desc][0] = open(h, O_NONBLOCK|O_RDWR);
					h = strtok(NULL, " ");
					desc[num_of_desc][1] = open(h, O_NONBLOCK|O_RDWR);
					FD_SET(desc[num_of_desc][0], &set_desc_cl);
					char msg[5] = "ACK";
					write(desc[num_of_desc][1], msg, 4);
					if (desc[num_of_desc][0] + 1 > maxnumb)
						maxnumb = desc[num_of_desc][0] + 1;
					num_of_desc++;
					printf("Connection was given. %d, %d %d\n", maxnumb, desc[num_of_desc-1][0], desc[num_of_desc-1][1]);

				}
			}
		}
		else
		{
			printf("Zero connecting clients.(retval = %d)\n", retval);
		}
	}
}
