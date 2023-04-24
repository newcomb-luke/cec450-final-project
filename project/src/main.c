#include "Effectors.h"
#include "LogMessage.h"
#include "SensorValues.h"
#include "Sensors.h"
#include "VisualizerMessaage.h"
#include "vector.h"
#include "array.h"
#include "Visualizer.h"
#include <sysLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <semLib.h>
#include <stdlib.h>
#include <msgQLib.h>
#include <time.h>
#include <wdLib.h>
#include <sigLib.h>
#include <tickLib.h>

int runMe(size_t argA, size_t argSemBin, size_t argMsgQ);
void repeatMe(int a);

WDOG_ID wd;

struct timespec main_subtractTimespecs(struct timespec before, struct timespec after);

int main() {
    // Necessary for the emulator to start counting ticks
    tickGet();

    struct timespec startTime;

    clock_gettime(CLOCK_REALTIME, &startTime);

    Visualizer* visualizer = malloc(sizeof(Visualizer));

    Visualizer_init(visualizer);

    Visualizer_start(visualizer);

    for (;;) {}

    /*

    SEM_ID semBin = semBCreate(0, SEM_EMPTY);

    if (semBin == NULL) {
        printf("Failed to create semaphore\n");
        exit(-1);
    }

    MSG_Q_ID msgQ = msgQCreate(10, sizeof(int), MSG_Q_FIFO);

    if (msgQ == NULL) {
        printf("Failed to create message queue\n");
        exit(-1);
    }

    TASK_ID task = taskSpawn("", 100, 0, 0x2000, (FUNCPTR) runMe,
                                            5,
                                            (size_t) semBin,
                                            (size_t) msgQ,
                                            0,
                                            0,
                                            0,
                                            0,
                                            0,
                                            0,
                                            0);

    printf("Hello from main thread\n");

    taskDelay(500);

    printf("Hello again from main thread\n");

    taskDelay(500);

    semGive(semBin);

    taskDelay(500);

    for (int i = 0; i < 9; i++) {
        int b = i * 3;
        msgQSend(msgQ, (char*) &b, sizeof(int), WAIT_FOREVER, MSG_PRI_NORMAL);
    }

    int c = 69;
    msgQSend(msgQ, (char*) &c, sizeof(int), WAIT_FOREVER, MSG_PRI_URGENT);

    printf("Num messages queued: %d\n", msgQNumMsgs(msgQ));

    c = 10;
    msgQSend(msgQ, (char*) &c, sizeof(int), WAIT_FOREVER, MSG_PRI_NORMAL);

    taskDelay(500);

    printf("Parent task is done!\n");

    // Would not be required on VxWorks, since tasks detach
    for (;;) {

        printf("Current time in ticks: %lu\n", tickGet());

        taskDelay(40);
    }

    */

    return 0;
}

struct timespec main_subtractTimespecs(struct timespec before, struct timespec after) {
    struct timespec result;

    result.tv_sec = after.tv_sec - before.tv_sec;
    result.tv_nsec = after.tv_nsec - before.tv_nsec;

    if (result.tv_nsec < 0) {
        result.tv_sec--;

        result.tv_nsec = 1000000000 - result.tv_nsec;
    }

    return result;
}

int runMe(size_t argA, size_t argSemBin, size_t argMsgQ) {
    int a = (int) argA;
    SEM_ID semBin = (SEM_ID) argSemBin;
    MSG_Q_ID msgQ = (MSG_Q_ID) argMsgQ;

    printf("Waiting to print...\n");

    semTake(semBin, WAIT_FOREVER);

    printf("Number that I was given: %d\n", a);

    for (int i = 0; i < 11; i++) {
        int b = 0;

        msgQReceive(msgQ, (char*) &b, sizeof(int), WAIT_FOREVER);

        printf("Received: %d\n", b);
    }

    msgQDelete(msgQ);

    wd = wdCreate();

    if (wd == NULL) {
        printf("Failed to create watchdog timer\n");
        return -1;
    }

    int status = wdStart(wd, 200, (FUNCPTR) repeatMe, 420);

    if (status != 0) {
        printf("Failed to start watchdog timer\n");
        return -1;
    }

    taskDelay(6000);

    wdCancel(wd);
    wdDelete(wd);

    printf("task runMe is done!\n");

    return 0;
}

void repeatMe(int a) {
    wdStart(wd, 200, (FUNCPTR) repeatMe, a + 1);

    printf("a: %d\n", a);

    taskDelay(40);
}
