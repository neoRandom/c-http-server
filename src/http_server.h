#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <winsock2.h>
#include <Windows.h>
#include <stdlib.h>

/* Socket abstraction (Port) */
typedef struct SocketOperations {
    SOCKET (*create)(int af, int type, int protocol);
    int (*bind)(SOCKET s, const struct sockaddr *name, int namelen);
    int (*listen)(SOCKET s, int backlog);
} SocketOperations;

typedef struct HTTP_Server {
    SOCKET socket;
    struct sockaddr_in addr;
} HTTP_Server;

/* Dependency Injection: pass operations */
HTTP_Server* init_server(u_short port, const SocketOperations *ops);

#endif // HTTP_SERVER_H
