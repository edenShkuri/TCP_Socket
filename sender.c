#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> 
#include <stdio.h>

#define SERVER_PORT 1064
#define SERVER_IP_ADDRESS "127.0.0.1"


int main()
{
int sock = socket(AF_INET,SOCK_STREAM,0);
if(sock == -1){
    printf("ERR! - invalid socket");
    return -1;
}

struct sockaddr_in server_address;
server_address.sin_family = AF_INET;
server_address.sin_port = htons(SERVER_PORT);
int rval = inet_pton(AF_INET,(const char*)SERVER_IP_ADDRESS,&server_address.sin_addr);
if(rval<=0){
    printf("ERR! - invalid ip\n");
    return -1;
}


int c = connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)); 
if(c == -1){
    printf("ERR! - invalid connection");
    return -1;
}   
printf("You are connect to the amazing server!\n");

char msg[] ="Yosef is the man!";
int msgLen = strlen(msg);

int reallySent = send(sock,msg,msgLen,0);

if(reallySent == -1){
    printf("ERR! - nothing has been sent!\n");
}
else if (reallySent < msgLen){
    printf("ERR! - there is bytes there not sent!\n");
    printf("SENT: %d, FULL MESSAGE: %d\n",reallySent,msgLen);
}
else{
    printf("Message was successfully sent.\n");
    printf("Message that sent:\n");
    printf("%s\n",msg);
}

sleep(3);
close(sock);
return 0;

}
