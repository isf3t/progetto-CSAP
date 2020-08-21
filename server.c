#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>     
#include <sys/sem.h>
#include "utility.h"

#define PORT 4446

int main(){
    
    key_t key = ftok("users.txt", 'E'); 

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;
    
    if (sem_init(key) < 0) printf("Error creating semaphore set!\n");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
            
            recv(newSocket, buffer, 1024, 0);
            printf("sono nel processo figlio\n. Ho ricevuto: %s\n", buffer);
            
            if(strcmp(buffer, ":exit") == 0){
 					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
 					break;
            }else{
                
                int i = 0;
                char* token = strtok(buffer, ":");
                char operation[1024];
                char username[1024];
                
                while (token != NULL){
                    if (i == 1) {
                        strcpy(username, token);
                        break;
                    }
                    else if (i == 0){
                        strcpy(operation, token);
                        token = strtok(NULL, ":");
                        i++;
                    }
                }
                
                if (strcmp(operation, "username") == 0){
                    
                    printf("Cerco di autenticare...%s\n", username);
                
                    int res = authenticatedUser(username);
                
                    if (res == 1) {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "authenticated");
                        send(newSocket, buffer, strlen(buffer), 0);
                    }
                    else if (res == 2) {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "alreadyAuthenticated");
                        send(newSocket, buffer, strlen(buffer), 0);
                    }
                    else{
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "unautorized");
                        send(newSocket, buffer, strlen(buffer), 0);
                    }
                    
                    printf("Ho inviato al client %s\n", buffer);
                    
                    bzero(buffer, sizeof(buffer));
                }
                
//                 printf("Client: %s\n", buffer);
//  					send(newSocket, buffer, strlen(buffer), 0);
//  					bzero(buffer, sizeof(buffer));
//  				}
// 			while(1){
// 				recv(newSocket, buffer, 1024, 0);
// 				if(strcmp(buffer, ":exit") == 0){
// 					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
// 					break;
// 				}else{
// 					printf("Client: %s\n", buffer);
// 					send(newSocket, buffer, strlen(buffer), 0);
// 					bzero(buffer, sizeof(buffer));
// 				}
// 			}
                
            }
		}

	}

	close(newSocket);


	return 0;
}
