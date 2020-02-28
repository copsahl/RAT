#ifndef QUEUE_H
#define QUEUE_H

#include <string.h>

struct qnode{
    char *cmd;
    struct qnode *next;

};

struct qhead{
    struct qnode *front;
};

struct qhead *buildQueue();
void addCmd(struct qhead *queueStart, char *cmd, unsigned int cmdLen);
char *getCommand(struct qhead *start);
void displayQueue(struct qhead *start);

#endif
