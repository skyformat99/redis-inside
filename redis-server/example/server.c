#include "server.h"

struct redisServer server; 

void initServerConfig(void)
{
    server.port = CONFIG_DEFAULT_SERVER_PORT;
    server.maxclients = CONFIG_DEFAULT_MAX_CLIENTS;
}

void initServer(void)
{
    server.el = aeCreateEventLoop(server.maxclients + CONFIG_FDSET_INCR);
    if (server.port != 0 && )
}

int main(int argc, char *argv[])
{
    initServerConfig();
    initServer();
    

    return 0;
}

