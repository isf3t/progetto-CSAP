#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int clientSocket, ret;
struct sockaddr_in serverAddr;
char buffer[1024];
#define PORT 4446

void list(char* service){
    
    bzero(buffer, sizeof(buffer));
    
    if (service == "message") {
        strcpy(buffer,"listM: ");
        send(clientSocket, buffer, strlen(buffer), 0);
        
        bzero(buffer, sizeof(buffer));
        
        int res = recv(clientSocket, buffer, 1024, 0);
        
        if (res <0){
            printf("Error comunicating with server\n");
            exit(1);
        }
        
        printf("Ho ricevuto: %s\n", buffer);
    
    }
    
    if (service == "topic") {
        strcpy(buffer,"listT: ");
        send(clientSocket, buffer, strlen(buffer), 0);
    }
    
    if (service == "thread") {
        strcpy(buffer,"listTH: ");
        send(clientSocket, buffer, strlen(buffer), 0);
    }
    
}


void authenticate(){
    
    strcpy(buffer,"username:");
    char str[50];
    
    bzero(str, sizeof(str));
        
//     printf("buffer: %s\n", buffer);
//     printf("str: %s\n", str);
        
    printf("Username: \t");
    scanf("%s", &str[0]);
        
//     printf("%s\t%s\n", buffer, str);
    strcat(buffer, str);
         
//     printf("stringa concatenata: %s\n", buffer);
        
    send(clientSocket, buffer, strlen(buffer), 0);
    
    bzero(buffer, sizeof(buffer));
    
    int res = recv(clientSocket, buffer, 1024, 0);
    
    if (res <0){
        printf("Error comunicating with server\n");
        exit(1);
    }
    
    if(strcmp(buffer, ":exit") == 0){
        close(clientSocket);
        printf("[-]Disconnected from server.\n");
        exit(1);
    }
    
    else if(strcmp(buffer, "alreadyAuthenticated") == 0){
        printf("utente già loggato");
    }
    
    else if(strcmp(buffer, "authenticated") == 0){
        printf("autenticato");
    }
    
    else if(strcmp(buffer, "unautorized") == 0){
        printf("NON autenticato");
    }
    
    
    bzero(buffer, sizeof(buffer));
}

int connectToServer(){

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		return -1;
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		return -1;
	}
	printf("[+]Connected to Server.\n");
    printf("[+]Authentication needed.\n");
    
    return 1;
//     int res = authenticate(clientSocket, buffer, strlen(buffer), 0);
    //repeat until server response with authenticated
    
}

int main(int argc, char* argv[]){
    
    char* cmd = argv[1];
    int serviceToCall = 0;
    char* messageCODE;
    char messageBODY[140];
    
    printf("argv %d\n", argv[2] == "");
    
    if (argv[1] == NULL) printf("\nUSAGE: {authenticate | list [ threads | topics|  messages ] | get [message#] | status [message#] | create [topic] | append [topic -> thread] | delete [topic]}\n");
    else{
        if (strcmp(cmd, "authenticate") == 0) serviceToCall = 1;
        if (strcmp(cmd, "list") == 0) {
            if (argv[2] == NULL) printf("\nUSAGE: ./main list list [ threads | topics|  messages ]\n");
            else if(strcmp(argv[2], "messages") == 0) serviceToCall = 2;
            else if (strcmp(argv[2], "topics") == 0) serviceToCall = 3;
            else if (strcmp(argv[2], "threads") == 0) serviceToCall = 4;
        }
        
        if (strcmp(cmd, "get") == 0) {
            if (argv[2] == NULL) printf("\nUSAGE: ./main get [message#]\n");
            else{
                serviceToCall = 5;
                messageCODE = argv[2];
            }
        }
        
        if (strcmp(cmd, "status") == 0){
            if (argv[2] == NULL) printf("\nUSAGE: ./main status [message#]\n");
            else{
                serviceToCall = 6;
            }
        }
        
        if (strcmp(cmd, "create") == 0){
            if (argv[2] == NULL) printf("\nUSAGE: ./main create [topic]\n");
            else{
                serviceToCall = 7;
            }
        }
        
        if (strcmp(cmd, "append") == 0){
            if (argv[2] == NULL) printf("\nUSAGE: ./main append [topic] [thread]\n");
            else{
                serviceToCall = 8;
            }
        }
        
        if (strcmp(cmd, "delete") == 0){
            if (argv[2] == NULL) printf("\nUSAGE: ./main delete [topic]\n");
            else{
                serviceToCall = 9;
            }
        }
        
    }
    
    printf("service to call %d\n", serviceToCall);
    
    switch(serviceToCall){
        
        case 1:
            if (connectToServer() > 0) authenticate();
            break;
        case 2:
            printf("service to call %d\n", serviceToCall);
            if (connectToServer() > 0) list("message");
            break;
        case 3:
            printf("service to call %d\n", serviceToCall);
            if (connectToServer() > 0) list("topic");
            break;
        case 4:
            printf("service to call %d\n", serviceToCall);
            if (connectToServer() > 0) list("thread");
            break;
        case 5:
            printf("servizio %d\n", serviceToCall);
            break;
        case 6:
            printf("servizio %d\n", serviceToCall);
            break;
        case 7:
            printf("servizio %d\n", serviceToCall);
            break;
    }  
    
    /*for (int i = 0; i < argc; i++){
        printf(argv[i]);
    }*/
    
    return 0;
}
