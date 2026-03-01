#ifndef _WIN32

#include "http_server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

static int posix_startup(void) { return 0; }
static void posix_cleanup(void) {}

static socket_handle posix_socket_create(void) {
    return socket(AF_INET, SOCK_STREAM, 0);
}

static int posix_socket_bind(socket_handle handle, uint16_t port) {
    int s = (int)handle;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return bind(s, (struct sockaddr*)&addr, sizeof(addr));
}

static int posix_socket_listen(socket_handle handle, int backlog) {
    return listen((int)handle, backlog);
}

static socket_handle posix_socket_accept(socket_handle handle) {
    return accept((int)handle, NULL, NULL);
}

static int posix_socket_recv(socket_handle handle, void *buffer, size_t length) {
    return recv((int)handle, buffer, length, 0);
}

static int posix_socket_send(socket_handle handle, const void *buffer, size_t length) {
    return send((int)handle, buffer, length, 0);
}

static int posix_socket_close(socket_handle handle) {
    return close((int)handle);
}

SocketOperations PlatformSocketOps = {
    .startup = posix_startup,
    .cleanup = posix_cleanup,
    .socket_create = posix_socket_create,
    .socket_bind = posix_socket_bind,
    .socket_listen = posix_socket_listen,
    .socket_accept = posix_socket_accept,
    .socket_recv = posix_socket_recv,
    .socket_send = posix_socket_send,
    .socket_close = posix_socket_close
};

#endif
