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

    printf("Server initialized on port 8080\n");

    http_server_run(server);

    return 0;
}
