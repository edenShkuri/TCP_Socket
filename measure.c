#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#define SERVER_PORT 1064

int main()
{
int server_socket = socket(AF_INET,SOCK_STREAM,0);
if(server_socket == -1){
    printf("ERR! - invalid socket");
    return -1;
}

struct sockaddr_in server_address;
memset(&server_address,0,sizeof(server_address));


server_address.sin_family = AF_INET;
server_address.sin_port = htons(SERVER_PORT);
server_address.sin_addr.s_addr = INADDR_ANY;

int bind_res = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
if(bind_res ==-1){
    printf("ERR! bind failed\n");
    return -1;
}
printf("bind() success!\nWaiting for signal from Client..\n");

if(listen(server_socket,5) == -1){
    printf("Err! - listen failed\n");
    return -1;
}
struct sockaddr_in client_address;
socklen_t client_addressLen = sizeof(client_address);

struct timeval t1, t2;
double TotalTime;

int t=1;
while(1)
{
    memset(&client_address,0,sizeof(client_address));
    client_addressLen = sizeof(client_address);
    int client_socket = accept(server_socket,(struct sockadrr *)&client_address,&client_addressLen); 
    if(client_socket == -1){
        printf("ERR! - failed to connect!\n");
        return -1;
    }
    printf("Good news! new connection accepted!\n\n");

    char buffer[256];
  //  int bufferLen = sizeof(buffer);

    gettimeofday(&t1, NULL);
    // for(int i=1; i<=5; i++)
    // {
    int reallyRecived = recv(client_socket, buffer, 256, 0);
   
    if(reallyRecived == -1){
        printf("ERR! - nothing has been sent!\n");
    }
    else{
        printf("Message was successfully recived!\n\n");
        t=0;
       // printf("%s\n",buffer);
        //puts(msg);
     }

    bzero(buffer, 256);
    //}
    gettimeofday(&t2, NULL);
    TotalTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    TotalTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("the time for reciving 5 messages-> %f ms\n\n", TotalTime);
    printf("the average time for each message-> %f ms\n", (TotalTime/5));
} 
close(server_socket);
return 0; 
}