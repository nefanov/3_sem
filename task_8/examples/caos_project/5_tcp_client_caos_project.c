#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 89
#define _PORT 8888
//#define DEBUG
#define SA struct sockaddr

void routine(int sockfd, unsigned short port)
{
    unsigned short PORT = port;
    
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
#ifdef DEBUG:
        printf("Enter the string : ");
#endif
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, strlen(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
#ifdef DEBUG:
        printf("From Server : %s", buff);
#endif
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
  
int main(int argc, char *argv[])
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    unsigned short PORT = _PORT;
 
    if (argc>1)
        PORT = atoi(argv[1]);
 
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
  
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
  
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
  
    // function for chat

    routine(sockfd, PORT);
  
    // close the socket
    close(sockfd);
}
