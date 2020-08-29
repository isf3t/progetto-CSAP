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

// int deleteRelatedTopics(int shmid, char* topicName){
//     return 0;
// }
// 
// int deleteRelatedTopics(int shmid, int shmidM, char* threadName, char* username){
// 
//     Topic* headT = (Topic *) shmat(shmid, NULL, 0);
//     Topic* curr;
//     
//     do {
//             
//             if(strcmp(threadName, headT->upperThread) == 0 && strcmp(username, headT->owner) == 0){
//                 
//                 if (deleteRelatedMessage(shmidM, headT->name) < 0) return -1;
//                 curr->next = headT->next;
//                 
//                 if (shmctl(headT->id, IPC_RMID, NULL) < 0) return -1;
//                 
//                 if (headT->next == -1) return 1;
//             
//                 else {
//                     curr = headT;
//                     headT = (Topic *) shmat(headT->next, NULL, 0);
//                     
//                 }
//             }
//             
//             if (headT->next == -1) break;
//             
//             else {
//                 curr = headT;
//                 headT = (Topic *) shmat(headT->next, NULL, 0);
//                 
//             }
//             
//         } while(1);
//         
//     return 1;
//     
// }

int deleteThread(int shmid, int shmidT, int shmidM, char* threadName, char* username){
    
    Thread* headT = (Thread *) shmat(shmid, NULL, 0);
    Thread* curr;
    
    do {
            
            if(strcmp(threadName, headT->name) == 0 && strcmp(username, headT->owner) == 0){
                
//                 if (deleteRelatedTopics(shmidT, shmidM, headT->name, username) < 0) return -1;
                curr->next = headT->next;
                
                if (shmctl(headT->id, IPC_RMID, NULL) < 0) return -1;
                
                if (headT->next == -1) return 1;
            
                else {
                    curr = headT;
                    headT = (Thread *) shmat(headT->next, NULL, 0);
                    
                }
            }
            
            if (headT->next == -1) break;
            
            else {
                curr = headT;
                headT = (Thread *) shmat(headT->next, NULL, 0);
                
            }
            
        } while(1);
        
    return 1;
}

