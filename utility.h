#pragma once

#include <stdio.h>

char* split(char *buf, char delim[]);
int authenticatedUser(char* buffer);
int sem_init(key_t key);
