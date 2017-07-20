/*
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __REDIS_H
#define __REDIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <syslog.h>
#include <netinet/in.h>
#include <lua.h>
#include <signal.h>

#define CONFIG_DEFAULT_SERVER_PORT          6379    /* TCP port */
#define CONFIG_DEFAULT_TCP_BACKLOG          511     /* TCP listen backlog */
#define CONFIG_DEFAULT_CLIENT_TIMEOUT       0       /* default client timeout: infinite */
#define CONFIG_BINDADDR_MAX                 16
#define CONFIG_DEFAULT_UNIX_SOCKET_PERM     0
#define CONFIG_DEFAULT_PROTECTED_MODE 1
#define CONFIG_DEFAULT_TCP_KEEPALIVE 300
#define CONFIG_DEFAULT_MAX_CLIENTS 10000

#define CONFIG_MIN_RESERVED_FDS 32

/* When configuring the server eventloop, we setup it so that the total number
 * of file descriptors we can handle are server.maxclients + RESERVED_FDS +
 * a few more to stay safe. Since RESERVED_FDS defaults to 32, we add 96
 * in order to make sure of not over provisioning more than 128 fds. */
#define CONFIG_FDSET_INCR (CONFIG_MIN_RESERVED_FDS+96)


struct redisServer {
    aeEventLoop *el;

    /* Networking */
    int port;                            /* TCP listening port */
    int tcp_backlog;                     /* TCP listen() backlog */
    char *bindaddr[CONFIG_BINDADDR_MAX]; /* Addresses we should bind to */
    int bindaddr_count;                  /* Number of addresses in server.bindaddr[] */
    char *unixsocket;                    /* UNIX socket path */
    mode_t unixsocketperm;               /* UNIX socket permission */
    int ipfd[CONFIG_BINDADDR_MAX];       /* TCP socket file descriptors */
    int ipfd_count;                      /* Used slots in ipfd[] */
    int sofd;                            /* Unix socket file descriptor */

    int cfd[CONFIG_BINDADDR_MAX];        /* Cluster bus listening socket */
    int cfd_count;                       /* Used slots in cfd[] */
    
    list *clients;                       /* List of active clients */
    list *clients_to_close;              /* Clients to close asynchronously */
    list *clients_pending_write;         /* There is to write or install handler. */
    list *slaves, *monitors;             /* List of slaves and MONITORs */
    client *current_client;              /* Current client, only used on crash report */
    int clients_paused;                  /* True if clients are currently paused */
    mstime_t clients_pause_end_time;     /* Time when we undo clients_paused */
    char neterr[ANET_ERR_LEN];           /* Error buffer for anet.c */
    // dict *migrate_cached_sockets;        /* MIGRATE cached sockets */
    uint64_t next_client_id;             /* Next client unique ID. Incremental. */
    int protected_mode;                  /* Don't accept external connections. */
    int tcpkeepalive;                    /* Set SO_KEEPALIVE if
                                          * non-zero. */
    unsigned int maxclients;            /* Max number of simultaneous clients */    
    
};

#endif /* __REDIS_H */
