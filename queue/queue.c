#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct qhead *buildQueue(){
    struct qhead *tmp = (struct qhead *)malloc(sizeof(struct qhead));
    tmp->front = NULL;
    return tmp;
}

void addCmd(struct qhead *queueStart, char *cmd, unsigned int cmdLen){
    struct qnode *tmp = (struct qnode *)malloc(sizeof(struct qnode));
    struct qnode *i;

    if(queueStart->front == NULL){
        queueStart->front = tmp;
        queueStart->front->cmd = malloc(sizeof(char) * cmdLen);
        strncpy(queueStart->front->cmd, cmd, cmdLen);
        queueStart->front->next = NULL;
    }else{
        for(i = queueStart->front; i->next != NULL; i = i->next){
            continue;
        }
        i->next = tmp;
        i->next->cmd = (char*)malloc(sizeof(char) * cmdLen);
        strncpy(i->next->cmd, cmd, cmdLen);
        i->next->next = NULL;
    }

    return;
}

char *getCommand(struct qhead *start){
    
    unsigned int size = strlen(start->front->cmd);
    char *cmd = (char *)malloc(sizeof(char) * size);
    strncpy(cmd, start->front->cmd, size);

    struct qnode *tmp;
    tmp = start->front;
    start->front = start->front->next;
    free(tmp);
    
    return cmd;
}

void displayQueue(struct qhead *start){
    struct qnode *i;
    unsigned int pos = 0;
    for(i = start->front; i != NULL; i = i->next){
        printf("[%d] - %s\n", pos, i->cmd);
        pos++;
    }
}
