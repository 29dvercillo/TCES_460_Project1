#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define size 5

int fingers[size];
int rbtfingers[size];
char msg[256]="";
char buffer[256];

void encode() {
	int i =0;
	for (i=0; i<size; i++) {
        	snprintf(msg,sizeof(msg),"%d",fingers[i]);
        	strcat(buffer,msg);
        	strcat(buffer,"R");
	}
}

void error(const char *msg) {
    perror(msg);
    exit(0);
}



int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    fingers[0] = 10;
    fingers[1] = 20;
    fingers[2] = 30;
    fingers[3] = 40;
    fingers[4] = 50;

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    while (1) {
		bzero(buffer,256);
		encode();
		printf("Sending Message: %s\n",buffer);
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) 
			 error("ERROR writing to socket");
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) 
			 error("ERROR reading from socket");
		printf("%s\n",buffer);
		sleep(1);

    }
    close(sockfd);
    return 0;
}
