#pragma once

#include <stdio.h>

char* split(char *buf, char delim[]);
int authenticatedUser(char* buffer, int shmid);
int sem_init_(key_t key);
int setSem(int semid, int sem_num);
int resetSem(int semid, int sem_num);
void initUserList(int shmid);
int logout(char* username, int shmid);
