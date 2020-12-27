
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define SERVER_PORT 5060  
  
int main()
{
int i=1;
double sumCubic=0, sumReno=0;
signal(SIGPIPE, SIG_IGN); 

int listeningSocket = -1;  
	 
if((listeningSocket = socket(AF_INET , SOCK_STREAM , 0 )) == -1) 
   printf("Could not create listening socket : %d" ,errno);
 
int enableReuse = 1;
if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) < 0)
   printf("setsockopt() failed with error code : %d" , errno);

struct sockaddr_in serverAddress;
memset(&serverAddress, 0, sizeof(serverAddress));

serverAddress.sin_family = AF_INET;
serverAddress.sin_addr.s_addr = INADDR_ANY;
serverAddress.sin_port = htons(SERVER_PORT);  
    
if (bind(listeningSocket, (struct sockaddr *)&serverAddress , sizeof(serverAddress)) == -1)
{   
  printf("Bind failed with error code : %d" , errno);
  return -1;
}
      
printf("Bind() success\n");
  
if (listen(listeningSocket, 500) == -1) 
{
  printf("listen() failed with error code : %d", errno);
  return -1;
}
      

printf("Waiting for connections...\n");
      
struct sockaddr_in clientAddress;  
socklen_t clientAddressLen = sizeof(clientAddress);

struct timeval t1, t2;
double TotalTime;

    while (1)
    {

        memset(&clientAddress, 0, sizeof(clientAddress));
        clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (clientSocket == -1)
        {
            printf("listen failed with error code : %d", errno);
            return -1;
        }
        if(i<6)
            printf("Current: Cubic\n");
        else
            printf("Current: Reno\n");        
        printf("A new client connection accepted -> ");
        gettimeofday(&t1, NULL);
        char buffer[256]={0};
        int recived = recv(clientSocket, buffer, 256, 0);

        if (-1 == recived)
            printf("Error in reciving the file\n");
        else if(recived==0)
            printf("nothing recived\n ");
        else
            printf("message %d recived succesfully\n\n", i); 

        gettimeofday(&t2, NULL);
        TotalTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        TotalTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        if(i<6)
            sumCubic+=TotalTime;
        else
            sumReno+=TotalTime;  

        i++; 

        if(i==6)
            printf("the avg for Cubic is %lf ms\n\n", (sumCubic/5.0));
            
        if(i==11)  
	{
                printf("the avg for Reno is %lf ms\n\n", (sumReno/5.0));
		break;
	}
    }

    close(listeningSocket);
    return 0;
}
