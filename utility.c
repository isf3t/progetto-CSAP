#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>     
#include <sys/ipc.h>     
#include "utility.h"

#define MAXCHAR 1000
#define NSEMS 3

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

int sem_init(key_t key){
    
    int semid;         
    
    /* create a semaphore set with 1 semaphore: */         
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {             
        perror("semget");             
        return -1;
    }         
    /* initialize semaphore #0 to 1: */         
    for (int i = 0; i<NSEMS; i++){
        if (semctl(semid, i, SETVAL, 1) == -1) {             
            perror("semctl");             
            return -1;
        } 
    }
    
    return semid;
}

