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

int main(){
    
    key_t key_shm = ftok("users.txt", 'E'); 
    
    int shmidMESSAGE;

    Message* messages;
    
    // SHMEM INIT
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
    messages = (Message *) shmat(shmidMESSAGE, NULL, 0);
    Message* headM = NULL;
    shmidMESSAGE = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    headM = (Message *) shmat(shmidMESSAGE, NULL, 0);
    headM->next = 0;
    strcpy(headM->src, "admin");
    strcpy(headM->upperTopic, "Welcome to this whiteboard written in C!");
    strcpy(headM->body, "This is your first message of welcome!");
    printf("messaggio %s\n", headM->src);
    shmdt(headM);
//     printf("messaggio %d\n", shmidMESSAGE);
    
    headM = (Message *) shmat(shmidMESSAGE, NULL, 0);
    printf("messaggio %s\n", headM->src);
    return 0;
}
