#ifndef SIGNAL_PORT
#define SIGNAL_PORT

#include <stdint.h>
#include <stdlib.h>

typedef struct SignalOperations
{
    int (*register_interrupt_handler)(void (*handler)(void));
} SignalOperations;

extern SignalOperations PlatformSignalOps;

#endif // SIGNAL_PORT
