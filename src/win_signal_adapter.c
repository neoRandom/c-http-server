#ifdef _WIN32

#include "signal_port.h"
#include <windows.h>

static void (*global_handler)(void) = NULL;

static BOOL WINAPI console_handler(DWORD signal)
{
    if (signal == CTRL_C_EVENT && global_handler)
    {
        global_handler();
        return TRUE;
    }
    return FALSE;
}

static int win_register_interrupt_handler(void (*handler)(void))
{
    global_handler = handler;
    return SetConsoleCtrlHandler(console_handler, TRUE) ? 0 : -1;
}

SignalOperations PlatformSignalOps = {
    .register_interrupt_handler = win_register_interrupt_handler
};

#endif
