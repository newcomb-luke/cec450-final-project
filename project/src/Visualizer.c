#include "Visualizer.h"
#include "DisplayManager.h"
#include "Effectors.h"
#include "LogMessage.h"
#include "SensorValues.h"
#include "VisualizerMessaage.h"
#include <vxworks.h>
#include <msgQLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <stdbool.h>

#define NANOSECONDS_PER_SECOND (1000000000)

#define VISUALIZER_MAX_MESSAGES (200)

#define VISUALIZER_TASK_PRIORITY (100)

#define VISUALIZER_TASK_DELAY          (5)
#define VISUALIZER_SUBSEQUENT_TIMEOUT  (5)
#define VISUALIZER_MAX_SUBSEQUENT_MSGS (2)

void _Visualizer_loop(Visualizer* this);
void _Visualizer_handleMessage(Visualizer* this, const VisualizerMessage* msg);

void Visualizer_init(Visualizer* this) {
    DisplayManager_init(&(this->_displayManager));

    this->_updateQueue = msgQCreate(VISUALIZER_MAX_MESSAGES, sizeof(VisualizerMessage), MSG_Q_FIFO);

    this->_visualizerTask = NULL;
}

void Visualizer_start(Visualizer* this) {
    this->_visualizerTask = taskSpawn("visualizerTask", 
                                      VISUALIZER_TASK_PRIORITY, 
                                      0, 
                                      0x2000, 
                                      (FUNCPTR) _Visualizer_loop, 
                                      (size_t) this, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void Visualizer_sendMessage(Visualizer* this, const VisualizerMessage* msg) {
    msgQSend(this->_updateQueue, 
             (void*) msg, 
             sizeof(VisualizerMessage),
             NO_WAIT, MSG_PRI_NORMAL);
}

void _Visualizer_loop(Visualizer* this) {

    DisplayManager_draw(&this->_displayManager);

    // Infinite loop
    for (;;) {
        VisualizerMessage currentMessage;

        // Wait for a new message
        msgQReceive(this->_updateQueue, (char*) &currentMessage, sizeof(VisualizerMessage), WAIT_FOREVER);

        _Visualizer_handleMessage(this, &currentMessage);

        // Just in case we are being sent multiple, try to read more with a little bit of a timeout
        int i;
        for (i = 0; i < VISUALIZER_MAX_SUBSEQUENT_MSGS; i++) {
            int success = msgQReceive(this->_updateQueue, (char*) &currentMessage, sizeof(VisualizerMessage), VISUALIZER_SUBSEQUENT_TIMEOUT);

            if (success != 0) {
                break;
            }

            // If we did get a message
            _Visualizer_handleMessage(this, &currentMessage);
        }

        // If we are out of messages, redraw
        DisplayManager_draw(&this->_displayManager);

        taskDelay(VISUALIZER_TASK_DELAY);
    }
}

void _Visualizer_handleMessage(Visualizer* this, const VisualizerMessage* msg) {

    if (msg->msgType == VISUALIZER_LOG_MESSAGE) {
        const int logTextMaxStrLen = 40;
        LogMessage logMsg = msg->msgData.logMsg;

        char textBuffer[logTextMaxStrLen + 1];

        if (logMsg.logMsgType == LOG_MESSAGE_WATER_SENSOR) {
            WaterSensorLogMessage wsMsg = logMsg.logMsgData.waterSensorMsg;

            int sensorNum = -1;

            switch (wsMsg.sensor) {
                case WATER_SENSOR_1_ID:
                    sensorNum = 1;
                    break;
                case WATER_SENSOR_2_ID:
                    sensorNum = 2;
                    break;
                case WATER_SENSOR_3_ID:
                    sensorNum = 3;
                    break;
                case WATER_SENSOR_4_ID:
                    sensorNum = 4;
                    break;
            }

            const char* actionName = "invalid";

            switch (wsMsg.action) {
                case ACTION_WATER_SENSOR_ROSE_PAST:
                    actionName = "rose past";
                    break;
                case ACTION_WATER_SENSOR_DROPPED_PAST:
                    actionName = "dropped below";
                    break;
            }

            snprintf(textBuffer, logTextMaxStrLen, "Water %s water sensor %d", actionName, sensorNum);

        } else if (logMsg.logMsgType == LOG_MESSAGE_HEATER) {
            HeaterLogMessage hMsg = logMsg.logMsgData.heaterMsg;

            const char* actionName = "invalid";

            switch (hMsg.action) {
                case ACTION_HEATER_ON:
                    actionName = "on";
                    break;
                case ACTION_HEATER_OFF:
                    actionName = "off";
                    break;
            }

            snprintf(textBuffer, logTextMaxStrLen, "Heater was turned %s", actionName);

        } else if (logMsg.logMsgType == LOG_MESSAGE_VALVE) {
            ValveLogMessage vMsg = logMsg.logMsgData.valveMsg;

            const char* valveName = "invalid";

            switch (vMsg.valve) {
                case INLET_VALVE_1_ID:
                    valveName = "Inlet valve 1";
                    break;
                case INLET_VALVE_2_ID:
                    valveName = "Inlet valve 2";
                    break;
                case OUTLET_VALVE_ID:
                    valveName = "Outlet valve";
                    break;
            }

            const char* actionName = "invalid";

            switch (vMsg.action) {
                case ACTION_VALVE_OPEN:
                    actionName = "opened";
                    break;
                case ACTION_VALVE_CLOSE:
                    actionName = "closed";
            }

            snprintf(textBuffer, logTextMaxStrLen, "%s %s", valveName, actionName);

        }

        const int logMsgMaxStrLen = 63;
        char msgBuffer[logMsgMaxStrLen + 1];

        float seconds = (double)(logMsg.msgTimestamp.seconds.tv_sec) + ( (double)(logMsg.msgTimestamp.seconds.tv_nsec) / (double)(NANOSECONDS_PER_SECOND) ) ;

        snprintf(msgBuffer, logMsgMaxStrLen, "%.4fs (%lu ticks):\n   %s", seconds, logMsg.msgTimestamp.ticks, textBuffer);

        DisplayManager_logMessage(&(this->_displayManager), msgBuffer);

    } else if (msg->msgType == VISUALIZER_UPDATE_MESSAGE) {
        UpdateMessage updateMsg = msg->msgData.updateMsg;

        SensorReadings readings = updateMsg.sensorReadings;
        EffectorsStates states = updateMsg.effectorsStates;

        DisplayManager_setWaterLevel(&(this->_displayManager), readings.waterLevelState);
        DisplayManager_setPressure(&(this->_displayManager), readings.pressure);
        DisplayManager_setTemperature(&(this->_displayManager), readings.temperature);

        DisplayManager_setHeater(&(this->_displayManager), states.heaterState);
        DisplayManager_setOutletValve(&(this->_displayManager), states.valveStates.outletValveState);

        DisplayManager_setInletValve1(&(this->_displayManager), *(ValveState*)(array_get(&states.valveStates.inletValveStates, 0)));
        DisplayManager_setInletValve2(&(this->_displayManager), *(ValveState*)(array_get(&states.valveStates.inletValveStates, 1)));
    }
}
