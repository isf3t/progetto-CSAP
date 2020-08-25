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
#include <sys/shm.h> 
#include "utility.h"
#include "data_structure.h"

#define PORT 4444

// SHMIDS FOR PROCESS
int shmidTOPIC;
int shmidTHREAD;
int shmidMESSAGE;
char buffer[1024];

void printMessageList(char* topicName){
//     shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
    Message* head = (Message *) shmat(shmidMESSAGE, NULL, 0);
    
    if (head == NULL) strcpy(buffer, "Nessun Messaggio Trovato!\n");
    else{
        strcpy(buffer, "Nessun Messaggio Trovato!\n");
    }
    
    while(head != NULL){
        
        if (strcmp(head->upperTopic, topicName) == 0){
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "Related To Topic: \n");
            strcat(buffer, head->upperTopic);
            strcat(buffer, "\n");
            strcat(buffer, "SRC: \n");
            strcat(buffer, head->src);
            strcat(buffer, "\n");
            strcat(buffer, "Body: \n");
            strcat(buffer, head->body);
            strcat(buffer, "\n\n");
        }
        
        else {
        
            head = head->next;
            
        }
    }
    
}

void printTopicList(Topic* head, char* threadName){
    
    if (head == NULL) strcpy(buffer, "Nessun Topic Trovato!\n");
    else{
        strcpy(buffer, "Nessun Topic Trovato!\n");
    }
    
    while(head != NULL){
        
        if (strcmp(head->upperThread, threadName) == 0){
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "Related To Thread: \n");
            strcat(buffer, head->upperThread);
            strcat(buffer, "\n");
            strcat(buffer, "Topic Name: \n");
            strcat(buffer, head->name);
            strcat(buffer, "\n\n");
        }
        
        else {
        
            head = head->next;
            
        }
    }
    
}

// VARIANTE 1
// void printTopicList(Thread* head, char* topicName, char* threadName){
//     
//     if (head == NULL) strcpy(buffer, "Nessun Thread Trovato\n");
//     
//     while(head != NULL){
//         
//         if (strcmp(head->name, threadName) == 0){
//             
//             if (head->topics == NULL) {
//                 
//                 strcat(buffer, "Nessun Topic Trovato\n");
//                 break;
//             }
//             
//             while(head->topics != NULL){
//                 strcat(buffer, "Topic Name: \n");
//                 strcat(buffer, head->topics->name);
//                 strcat(buffer, "\n\n");
//                 head->topics = head->topics->next;
//             }
//         }
//         
//     }    
// }

void printThreadList(Thread* head){
    
    printf("sono qui");
    
    if (head == NULL) strcpy(buffer, "Nessun Thread Trovato");
    else{
        strcpy(buffer, "Nessun Topic Trovato!\n");
    }
    
    while(head != NULL){
        printf("sono nel ciclo");
        bzero(buffer, sizeof(buffer));
        strcat(buffer, "Thread Name: \n");
        strcat(buffer, head->name);
        strcat(buffer, "\n");
        strcat(buffer, "Thread Owner: \n");
        strcat(buffer, head->owner);
        strcat(buffer, "\n\n");
        head = head->next;
        
    }    
    
    printf("buffer dopo il ciclo %s\n", buffer);
}

int main(){
    
    key_t key_shm = ftok("users.txt", 'E'); 
    key_t key_sem = ftok("users.txt", 'J'); 

	int sockfd, ret;
    struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	pid_t childpid;
    
    Topic* topics;
    Thread* threads;
    Message* messages;
    
    // SHMEM INIT
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
    messages = (Message *) shmat(shmidMESSAGE, NULL, 0);
    Message* headM = NULL;
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    headM = (Message *) shmat(shmidMESSAGE, NULL, 0);
    headM->next = NULL;
    strcpy(headM->src, "admin");
    strcpy(headM->upperTopic, "Welcome to this whiteboard written in C!");
    strcpy(headM->body, "This is your first message of welcome!");
    shmdt(headM);
    
    shmidTHREAD = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    threads = (Thread *) shmat(shmidTHREAD, NULL, 0);
    Thread* headTH = NULL;
    shmidTHREAD = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    headTH = (Thread *) shmat(shmidTHREAD, NULL, 0);
    headTH->next = NULL;
    strcpy(headTH->name, "Welcome!");
    strcpy(headTH->owner, "admin");
    shmdt(headTH);
    
    shmidTOPIC = shmget(IPC_PRIVATE, 1 * sizeof(Topic), IPC_CREAT | 0666);
    topics = (Topic *) shmat(shmidTOPIC, NULL, 0);
    Topic* headT = NULL;
    shmidTOPIC = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    headT = (Topic *) shmat(shmidTOPIC, NULL, 0);
    headT->next = NULL;
    strcpy(headT->name, "Welcome to this whiteboard written in C!");
    strcpy(headT->upperThread, headTH->name);
    shmdt(headT);

    // INIT OF SYSV SEMAPHORE
    if (sem_init(key_sem) < 0) printf("Error creating semaphore set!\n");
    
    // SERVER SOCKET INIT
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

    // MULTIPROCESS CONNECTION
	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
            
            recv(newSocket, buffer, 1024, 0);
//             printf("sono nel processo figlio\n. Ho ricevuto: %s\n", buffer);
            
            if(strcmp(buffer, ":exit") == 0){
 					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
 					break;
            }else{
                
                int i = 0;
                char* token = strtok(buffer, ":");
                char operation[1024];
                char payload[1024];
                
//                 printf("OPERAZIONE %s\n", operation);
                
                 while (token != NULL){
                        if (i == 1) {
                            strcpy(payload, token);
                            break;
                        }
                        else if (i == 0){
                            strcpy(operation, token);
                            token = strtok(NULL, ":");
                            i++;
                        }
                    }
                
                if (strcmp(operation, "username") == 0){
                    
                    printf("Cerco di autenticare...%s\n", payload);
                
                    int res = authenticatedUser(payload);
                
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
                    
//                     printf("Ho inviato al client %s\n", buffer);
                    
                    bzero(buffer, sizeof(buffer));
                }
                
                if (strcmp(operation, "listM") == 0){
                    
                    int i = 0;
                    char* values = strtok(payload, ",");
                    char threadName[50];
                    char topicName[50];
                    
                    bzero(buffer, sizeof(buffer));
                    
                    while (payload != NULL){
                        
                        if (i == 1) {
                            strcpy(topicName, values);
                            break;
                        }
                        if (i == 0){
                            strcpy(threadName, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                    }
                    
                    
                    printf("%d", shmidMESSAGE);
                    printMessageList(topicName);
                    send(newSocket, buffer, strlen(buffer), 0);
                }
                
                if (strcmp(operation, "listT") == 0){
                    
                    bzero(buffer, sizeof(buffer));
                    
                    printf("%d\n", shmidTOPIC);
                    
                        headT = (Topic *) shmat(shmidTOPIC, NULL, 0);

                    printTopicList(headT, payload);
                    printf("buffer %s\n", buffer);
                    send(newSocket, buffer, strlen(buffer), 0);
                    
                }
                
                if (strcmp(operation, "listTH") == 0){

                    bzero(buffer, sizeof(buffer));
                    
                    headTH = (Thread *) shmat(shmidTHREAD, NULL, 0);
                    
                    printThreadList(headTH);
                    
                    printf("buffer dopo la funzione %s\n", buffer);
                    send(newSocket, buffer, strlen(buffer), 0);
                }
                
                if (strcmp(operation, "replyTO") == 0){
                    printf("richiesta di reply ricevuta");
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
