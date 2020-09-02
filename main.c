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
char threadName[50];
char topicName[50];
char username[20];

#define PORT 4444

void shutdownServer(){
    
    strcpy(buffer,"shutdown:");
    strcat(buffer, "\0");
    printf("stringa concatenata %s\n", buffer);
    
    send(clientSocket, buffer, strlen(buffer), 0);
        
    bzero(buffer, sizeof(buffer));
        
    int res = recv(clientSocket, buffer, 1024, 0);
        
    if (res <0){
        printf("Error comunicating with server\n");
        exit(1);
    }
    
    if (strcmp(buffer, "ok") == 0) printf("Server shutted down correctly\n");
    else printf("An error occured. Retry...\n");
    
}

void logout(char* username){
    
    bzero(buffer, sizeof(buffer));
    
    strcpy(buffer,"logout:");
    strcat(buffer, username);
    strcat(buffer, "\0");
    printf("stringa concatenata %s\n", buffer);
    
    send(clientSocket, buffer, strlen(buffer), 0);
        
    bzero(buffer, sizeof(buffer));
        
    int res = recv(clientSocket, buffer, 1024, 0);
        
    if (res <0){
        printf("Error comunicating with server\n");
        exit(1);
    }
    
    if (strcmp(buffer, "ok") == 0) printf("You are now logged out correctly!\n");
    
    else if (strcmp(buffer, "ko") == 0) printf("Some error occured or inserted user not logged in! Retry...\n");
}

void deleteThread(char* threadName){
    char username[50];
    
    printf("Type your username:\n");
    scanf("%s", &username[0]);
    
    bzero(buffer, sizeof(buffer));
    
    strcpy(buffer,"delThread:");
    strcat(buffer, threadName);
    strcat(buffer, "~");
    strcat(buffer, username);
    strcat(buffer, "\0");
    
    send(clientSocket, buffer, strlen(buffer), 0);
        
    bzero(buffer, sizeof(buffer));
        
    int res = recv(clientSocket, buffer, 1024, 0);
        
    if (res <0){
        printf("Error comunicating with server\n");
        exit(1);
    }
    
    if (strcmp(buffer, "ok") == 0) printf("Thread and all topics and messages related are deleted correctly!\n");
}

void addThread(char* threadName){
    
    char username[50];
    
    printf("Type your username:\n");
    scanf("%s", &username[0]);

    bzero(buffer, sizeof(buffer));
    
    strcpy(buffer,"addThread:");
    strcat(buffer, threadName);
    strcat(buffer, "~");
    strcat(buffer, username);
    strcat(buffer, "\0");
    
    send(clientSocket, buffer, strlen(buffer), 0);
        
    bzero(buffer, sizeof(buffer));
        
    int res = recv(clientSocket, buffer, 1024, 0);
        
    if (res <0){
        printf("Error comunicating with server\n");
        exit(1);
    }
    
    if (strcmp(buffer, "ok") == 0) printf("Thread created correctly!\n");
    else printf("Error adding new thread or this thread already exist!\n");

    
}

void addTopic(char* threadName, char* topicName){
    
    char username[50];
    
    printf("Type your username:\n");
    scanf("%s", &username[0]);

    bzero(buffer, sizeof(buffer));
    
    strcpy(buffer,"addTopic:");
    strcat(buffer, threadName);
    strcat(buffer, "~");
    strcat(buffer, topicName);
    strcat(buffer, "~");
    strcat(buffer, username);
    strcat(buffer, "\0");
    
    send(clientSocket, buffer, strlen(buffer), 0);
        
    bzero(buffer, sizeof(buffer));
        
    int res = recv(clientSocket, buffer, 1024, 0);
        
    if (res <0){
        printf("Error comunicating with server\n");
        exit(1);
    }
    
    if (strcmp(buffer, "ok") == 0) printf("Thread created correctly!\n");
    
    else printf("Error adding new topic or this topic already exist!\n");
    
}

void reply(char* messageBody){
    
    char username[50];
    
    printf("Type your username:\n");
    scanf("%s", &username[0]);

    bzero(buffer, sizeof(buffer));
    
    strcpy(buffer,"replyTO:");
    strcat(buffer, topicName);
    strcat(buffer, "~");
    strcat(buffer, messageBody);
    strcat(buffer, "~");
    strcat(buffer, username);
    strcat(buffer, "\0");
    
    send(clientSocket, buffer, strlen(buffer), 0);
        
    bzero(buffer, sizeof(buffer));
        
    int res = recv(clientSocket, buffer, 1024, 0);
        
    if (res <0){
        printf("Error comunicating with server\n");
        exit(1);
    }
    
    if (strcmp(buffer, "ok") == 0) printf("Message replied correctly!\n");
    else printf("Error adding new message or this message already exist!\n");

}

