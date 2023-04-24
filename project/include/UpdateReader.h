#include <msgQLib.h>

/**
 * A type that helps with reading our update message queue, but translates them into
 * things our visualizer can understand
 */
typedef struct {
    MSG_Q_ID _updateQueue;
} UpdateReader;
