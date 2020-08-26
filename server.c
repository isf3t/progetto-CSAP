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

char* printNode(int shmid, char* flag){
    
    if(strcmp(flag, "m") == 0){
        Message* head = (Message *) shmat(shmid, NULL, 0);
    
        do {
            printf("sono nel ciclo\n");
            printf("ecco il messaggio corrente %s\n", head->body);
            
            if (head->next == -1) break;
            
            else {
            
                head = (Message *) shmat(head->next, NULL, 0);
                
            }
            
        } while(1);
    }
    else if (strcmp(flag, "th") == 0){
        Thread* head = (Thread *) shmat(shmid, NULL, 0);
    
        do {
            printf("sono nel ciclo\n");
            printf("ecco il messaggio corrente %s\n", head->name);
            
            if (head->next == -1) break;
            
            else {
            
                head = (Thread *) shmat(head->next, NULL, 0);
                
            }
            
        } while(1);
    }
    else{
    
        Topic* head = (Topic *) shmat(shmid, NULL, 0);
    
        do {
            printf("sono nel ciclo\n");
            printf("ecco il messaggio corrente %s\n", head->name);
            
            if (head->next == -1) break;
            
            else {
            
                head = (Topic *) shmat(head->next, NULL, 0);
                
            }
            
        } while(1);
        
    }
    
}

