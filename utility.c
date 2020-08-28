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
    
    printf("setto i semafori\n");
 
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

int sem_init(key_t key){
    
    int semid; 
    
    /* create a semaphore set with NSEMS semaphore: */         
    if ((semid = semget(key, NSEMS, 0666 | IPC_CREAT)) < 0) {             
        perror("semget");             
        return -1;
    }         
    /* initialize all semaphore to 0: */   
    for (int i = 0; i<NSEMS; i++){
        if (semctl(semid, i, SETVAL, 0) < 0) {             
            perror("semctl");             
            return -1;
        } 
    }
    
    return semid;
}

int setSem(int semid, int sem_num){
    
    struct sembuf sop[1];
    
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
    
    struct sembuf sop[1];
    
    sop[0].sem_num=sem_num;
	sop[0].sem_op=1;
    sop[0].sem_flg = 0;
    
	if (semop(semid,sop,1)) {
	    perror("semop");
	    return -1;
	}

    return 0;
}
