#ifndef NETWORKING_H
#define NETWORKING_H

#include "ae.h"

typedef struct client client;

client *createClient(int fd);

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void acceptUnixHandler(aeEventLoop *el, int fd, void *privdata, int mask);


#endif /* NETWORKING_H */
