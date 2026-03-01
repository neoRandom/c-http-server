#include <stdio.h>
#include <string.h>
#include "http_server.h"

extern SocketOperations PlatformSocketOps;

int main() 
{
    HTTP_Server *server = init_server(8080, &PlatformSocketOps);
    if (!server) {
        printf("Failed to start server\n");
        return 1;
    }

    const SocketOperations *ops = http_server_get_ops(server);
    if (!ops) {
        printf("Failed to get server operations\n");
        return 1;
    }

    socket_handle server_socket = http_server_get_socket(server);

    //

    printf("Server initialized on port 8080\n");

    //

    while (1) 
    {
        socket_handle client = ops->socket_accept(server_socket);

        char request[512] = {0};
        ops->socket_recv(client, request, sizeof(request));

        //

        if (memcmp(request, "GET / ", 6) == 0) 
        {
            FILE* f = fopen("static/index.html", "r");

            char buffer[1024] = {0};
            fread(buffer, 1, 1024, f);

            ops->socket_send(client, buffer, 1024);

            fclose(f);
        }

        //

        if (memcmp(request, "GET /close-the-server ", 22) == 0) 
        {
            const char* msg = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n";

            ops->socket_send(client, msg, strlen(msg));
            ops->socket_close(client);
            break;
        }

        //

        ops->socket_close(client);
    }

    //

    ops->cleanup();
    free(server);

    return 0;
}
