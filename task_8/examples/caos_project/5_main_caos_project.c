#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define TRUE   1
#define _PORT 8888
//#define DEBUG
#define CONN_LIMIT 5
#define MAX_DIFFERENT_CONNS 1024

int msg_lenghts[MAX_DIFFERENT_CONNS];

typedef struct conn_status {
    int all_host_cnt; // number of different connections established
    int current_host_cnt; // current number of connected clients
} CS;

CS cs;

void output_and_exit() {
#ifdef DEBUG
    printf("output_and_exit(): %d conns during the work\n", cs.all_host_cnt);
#endif
    for (int i=0;i<cs.all_host_cnt;i++)
        printf("Client %d: %d\n", i, msg_lenghts[i]);
    exit(0);
    return;
}

int main(int argc , char *argv[])
{
    int opt = TRUE;
    unsigned short PORT = _PORT;
    if (argc > 1)
        PORT = atoi(argv[1]);

    int master_socket , addrlen , new_socket , client_socket[30] ,
            max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[1025];  //data buffer of 1K 

    //set of socket descriptors 
    fd_set readfds;

    //a message 
    char *message = "ECHO Daemon v1.0 \r\n";

    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    printf("Listener on port %d \n", PORT);
#endif
    //specify maximum of CONN_LIMIT pending connections for the master socket
    if (listen(master_socket, CONN_LIMIT) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection 
    addrlen = sizeof(address);
#ifdef DEBUG
    puts("Waiting for connections ...");
#endif
    while(TRUE)
    {
        //clear the socket set 
        FD_ZERO(&readfds);

        //add master socket to set 
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor 
            sd = client_socket[i];

            //if valid socket descriptor then add to read list 
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
#ifdef DEBUG
        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }
#endif
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            //inform user of socket number - used in send and receive commands
#ifdef DEBUG
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                    (address.sin_port));
#endif
            //increment clients numbers
            cs.current_host_cnt++;
            cs.all_host_cnt++;
#ifdef DEBUG
            printf("cs status: %d %d\n", cs.all_host_cnt,cs.current_host_cnt);
#endif

#ifdef DEBUG
            //send new connection greeting message 
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");
#endif
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty 
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
#ifdef DEBUG
                    printf("Adding to list of sockets as %d\n" , i);
#endif
                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the 
                //incoming message
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);
                    cs.current_host_cnt--;
                    printf("Host %d disconnected\n", i);
                    if (cs.current_host_cnt==0) {
                        output_and_exit();
                    }
#ifdef DEBUG
                    printf("Host disconnected , ip %s , port %d \n" ,
                           inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
#endif
                    //Close the socket and mark as 0 in list for reuse 
                    close(sd);
                    client_socket[i] = 0;
                }

                    //Echo back the message that came in
                else
                {
                    msg_lenghts[i] += valread;
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';
#ifdef DEBUG
                    printf("DBG:%s\n", buffer);
#endif
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }

    return 0;
}  