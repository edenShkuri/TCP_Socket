#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h> 
#include <stdio.h> 

#define SIZE 500
#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "127.0.0.1"

int main()
{
    for(int i=1; i<=10; i++){

        int sock = socket(AF_INET, SOCK_STREAM, 0);

        if(sock == -1)
            printf("Could not create socket : %d" ,errno);

        char buf[256];
        socklen_t len;

        if(i>5){
        strcpy(buf, "reno"); 
        len = strlen(buf);
        if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
        perror("setsockopt"); 
        return -1;
        }
        len = sizeof(buf); 
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
        perror("getsockopt"); 
        return -1; 
        } 
        }

        len = sizeof(buf); 
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
        perror("getsockopt");
        return -1;
        } 
        printf("Current: %s\n", buf); 

        struct sockaddr_in serverAddress;
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(SERVER_PORT);
        int rval = inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
        if (rval <= 0)
        {
        printf("inet_pton() failed");
        return -1;
        }

        if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
        printf("connect() failed with error code : %d" ,errno);
            
        printf("connected to server\n");

        FILE *fp;
        char *filename = "1mb.txt";
        fp = fopen(filename, "r");
        if (fp == NULL) {
            perror("Error in reading file.");
            return -1;
        }

        char data[SIZE] = {'\0'};
        while(fgets(data, SIZE, fp) != NULL) {
            int bytesSent=send(sock, data, SIZE, 0);
            if (bytesSent == -1) {
            perror("Error in sending file.");
            return -1;
            }
            else if (0 == bytesSent)
            printf("peer has closed the TCP connection prior to send().\n");

            else if (sizeof(data) > bytesSent)
            printf("sent only %d bytes from the required %d.\n", sizeof(data), bytesSent);

        bzero(data, SIZE);

        printf("File data number %d sent successfully.\n\n", i);
        fclose(fp);
        }

        sleep(3);
        close(sock);
    }
return 0;
}