char* printNode(int shmid, char* flag){
    
    if(strcmp(flag, "m") == 0){
        Message* head = (Message *) shmat(shmid, NULL, 0);
    
        do {
            printf("sono nel ciclo\n");
            printf("ecco il messaggio corrente %s, da: %s\n", head->body, head->src);
            
            if (head->next == -1) break;
            
            else {
            
                head = (Message *) shmat(head->next, NULL, 0);
                
            }
            
        } while(1);
    }
    else if (strcmp(flag, "th") == 0){
        printf("sono nei trhread\n");
        Thread* head = (Thread *) shmat(shmid, NULL, 0);
    
        do {
            printf("sono nel ciclo\n");
            printf("ecco il thread corrente %s\n", head->name);
            
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

int addTopic(int shmid, char* threadName, char* topicName, char* username){
    
    Topic* head = (Topic *) shmat(shmid, NULL, 0);
    int numTopic = 0;
    
    printf("threadname e topicname: %s, %s\n", threadName, topicName);
    
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
                    strcpy(new->name, topicName);
                    strcpy(new->owner, username);
                    strcpy(new->upperThread, threadName);
                    new->next = -1;
                    new->id = shmid;
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
    shmdt(head);
    return numTopic;
}

int addThread(int shmid, char* threadName, char* username){
    
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
                    strcpy(new->owner, username);
                    new->next = -1;
                    new->id = shmid;
                    head->next = shmid;
                    shmdt(new);
                    numThread++;
                    break;
                }
            }            
        }
        
        else {
            
            printf("else scorro la lista\n");
            head = (Thread *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
    printf("\n\n");
    shmdt(head);
    return numThread;
}

int addMessage(int shmid, char* topicName, char* body, char* username){
    
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
                    strcpy(new->src, username);
                    new->next = -1;
                    new->id = shmid;
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
    shmdt(head);
    return numMess;
}

char* printMessageList(int shmid, char* topicName, char* buffer){
    
    Message* head = (Message *) shmat(shmid, NULL, 0);

    if (head == NULL) {
        strcpy(buffer, "Nessun Messaggio Trovato!\n");
        return buffer;
    }
    
    do {
        if (strcmp(head->upperTopic, topicName) == 0){

            strcat(buffer, "\n");
            strcat(buffer, "Related To Topic: \n");
            strcat(buffer, head->upperTopic);
            strcat(buffer, "\n");
            strcat(buffer, "SRC: \n");
            strcat(buffer, head->src);
            strcat(buffer, "\n");
            strcat(buffer, "Body: \n");
            strcat(buffer, head->body);
            strcat(buffer, "\n\n");
            printf("buffer nell'if %s\n", buffer);
        }
        
        if (head->next == -1) {
                break;
            }
        
        else {
            head = (Message *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
    shmdt(head);

    return buffer;
    
}

char* printTopicList(int shmid, char* threadName, char* buffer){
    
    Topic* head = (Topic *) shmat(shmid, NULL, 0);
    
    if (head == NULL) {
        strcpy(buffer, "Nessun Topic Trovato!\n");
        return buffer;
    }

    do {
        if (strcmp(head->upperThread, threadName) == 0){
            
            printf("ho trovato il topic: %s con %s\n", head->upperThread, threadName);
            strcat(buffer, "\n");
            strcat(buffer, "Related To Thread: \n");
            strcat(buffer, head->upperThread);
            strcat(buffer, "\n");
            strcat(buffer, "Topic Name: \n");
            strcat(buffer, head->name);
            strcat(buffer, "\n\n");
            
        }
        
        if(head->next == -1) break;
        
        else {
        
            head = (Topic *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
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
    
    if (head == NULL) {
        strcpy(buffer, "Nessun Thread Trovato!\n");
        return buffer;
    }
    
    do {

        strcat(buffer, "\n");
        strcat(buffer, "Thread name: \n");
        strcat(buffer, head->name);
        strcat(buffer, "\n");
        strcat(buffer, "Owner: \n");
        strcat(buffer, head->owner);
        strcat(buffer, "\n\n");
        
        if (head->next == -1) break;
        
        else{
            
            head = (Thread *) shmat(head->next, NULL, 0);
       
            
        }        
        
    }  while(1);
    
   
    
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
    
    struct sembuf sop[1];
    struct shmid_ds shmid_struct;
    int semid;
    
    memset( &sop[0], 0, sizeof( sop[0] ) );
    memset( &shmid_struct, 0, sizeof( shmid_struct ) );
    
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
    headT->id = shmidTOPIC;
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
    headTH->id = shmidTHREAD;
    strcpy(headTH->owner, "admin");
    strcpy(headTH->name, "Welcome!");
    shmdt(headTH);
    
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
    messages = (Message *) shmat(shmidMESSAGE, NULL, 0);
    Message* headM = NULL;
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
    headM = (Message *) shmat(shmidMESSAGE, NULL, 0);
    headM->next = -1;
    headM->id = shmidMESSAGE;
    strcpy(headM->upperTopic, "This is your first topic!");
    strcpy(headM->src, "admin");
    strcpy(headM->body, "This is a test message to show you how it works!");
    shmdt(headM);
    
    // INIT OF SYSV SEMAPHORE
    semid = sem_init_(key_sem);
    if (semid < 0) printf("Error creating semaphore set!\n");
    
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
                    
                    // SET SEM TO LOCK RESURCES
//                     printf("setto i sem\n");
//                     sop[0].sem_num=0;
//                     sop[0].sem_op=-1;
//                     sop[0].sem_flg=0;
//                     
//                     if (semop(semid,sop,1) < 0) {
//                         perror("semop");
//                         exit(1);
//                     }
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");
                    
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
                    
                    // SET SEM TO LOCK RESURCES
//                     sop[0].sem_num=0;
//                     sop[0].sem_op=1;
//                     sop[0].sem_flg = 0;
//                     if (semop(semid,sop,1)) {
//                         perror("semop");
//                         exit(1);
//                     }
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");
                    
                }
                
                if (strcmp(operation, "listM") == 0){
                    
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");
                    
                    bzero(buffer, sizeof(buffer));

                    strcpy(buffer, printMessageList(shmidMESSAGE, payload, buffer));
                    
                    if (strcmp(buffer, "") == 0) strcpy(buffer, "No Message found!");
                    
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");
                    
                    send(newSocket, buffer, strlen(buffer), 0);
                }
                
                if (strcmp(operation, "listT") == 0){
                    
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");
                    
                    bzero(buffer, sizeof(buffer));
                    
                    strcpy(buffer, printTopicList(shmidTOPIC, payload, buffer));
                    
                    if (strcmp(buffer, "") == 0) strcpy(buffer, "No Topic found!");
                    
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");
                    
                    send(newSocket, buffer, strlen(buffer), 0);
                    
                }
                
                if (strcmp(operation, "listTH") == 0){
                    
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");

                    bzero(buffer, sizeof(buffer));

                    strcpy(buffer, printThreadList(shmidTHREAD, buffer));
                    
                    if (strcmp(buffer, "") == 0) strcpy(buffer, "No Thread found!");
                    
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");

                    send(newSocket, buffer, strlen(buffer), 0);
                    
                }
                
                if (strcmp(operation, "replyTO") == 0){
                    
                    
                    int i = 0;
                    char* values = strtok(payload, ",");
                    char body[140];
                    char topicName[50];
                    char username[50];
                    
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");
                    
                    bzero(buffer, sizeof(buffer));
                    
                    while (payload != NULL){
                        
                        if (i == 2){
                            strcpy(username, values);
                            break;
                        }
                        if (i == 1) {
                            strcpy(body, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                        if (i == 0){
                            strcpy(topicName, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                    }
                    
                    int numMess = addMessage(shmidMESSAGE, topicName, body, username);
                    
                    if (numMess > 0){
                        
                        printNode(shmidMESSAGE, "m");
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "ok");
                        send(newSocket, buffer, strlen(buffer), 0);
                        
                    }
                    
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");
                    
                    else printf("Error replying to message!\n");
                }
                
                if (strcmp(operation, "addThread") == 0){
                    
                    int i = 0;
                    char* values = strtok(payload, ",");
                    char threadName[50];
                    char username[50];
                    
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");
                    
                    bzero(buffer, sizeof(buffer));
                    
                    while (payload != NULL){
                        
                        if (i == 1) {
                            strcpy(username, values);
                            break;
                        }
                        if (i == 0){
                            strcpy(threadName, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                    }
                    
                    int numThread = addThread(shmidTHREAD, threadName, username);
                    
                    if (numThread > 0) {
                        printf("printo i Thread\n");
                        printNode(shmidTHREAD, "th");
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "ok");
                        send(newSocket, buffer, strlen(buffer), 0);
                        
                    }
                    
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");
                }
                
                if (strcmp(operation, "addTopic") == 0){
                    
                    int i = 0;
                    char* values = strtok(payload, ",");
                    char threadName[50];
                    char topicName[50];
                    char username[50];
                    
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");
                    
                    bzero(buffer, sizeof(buffer));
                    
                    while (payload != NULL){
                        
                        if (i == 2) {
                            strcpy(username, values);
                            break;
                        }
                        if (i == 1) {
                            strcpy(topicName, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                        if (i == 0){
                            strcpy(threadName, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                    }
                    
                    printf("%s, %s, %s\n", threadName, topicName, username);
                    
                    int numTopic = addTopic(shmidTOPIC, threadName, topicName, username);
                    
                    if (numTopic > 0) {
                       
                        printNode(shmidTOPIC, "t");
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "ok");
                        send(newSocket, buffer, strlen(buffer), 0);
                        
                    }
                    
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");
                }
                
                if (strcmp(operation, "delThread") == 0){
                    
                    printf("richiesta ricevuta\n");
                    int i = 0;
                    char* values = strtok(payload, ",");
                    char threadName[50];
                    char username[50];
                    
                    if (setSem(semid, 0) < 0) printf("ERROR during resources lock!\n");
                    
                    bzero(buffer, sizeof(buffer));
                    
                    while (payload != NULL){
                        
                        if (i == 1) {
                            strcpy(username, values);
                            break;
                        }
                        if (i == 0){
                            strcpy(threadName, values);
                            values = strtok(NULL, ",");
                            i++;
                        }
                    }
                    
                    printf("chiamo la funzione elimina thread \n");
                    
                    if (deleteThread(shmidTHREAD, shmidTOPIC, shmidMESSAGE, threadName, username) >= 0){
                        printf("sono tornato\n");
                        printNode(shmidTHREAD, "th");
                        strcpy(buffer, "ok");
                        send(newSocket, buffer, strlen(buffer), 0);   
                    
                        
                    }
                    if (resetSem(semid, 0) < 0) printf("ERROR during resources unlock!\n");
                }
            }
		}

	}

	close(newSocket);


	return 0;
}
