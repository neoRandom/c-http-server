#include <stdio.h>
#include <string.h>
#include "http_server.h"
#include "signal_port.h"

extern SocketOperations PlatformSocketOps;

static HTTP_Server *g_server = NULL;

static void on_interrupt(void)
{
    if (g_server)
    {
        printf("\nInterrupt received. Shutting down...\n");
        http_server_stop(g_server);
    }
}

int main() 
{
    HTTP_Server *server = init_server(8080, &PlatformSocketOps);
    if (!server) {
        printf("Failed to start server\n");
        return 1;
    }

    g_server = server;

    if (PlatformSignalOps.register_interrupt_handler(on_interrupt) != 0)
    {
        printf("Failed to register interrupt handler\n");
        return 1;
    }

    printf("Server initialized on port 8080\n");
    printf("Press Ctrl+C to stop the server\n");

    http_server_run(server);

    return 0;
}
