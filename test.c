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

void printNode(int shmid){
    
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

int sem_init(key_t key){
    
    int semid; 
    
    /* create a semaphore set with NSEMS semaphore: */         
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {             
        perror("semget");             
        return -1;
    }         

    for (int i = 0; i<1; i++){
        if (semctl(semid, i, SETVAL, 1) == -1) {             
            perror("semctl");             
            return -1;
        } 
    }
    
    return semid;
}

void addNode(int shmid){
    
    Thread* head = (Thread *) shmat(shmid, NULL, 0);
    
    do {
        printf("sono nel ciclo\n");
        if (head->next == -1) {
            printf("aggiungo un nodo\n");
            shmid = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
            Thread* new = (Thread *) shmat(shmid, NULL, 0);
            strcpy(new->name, "Questo + un nuovo topic di test\n");
            strcpy(new->owner, "isfet");
            new->next = -1;
            head->next = shmid;
            shmdt(new);
            break;
        }
        
        else {
            printf("else scorro la lista\n");
            head = (Thread *) shmat(head->next, NULL, 0);
            
        }
        
    } while(head->next != -1);
}

int main(){
    
    key_t key_shm = ftok("users.txt", 'E'); 
    
    int shmid;

    Thread* threads;
    
    int id = sem_init(key_shm);
    printf("%d\n",id);
    
    // SHMEM INIT
    shmid = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    threads = (Thread *) shmat(shmid, NULL, 0);
    Thread* headTH = NULL;
    shmid = shmget(IPC_PRIVATE, 1 * sizeof(Thread), IPC_CREAT | 0666);
    headTH = (Thread *) shmat(shmid, NULL, 0);
    headTH->next = -1;
    strcpy(headTH->owner, "admin");
    strcpy(headTH->name, "Welcome!");
    shmdt(headTH);
    
    headTH = (Thread *) shmat(shmid, NULL, 0);
    printf("messaggio %s, id %d\n", headTH->name, shmid);
    shmdt(headTH);
    
    addNode(shmid);
    
    printNode(shmid);
    return 0;
}
