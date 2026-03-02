#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L

#include "signal_port.h"
#include <signal.h>
#include <string.h>

static void (*global_handler)(void) = NULL;

static void posix_signal_handler(int signo)
{
    if (signo == SIGINT && global_handler)
    {
        global_handler();
    }
}

static int posix_register_interrupt_handler(void (*handler)(void))
{
    global_handler = handler;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = posix_signal_handler;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        return -1; 
    }

    return 0;
}

SignalOperations PlatformSignalOps = {
    .register_interrupt_handler = posix_register_interrupt_handler
};

#endif
