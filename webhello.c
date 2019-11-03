#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <string.h>
#define MAX 32000
#define PORT 31337
#define BUFSIZE 8096

// Function designed for chat between client and server. 
void func(int sockfd, int traffic) 
{ 
    char buff[MAX];
    bzero(buff, strlen(buff)); 

    // read the message from client and copy it in buffer 
    read(sockfd, buff, sizeof(buff)); 
    
    // print buffer which contains the client contents 
    printf("(%d) From client: %s\n", traffic, buff); 
    
    //reuse the variable
    bzero(buff, strlen(buff));     
    //Example based on IETF for HTTP 200 responses: https://www.tutorialspoint.com/http/http_responses.htm
    strcpy(buff,"HTTP/1.1 200 OK\nServer: HelloWorldServer\nContent-Length: 51\nContent-Type: text/html\nConnection: Closed\n\n<html>\n<body><h1>Hello, World!</h1>\n</body></html>\n");
    write(sockfd, buff, strlen(buff)); 
} 

int main(){
	int sockfd, connfd; 
	unsigned int len;
	struct sockaddr_in servaddr, cli; 

    /* Optional if you want this process to be zombie running on background
       But if you do so, you are not able to reply the message from STDIN 
    if (fork() != 0)
        return 0;
    /* */

    // socket create and verification 
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 

    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification 
    if ((listen(sockfd, 64)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 

    int traffic;
    for (traffic=1; ;traffic++) {
	    // Accept the data packet from client and verification 
	    connfd = accept(sockfd, (struct sockaddr *)&cli, &len); 
	    if (connfd < 0) { 
	        printf("server acccept failed...\n"); 
	        exit(0); 
	    } 
	    else
	        printf("server acccept the client at traffic %d...\n",traffic); 
	  
        // Forks process after accept
        int pid = fork(); //
        if (pid < 0){
            printf("Error while forking");
        } else if (pid == 0){ //Child
            close(sockfd);
            func(connfd, traffic);  
        } else { //Parent
            close(connfd);
        }
	}
	
	// After chatting close the socket 
	close(sockfd); 

    return 0;
}