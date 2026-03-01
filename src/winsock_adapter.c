#ifdef _WIN32

#include "http_server.h"
#include <winsock2.h>
#include <ws2tcpip.h>

static int win_startup(void) {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,2), &wsa);
}

static void win_cleanup(void) {
    WSACleanup();
}

static socket_handle win_socket_create(void) {
    return (socket_handle)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

static int win_socket_bind(socket_handle handle, uint16_t port) {
    SOCKET s = (SOCKET)handle;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return bind(s, (struct sockaddr*)&addr, sizeof(addr));
}

static int win_socket_listen(socket_handle handle, int backlog) {
    return listen((SOCKET)handle, backlog);
}

static socket_handle win_socket_accept(socket_handle handle) {
    return (socket_handle)accept((SOCKET)handle, NULL, NULL);
}

static int win_socket_recv(socket_handle handle, void *buffer, size_t length) {
    return recv((SOCKET)handle, buffer, (int)length, 0);
}

static int win_socket_send(socket_handle handle, const void *buffer, size_t length) {
    return send((SOCKET)handle, buffer, (int)length, 0);
}

static int win_socket_close(socket_handle handle) {
    return closesocket((SOCKET)handle);
}

SocketOperations PlatformSocketOps = {
    .startup = win_startup,
    .cleanup = win_cleanup,
    .socket_create = win_socket_create,
    .socket_bind = win_socket_bind,
    .socket_listen = win_socket_listen,
    .socket_accept = win_socket_accept,
    .socket_recv = win_socket_recv,
    .socket_send = win_socket_send,
    .socket_close = win_socket_close
};

#endif
