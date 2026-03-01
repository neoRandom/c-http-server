#include "http_server.h"

struct HTTP_Server 
{
    socket_handle socket;
    uint16_t port;
    const SocketOperations *ops;
};

HTTP_Server * init_server(uint16_t port, const SocketOperations *ops) 
{
    if (!ops) { return NULL; }

    if (ops->startup && ops->startup() != 0) {
        return NULL;
	}

    HTTP_Server *server = malloc(sizeof(HTTP_Server));
    if (!server) {
        return NULL;
	}

    server->ops = ops;
    server->port = port;

    server->socket = ops->socket_create();
    if (server->socket == 0) {
        return NULL;
	}

    if (ops->socket_bind(server->socket, port) != 0) {
        return NULL;
	}
    if (ops->socket_listen(server->socket, 10) != 0) {
        return NULL;
	}

    return server;
}

socket_handle http_server_get_socket(HTTP_Server *server)
{
    return server ? server->socket : 0;
}

const SocketOperations* http_server_get_ops(HTTP_Server *server)
{
    return server ? server->ops : NULL;
}
