#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define SIZE 1024
#define SERVER_PORT 1064
#define SERVER_IP_ADDRESS "127.0.0.1"

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("Error in sending file.");
      return;
    }
    bzero(data, SIZE);
  }
}

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
printf("You are connect to the amazing server!\n\n");

FILE *fp;
char *filename = "1mb.txt";

for(int i=1; i<=5; i++){
fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error in reading file.");
    return -1;
  }

send_file(fp, sock);
printf("File data number %d sent successfully.\n", i);

}

sleep(3);
close(sock);
return 0;

}
