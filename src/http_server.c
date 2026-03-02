#include "http_server.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

struct HTTP_Server 
{
    socket_handle socket;
    uint16_t port;
    const SocketOperations *ops;
    int running;
};

const char* INDEX_PAGE_PATH = "static/index.html";
const char* STOP_PAGE_PATH = "static/stop.html";

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
    server->running = 1;

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

void http_server_run(HTTP_Server *server)
{
    if (!server) return;

    const SocketOperations *ops = server->ops;

    while (server->running)
    {
        socket_handle client = ops->socket_accept(server->socket);
        if (client == 0)
            continue;

        char request[BUFFER_SIZE] = {0};
        ops->socket_recv(client, request, sizeof(request));

        if (memcmp(request, "GET / ", 6) == 0)
        {
            FILE* f = fopen(INDEX_PAGE_PATH, "r");

            if (f)
            {
                char buffer[BUFFER_SIZE] = {0};
                fread(buffer, 1, BUFFER_SIZE, f);
                ops->socket_send(client, buffer, BUFFER_SIZE);
                ops->socket_close(client);
                fclose(f);
            }
        }

        if (memcmp(request, "GET /stop ", 10) == 0)
        {
            FILE* f = fopen(STOP_PAGE_PATH, "r");

            if (f)
            {
                char buffer[BUFFER_SIZE] = {0};
                fread(buffer, 1, BUFFER_SIZE, f);
                ops->socket_send(client, buffer, BUFFER_SIZE);
                ops->socket_close(client);
                fclose(f);
            }

            http_server_stop(server);
        }

        ops->socket_close(client);
    }

    if (server->socket) {
        ops->socket_close(server->socket);
        server->socket = 0;
    }

    ops->cleanup();
    free(server);
}

void http_server_stop(HTTP_Server *server)
{
    if (!server) return;

    server->running = 0;

    if (server->ops && server->socket) {
        server->ops->socket_close(server->socket);
        server->socket = 0;
    }
}