void list(char* service){
    
    bzero(buffer, sizeof(buffer));
    
    if (service == "message") {
        
        // create a message to server in form "listM:threadname,topicname,message"
        bzero(buffer, sizeof(buffer));
        strcpy(buffer,"listM:");
        strcat(buffer, topicName);
        strcat(buffer, "\0");
        
        send(clientSocket, buffer, strlen(buffer), 0);
        
        bzero(buffer, sizeof(buffer));
        
        int res = recv(clientSocket, buffer, 1024, 0);
        
        if (res <0){
            printf("Error comunicating with server\n");
            exit(1);
        }
                        printf("%s\n", buffer);

    }
    
    if (service == "topic") {
        
        // create a message to server in form "listM:threadname,topicname,message"
        strcpy(buffer,"listT:");
        strcat(buffer, threadName);
        strcat(buffer, "\0");
        
        send(clientSocket, buffer, strlen(buffer), 0);
        
        bzero(buffer, sizeof(buffer));
        
        int res = recv(clientSocket, buffer, 1024, 0);
        
        if (res <0){
            printf("Error comunicating with server\n");
            exit(1);
        }
        
                printf("%s\n", buffer);

        
    }
    
    if (service == "thread") {
        
        // create a message to server in form "listM:threadname,topicname,message"
        strcpy(buffer,"listTH:");
        strcat(buffer, "\0");
        send(clientSocket, buffer, strlen(buffer), 0);
        
        bzero(buffer, sizeof(buffer));
        
        int res = recv(clientSocket, buffer, 1024, 0);
        
        if (res <0){
            printf("Error comunicating with server\n");
            exit(1);
        }
        
        printf("%s\n", buffer);
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
        printf("utente già loggato\n");
    }
    
    else if(strcmp(buffer, "authenticated") == 0){
        printf("autenticato\n");
    }
    
    else if(strcmp(buffer, "unautorized") == 0){
        printf("NON autenticato\n");
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
    
    return 1;
//     int res = authenticate(clientSocket, buffer, strlen(buffer), 0);
    //repeat until server response with authenticated
    
}

int main(int argc, char* argv[]){
    
    char* cmd = argv[1];
    int serviceToCall = 0;
    char messageBody[140];
//     printf("argv1 %d, argv2 %d, argv3 %d, argv4 %d, \n", argv[1], argv[2], argv[3], argv[4]);
    
    if (argv[1] == NULL) printf("\nUSAGE: {authenticate | list [ threads | topics|  messages ] | create [thread] | addTopic [topic -> thread] | delete [thread] | reply [topic name + body (max 140)]}\n");
    else{
        if (strcmp(cmd, "authenticate") == 0) serviceToCall = 1;
        if (strcmp(cmd, "list") == 0) {
            if (argv[2] == NULL) printf("\nUSAGE: ./main list [ threads | topics [thread name]|  messages [topic name]\n");
            else if(strcmp(argv[2], "messages") == 0) {
                if (argv[3] != NULL && argc >= 3){
                    serviceToCall = 2;
                    strcpy(topicName, argv[3]);
                }
                else{
                    printf("\nUSAGE: ./main list messages [topic name]\n");
                }
            }
            else if (strcmp(argv[2], "topics") == 0) {
            
                if (argv[3] != NULL){
                    serviceToCall = 3;
                    strcpy(threadName, argv[3]);
                }
                else{
                    printf("\nUSAGE: ./main list topics (thread name)]\n");
                }
                
            }
            else if (strcmp(argv[2], "threads") == 0) serviceToCall = 4;
        }
        
        if (strcmp(cmd, "reply")== 0) {
            if (argc >= 4 && argv[3] != NULL && strlen(argv[3]) <= 140){
                    
                    serviceToCall = 5;
                    strcpy(topicName, argv[2]);
                    strcpy(messageBody, argv[3]);
                }
                else{
                    printf("%s, %s\n", argv[3], argv[4]);
                    printf("\nUSAGE: ./main reply (topic name + message max 140)]\n");
                }
        }
        
        if (strcmp(cmd, "create") == 0){
            if (argv[2] == NULL) printf("\nUSAGE: ./main create [thread]\n");
            else{
                strcpy(threadName, argv[2]);
                serviceToCall = 6;
            }
        }
        
        if (strcmp(cmd, "delete") == 0){
            if (argv[2] == NULL) printf("\nUSAGE: ./main delete [thread]\n");
            else{
                serviceToCall = 7;
                strcpy(threadName, argv[2]);
            }
        }
        
        if (strcmp(cmd, "addTopic") == 0){
            if (argv[2] == NULL || argv[3] == NULL && argc < 4) printf("\nUSAGE: ./main addTopic [thread name + topic name]\n");
            else{
                strcpy(threadName, argv[2]);
                strcpy(topicName, argv[3]);
                serviceToCall = 8;
            }
        }
        
        if (strcmp(cmd, "logout") == 0){
            if (argv[2] == NULL) printf("\nUSAGE: ./main logout [username]\n");
            else{
                strcpy(username, argv[2]);
                serviceToCall = 9;
            }
        }
        
        if (strcmp(cmd, "shutdown") == 0){
            serviceToCall = 10;
        }
        
    }
    
//     printf("service to call %d\n", serviceToCall);
    
    switch(serviceToCall){
        
        case 1:
            if (connectToServer() > 0) authenticate();
            break;
        case 2:
            if (connectToServer() > 0) list("message");
            break;
        case 3:
            if (connectToServer() > 0) list("topic");
            break;
        case 4:
            if (connectToServer() > 0) list("thread");
            break;
        case 5:
            if (connectToServer() > 0) reply(messageBody);
            break;
        case 6:
            if (connectToServer() > 0) addThread(threadName);
            break;
        case 7:
            if (connectToServer() > 0) deleteThread(threadName);
            break;
        case 8:
            if (connectToServer() > 0) addTopic(threadName, topicName);
            break;
        case 9:
            if (connectToServer() > 0) logout(username);
            break;
        case 10:
            if (connectToServer() > 0) shutdownServer();
            break;
    }  
    
    /*for (int i = 0; i < argc; i++){
        printf(argv[i]);
    }*/
    
    return 0;
}