int addThread(int shmid, char* threadName){
    
    Thread* head = (Thread *) shmat(shmid, NULL, 0);
    int numThread = 0;
    
    do {
        
        numThread++;
        printf("sono nel ciclo\n");
        if (head->next == -1) {
            printf("aggiungo un nodo\n");
            shmid = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
            
            if (shmid < 0) return -1;
            
            else{
                Thread* new = (Thread *) shmat(shmid, NULL, 0);
                
                if (new == (void*)-1) return -1;
                
                else{
                    strcpy(new->name, threadName);
                    strcpy(new->name, topicName);
                    strcpy(new->owner, "test");
                    new->next = -1;
                    head->next = shmid;
                    shmdt(new);
                    numTopic++;
                    break;
                }
            }            
        }
        
        else {
            
            printf("else scorro la lista\n");
            head = (Topic *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
    printf("\n\n");
    
    return numThread;
}

int addMessage(int shmid, char* topicName, char* body){
    
    Message* head = (Message *) shmat(shmid, NULL, 0);
    int numMess = 0;
    
    do {
        
        numMess++;
        printf("sono nel ciclo\n");
        if (head->next == -1) {
            printf("aggiungo un nodo\n");
            shmid = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
            
            if (shmid < 0) return -1;
            
            else{
                Message* new = (Message *) shmat(shmid, NULL, 0);
                
                if (new == (void*)-1) return -1;
                
                else{
                    strcpy(new->upperTopic, topicName);
                    strcpy(new->body, body);
                    new->next = -1;
                    head->next = shmid;
                    shmdt(new);
                    numMess++;
                    break;
                }
            }            
        }
        
        else {
            
            printf("else scorro la lista\n");
            head = (Message *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
    printf("\n\n");
    
    return numMess;
}

int addTopic(int shmid, char* threadName, char* topicName){
    
    Topic* head = (Topic *) shmat(shmid, NULL, 0);
    int numTopic = 0;
    
    do {
        
        numTopic++;
        printf("sono nel ciclo\n");
        if (head->next == -1) {
            printf("aggiungo un nodo\n");
            shmid = shmget(IPC_PRIVATE, 1 * sizeof(Topic), IPC_CREAT | 0666);
            
            if (shmid < 0) return -1;
            
            else{
                Topic* new = (Topic *) shmat(shmid, NULL, 0);
                
                if (new == (void*)-1) return -1;
                
                else{
                    strcpy(new->upperThread, threadName);
                    strcpy(new->name, topicName);
                    strcpy(new->owner, "test");
                    new->next = -1;
                    head->next = shmid;
                    shmdt(new);
                    numTopic++;
                    break;
                }
            }            
        }
        
        else {
            
            printf("else scorro la lista\n");
            head = (Topic *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
    printf("\n\n");
    
    return numTopic;
}

char* printMessageList(int shmid, char* topicName, char* buffer){
    
    Message* head = (Message *) shmat(shmid, NULL, 0);

    if (head == NULL) strcpy(buffer, "Nessun Topic Trovato!\n");
    else{
        strcpy(buffer, "Nessun Topic Trovato!\n");
    }
    
    do {
        if (strcmp(head->upperTopic, topicName) == 0){
            printf("sono in if\n");
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
        
            if (head->next == -1) {
                printf("sono in if interno\n");
                break;
            }
            
        }
        
        else {
            printf("sono in else\n");
            head = (Message *) shmat(head->next, NULL, 0);
            
        }
        
    } while(head->next != -1);
    
    return buffer;
    
}

char* printTopicList(int shmid, char* threadName, char* buffer){
    
    Topic* head = (Topic *) shmat(shmid, NULL, 0);
    
    if (head == NULL) strcpy(buffer, "Nessun Topic Trovato!\n");
    else{
        strcpy(buffer, "Nessun Topic Trovato!\n");
    }
    
    do {
        if (strcmp(head->upperThread, threadName) == 0){
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "Related To Thread: \n");
            strcat(buffer, head->upperThread);
            strcat(buffer, "\n");
            strcat(buffer, "Topic Name: \n");
            strcat(buffer, head->name);
            strcat(buffer, "\n\n");
        }
        
        else if(head->next == -1) break;
        
        else {
        
            head = (Topic *) shmat(head->next, NULL, 0);
            
        }
        
    } while(head->next != -1);
    
    shmdt(head);
    
    return buffer;
    
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

char* printThreadList(int shmid, char* buffer){
    
    Thread* head = (Thread *) shmat(shmid, NULL, 0);
    
    if (head == NULL) strcpy(buffer, "Nessun Topic Trovato!\n");
    else{
        strcpy(buffer, "Nessun Topic Trovato!\n");
        printf("sono qui %d", head->name);
    }
    
    do {
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "Thread name: \n");
        strcat(buffer, head->name);
        strcat(buffer, "\n");
        strcat(buffer, "Owner: \n");
        strcat(buffer, head->owner);
        strcat(buffer, "\n\n");
        
        if (head->next == -1) break;
        
        else{
            head = (Thread *) shmat(head->next, NULL, 0);
        }        
        
    }  while(head->next != -1);
    
    shmdt(head);
    
    return buffer;
    
}

int main(){
    
    key_t key_shm = ftok("users.txt", 'E'); 
    key_t key_sem = ftok("users.txt", 'J'); 

	int sockfd, ret;
    struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;
    
    Topic* topics;
    Thread* threads;
    Message* messages;
    
    // SHMEM INIT
    shmidTOPIC = shmget(IPC_PRIVATE, 1 * sizeof(Topic), IPC_CREAT | 0666);
    topics = (Topic *) shmat(shmidTOPIC, NULL, 0);
    Topic* headT = NULL;
    shmidTOPIC = shmget(IPC_PRIVATE, 1 * sizeof(Topic), IPC_CREAT | 0666);
    headT = (Topic *) shmat(shmidTOPIC, NULL, 0);
    headT->next = -1;
    strcpy(headT->owner, "admin");
    strcpy(headT->name, "This is your first topic!");
    strcpy(headT->upperThread, "Welcome!");
    shmdt(headT);
    
    shmidTHREAD = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    threads = (Thread *) shmat(shmidTHREAD, NULL, 0);
    Thread* headTH = NULL;
    shmidTHREAD = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    headTH = (Thread *) shmat(shmidTHREAD, NULL, 0);
    headTH->next = -1;
    strcpy(headTH->owner, "admin");
    strcpy(headTH->name, "Welcome!");
    shmdt(headTH);
    
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
    messages = (Message *) shmat(shmidMESSAGE, NULL, 0);
    Message* headM = NULL;
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
    headM = (Message *) shmat(shmidMESSAGE, NULL, 0);
    headM->next = -1;
    strcpy(headM->upperTopic, "This is your first topic!");
    strcpy(headM->src, "admin");
    strcpy(headM->body, "This is a test message to show you how it works!");
    shmdt(headM);
    
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
            printf("sono nel processo figlio\n. Ho ricevuto: %s\n", buffer);
            
            if(strcmp(buffer, ":exit") == 0){
 					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
 					break;
            }else{
                
                int i = 0;
                char* token = strtok(buffer, ":");
                char operation[1024];
                char payload[1024];
                
                printf("OPERAZIONE %s\n", operation);
                
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
                    
                    bzero(buffer, sizeof(buffer));
                    
                    strcpy(buffer, printMessageList(shmidMESSAGE, payload, buffer));

                    send(newSocket, buffer, strlen(buffer), 0);
                }
                
                if (strcmp(operation, "listT") == 0){
                    
                    bzero(buffer, sizeof(buffer));
                    
                    strcpy(buffer, printTopicList(shmidTOPIC, payload, buffer));
                    
                    send(newSocket, buffer, strlen(buffer), 0);
                    
                }
                
                if (strcmp(operation, "listTH") == 0){

                    bzero(buffer, sizeof(buffer));
                    
                    strcpy(buffer, printThreadList(shmidTHREAD, buffer));

                    send(newSocket, buffer, strlen(buffer), 0);
                    
                }
                
                if (strcmp(operation, "replyTO") == 0){
                    
                    
                    int i = 0;
                    char* values = strtok(payload, ",");
                    char body[140];
                    char topicName[50];
                    
                    bzero(buffer, sizeof(buffer));
                    
                    while (payload != NULL){
                        
                        if (i == 1) {
                            strcpy(body, values);
                            break;
                        }
                        if (i == 0){
                            strcpy(topicName, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                    }
                    
                    int numMess = addMessage(shmidMESSAGE, topicName, body);
                    
                    if (numMess > 0){
                        
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "ok");
                        send(newSocket, buffer, strlen(buffer), 0);
                        
                    }
                    
                    else printf("Error replying to message!\n");
                }
            }
		}

	}

	close(newSocket);


	return 0;
}
