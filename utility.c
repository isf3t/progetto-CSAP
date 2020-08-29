#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>     
#include <sys/ipc.h>
#include <sys/shm.h> 
#include "utility.h"

#define MAXCHAR 1000
#define NSEMS 1

char* split(char* buf, char delim[]){
    
    printf("sono in split: %s", buf);
    int init_size = strlen(buf);

	char *ptr = strtok(buf, delim);
    
    return ptr;
    
}

int authenticatedUser(char* buffer) {
    FILE *fp;
    char str[MAXCHAR];
    char* filename = "users.txt";
    int found = 0;
    int authenticated = '1';
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s\n",filename);
        exit(1);
    }
    while (fgets(str, MAXCHAR, fp) != NULL){
        printf("Username nel file: %s, username arrivato: %s\n", strtok(str, "\n"), buffer);
        if (strncmp(str, buffer, strlen(buffer)) == 0) {
            if (strchr(str, authenticated) == NULL){
                found = 1;
                break;
            }
            else found = 2;
        }
    }
    fclose(fp);
    
    return found;
}

int sem_init_(key_t key){
    
    int semid; 
    
    /* create a semaphore set with NSEMS semaphore: */         
    if ((semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) < 0) {             
        perror("semget");             
        return -1;
    }         
    /* initialize all semaphore to 0: */   
    if (semctl(semid, 0, SETVAL, 1) < 0) {             
        perror("semctl");             
        return -1;
    } 
    
    printf("id sem nella funzione: %d\n", semid);
    
    return semid;
}

int setSem(int semid, int sem_num){
    
    printf("setto semaforo: %d, con id: %d\n", sem_num, semid);
    
    struct sembuf sop[2];
    
    sop[0].sem_num=sem_num;
	sop[0].sem_op=-1;
    sop[0].sem_flg = 0;
    
	if (semop(semid,sop,1)) {
	    perror("semop");
	    return -1;
	}

    return 0;
}

int resetSem(int semid, int sem_num){
    
    printf("resetto semaforo: %d, con id: %d\n", sem_num, semid);
    
    struct sembuf sop[2];
    
    sop[0].sem_num=sem_num;
	sop[0].sem_op=1;
    sop[0].sem_flg = 0;
    
	if (semop(semid,sop,1)) {
	    perror("semop");
	    return -1;
	}

    return 0;
}
