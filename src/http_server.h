#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdint.h>
#include <stdlib.h>

typedef intptr_t socket_handle;

typedef struct SocketOperations 
{
    int (*startup)(void);
    void (*cleanup)(void);

    socket_handle (*socket_create)(void);
    int (*socket_bind)(socket_handle, uint16_t port);
    int (*socket_listen)(socket_handle, int backlog);

    socket_handle (*socket_accept)(socket_handle);
    int (*socket_recv)(socket_handle, void *buffer, size_t length);
    int (*socket_send)(socket_handle, const void *buffer, size_t length);
    int (*socket_close)(socket_handle);
} SocketOperations;

typedef struct HTTP_Server HTTP_Server;

HTTP_Server* init_server(uint16_t port, const SocketOperations *ops);
void http_server_run(HTTP_Server *server);

#endif // HTTP_SERVER_H
