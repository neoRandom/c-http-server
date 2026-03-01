#include "http_server.h"

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
