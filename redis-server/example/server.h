#ifndef SERVER_H
#define SERVER_H

#include "../src/ae.h"

#define CONFIG_MIN_RESERVED_FDS 32
#define CONFIG_FDSET_INCR (CONFIG_MIN_RESERVED_FDS + 96)
#define CONFIG_DEFAULT_MAX_CLIENTS 10000
#define CONFIG_DEFAULT_SERVER_PORT 6379

struct redisServer
{
    aeEventLoop *el;
    int port;
    unsigned int maxclients; 
};

#endif /* SERVER_H */
