#ifndef SERVER_H
#define SERVER_H

#include "ae.h"
#include "anet.h"
#include "adlist.h"

#include <unistd.h>
#include <stdint.h>

#define CONFIG_MIN_RESERVED_FDS 32
#define CONFIG_FDSET_INCR (CONFIG_MIN_RESERVED_FDS + 96)
#define CONFIG_DEFAULT_MAX_CLIENTS 10000
#define CONFIG_DEFAULT_SERVER_PORT 6379
#define CONFIG_BINDADDR_MAX 16
#define CONFIG_DEFAULT_TCP_BACKLOG       511     /* TCP listen backlog */
#define CONFIG_DEFAULT_UNIX_SOCKET_PERM 0
#define PROTO_REPLY_CHUNK_BYTES (16*1024) /* 16k output buffer */
#define PROTO_IOBUF_LEN         (1024*16)  /* Generic I/O buffer size */
#define CLIENT_UNIX_SOCKET (1<<11) /* Client connected via Unix domain socket */
#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */
#define CONFIG_DEFAULT_TCP_KEEPALIVE 300

#define C_ERR                   -1
#define C_OK                    0

#define serverPanic(_e) _serverPanic(#_e,__FILE__,__LINE__),_exit(1)

/* Anti-warning macro... */
#define UNUSED(V) ((void) V)

struct redisServer
{
    aeEventLoop *el;
    int port;
    unsigned int maxclients;
    int ipfd[CONFIG_BINDADDR_MAX];       // 作为 listen() 用的 tcp 文件描述符
    int ipfd_count;                      // ipfd 数组的长度

    char *bindaddr[CONFIG_BINDADDR_MAX]; // 作为 bind() 用的地址 
    int bindaddr_count;                  // bindaddr 数组的长度
    int tcp_backlog;                     /* tcp listen() 的 backlog 参数 */    
    char neterr[ANET_ERR_LEN];           /* Error buffer for anet.c */

    char *unixsocket;           /* UNIX socket path */
    int sofd;                   /* Unix socket file descriptor */
    mode_t unixsocketperm;      /* UNIX socket permission */

    uint64_t next_client_id;    /* Next client unique ID Incremental. */
    int tcpkeepalive;           /* Set SO_KEEPALIVE if non-zero. */
    list *clients;              /* List of active clients */        
};

typedef struct client {
    uint64_t id;
    int fd;

    /* Response buffer */
    int bufpos;
    char buf[PROTO_REPLY_CHUNK_BYTES];    // 发送缓存区
} client;


extern struct redisServer server;


#endif /* SERVER_H */
