#include "http_server.h"

/* Concrete Adapter */

static SOCKET winsock_create(int af, int type, int protocol) {
    return socket(af, type, protocol);
}

static int winsock_bind(SOCKET s, const struct sockaddr *name, int namelen) {
    return bind(s, name, namelen);
}

static int winsock_listen(SOCKET s, int backlog) {
    return listen(s, backlog);
}

/* Exported adapter instance */
SocketOperations WinsockOps = {
    .create = winsock_create,
    .bind   = winsock_bind,
    .listen = winsock_listen
};
