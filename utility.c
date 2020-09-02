#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>     
#include <sys/ipc.h>
#include <sys/shm.h> 
#include "utility.h"
#include "data_structure.h"

#define MAXCHAR 1000
#define NSEMS 1

void initUserList(int shmid){

    char username[50];
    
    printf("Insert username to add new user list. Insert exit to stop process!\n");
    
    do {
        
        printf("Insert username: \n");
        scanf("%s", &username[0]);
        printf("\n\n");
        
        if (strcmp(username, "exit") == 0 || strcmp(username, "Exit") == 0 || strcmp(username, "e") == 0) break;
        
        else if (containsNode(shmid, "u", username) == 0) printf("User already exist!\n");
        
        else {
            
            Users* head = (Users *) shmat(shmid, NULL, 0);
    
            do {
                
                if (head->next == -1) {
                    
                    shmid = shmget(IPC_PRIVATE, 1 * sizeof(Message), IPC_CREAT | 0666);
                    
                    if (shmid < 0) exit(0);
                    
                    else{
                        Users* new = (Users *) shmat(shmid, NULL, 0);
                        
                        if (new == (void*)-1) exit(0);
                        
                        else{
                            strcpy(new->username, username);
                            new->logged = 0;
                            new->next = -1;
                            new->id = shmid;
                            head->next = shmid;
                            shmdt(new);
                            break;
                        }
                    }            
                }
                
                else {

                    head = (Users *) shmat(head->next, NULL, 0);
                    
                }
                
            } while(1);
            
        }
        
    } while(1);
}

// int authenticatedUser(char* buffer {
//     FILE *fp;
//     char str[MAXCHAR];
//     char* filename = "users.txt";
//     int found = 0;
//     int authenticated = '1';
//  
//     fp = fopen(filename, "r");
//     if (fp == NULL){
//         printf("Could not open file %s\n",filename);
//         exit(1);
//     }
//     while (fgets(str, MAXCHAR, fp) != NULL){
//         printf("Username nel file: %s, username arrivato: %s\n", strtok(str, "\n"), buffer);
//         if (strncmp(str, buffer, strlen(buffer)) == 0) {
//             if (strchr(str, authenticated) == NULL){
//                 found = 1;
//                 break;
//             }
//             else found = 2;
//         }
//     }
//     fclose(fp);
//     
//     return found;
// }

int authenticatedUser(char* username, int shmid){

    int found = 0;
    
    Users* head = (Users *) shmat(shmid, NULL, 0);

    if (head == NULL) {
        return found;
    }
    
    do {
        if (strcmp(head->username, username) == 0){

            if (head->logged == 0) {
            
                head->logged = 1;
                found = 1;
                
            }
                else found = 2;
            
        }
        
        if (head->next == -1) {
                break;
            }
        
        else {
            head = (Users *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
    shmdt(head);

    return found;
    
}

int logout(char* username, int shmid){

    int found = 0;
    
    Users* head = (Users *) shmat(shmid, NULL, 0);

    if (head == NULL) {
        return found;
    }
    
    do {
        if (strcmp(head->username, username) == 0){

            if (head->logged == 0) {
            
                found = -1;
                
            }
            else {
                head->logged = 0;
                found = 1;
            }
        }
        
        if (head->next == -1) {
                break;
            }
        
        else {
            head = (Users *) shmat(head->next, NULL, 0);
            
        }
        
    } while(1);
    
    shmdt(head);

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

int containsNode(int shmid, char* flag, char* toFind){
 
    int found = 0;
    
    if(strcmp(flag, "u") == 0){
        Users* head = (Users *) shmat(shmid, NULL, 0);
    
        do {
            
            if (strcmp(head->username, toFind) ==0) {
                found = 1;
                break;
            }
            
            if (head->next == -1) break;
            
            else {
            
                head = (Users *) shmat(head->next, NULL, 0);
                
            }
            
        } while(1);
    }
    else if(strcmp(flag, "m") == 0){
        Message* head = (Message *) shmat(shmid, NULL, 0);
    
        do {
            
            if (strcmp(head->body, toFind) ==0) {
                found = 1;
                break;
            }
            
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
            
            if (strcmp(head->name, toFind) ==0) {
                found = 1;
                break;
            }
            
            if (head->next == -1) break;
            
            else {
            
                head = (Thread *) shmat(head->next, NULL, 0);
                
            }
            
        } while(1);
    }
    else{
    
        Topic* head = (Topic *) shmat(shmid, NULL, 0);
    
        do {
            
            if (strcmp(head->name, toFind) ==0) {
                found = 1;
                break;
            }
            
            if (head->next == -1) break;
            
            else {
            
                head = (Topic *) shmat(head->next, NULL, 0);
                
            }
            
        } while(1);
        
    }
    
}
