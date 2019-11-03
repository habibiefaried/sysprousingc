#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <string.h>
#define MAX 8000
#define PORT 31337

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 

        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, sizeof(buff)); 

        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n'); 
  
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 

int main(){
	int sockfd, connfd; 
	unsigned int len;
	struct sockaddr_in servaddr, cli; 

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

    while (1) {
	    // Accept the data packet from client and verification 
        // Always listening to the port, but blocking mechanism
	    connfd = accept(sockfd, (struct sockaddr *)&cli, &len); 
	    if (connfd < 0) { 
	        printf("server acccept failed...\n"); 
	        exit(0); 
	    } 
	    else
	        printf("server acccept the client...\n"); 
	  
	    // Function for chatting between client and server 
	    func(connfd);     
	}
	
	// After chatting close the socket 
	close(sockfd); 

    return 0;
}