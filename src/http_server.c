#include "http_server.h"

HTTP_Server* init_server(u_short port, const SocketOperations *ops {
    WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	if (!ops) return NULL;

    HTTP_Server* http_server = (HTTP_Server*) malloc(sizeof(HTTP_Server));
	if (!http_server) return NULL;
	
	http_server->socket = ops->socket(AF_INET, SOCK_STREAM, 0);

	http_server->addr.sin_family = AF_INET;
	http_server->addr.sin_addr.s_addr = 0;
	http_server->addr.sin_port = htons(port);

	ops->bind(
		http_server->socket, 
		(SOCKADDR*) &http_server->addr, 
		sizeof(http_server->addr)
	);

	ops->listen(http_server->socket, 10);

    return http_server;
}
