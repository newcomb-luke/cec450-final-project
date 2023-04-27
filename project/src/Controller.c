#include "Controller.h"
#include "Effectors.h"
#include "EffectorsUpdater.h"
#include "LogMessage.h"
#include "SensorValues.h"
#include "SensorsMonitor.h"
#include "TemperatureController.h"
#include "Visualizer.h"
#include "VisualizerMessaage.h"
#include "WaterFlowController.h"
#include "Simulator.h"
#include "array.h"
#include "tickLib.h"
#include "vector.h"
#include <vxworks.h>
#include <taskLib.h>

#define CONTROLLER_TASK_PRIORITY (100)
#define CONTROLLER_TASK_DELAY    (20)

void _Controller_loop(Controller* this);

EffectorsStates initializeEffectorsStates();
void copyEffectorsStates(EffectorsStates* dest, EffectorsStates* src);

void Controller_init(Controller* this, ControllerInputs inputs) {
    SensorsMonitor_init(&(this->_sensorsMonitor), inputs.sensors);
    EffectorsUpdater_init(&(this->_effectorsUpdater), inputs.effectors);

    this->_visualizer = inputs.visualizer;

    TemperatureController_init(&(this->_temperatureController));
    WaterFlowController_init(&(this->_waterFlowController));

    this->_taskID = NULL;
}

void Controller_start(Controller* this) {

    SensorsMonitor_start(&(this->_sensorsMonitor));

    this->_taskID = taskSpawn("controller",
                              CONTROLLER_TASK_PRIORITY,
                              0, 0x2000, 
                              (FUNCPTR) _Controller_loop, (size_t) this,
                              0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void _Controller_loop(Controller* this) {
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    // Initialize our EffectorsStates variable
    EffectorsStates effectorStates = initializeEffectorsStates();
    EffectorsStates previousEffectorStates = initializeEffectorsStates();

    WaterLevelState previousWaterLevelState = WATER_LEVEL_EMPTY;

    // Start filling a bit
    EffectorsUpdater_write(&(this->_effectorsUpdater), effectorStates);

    // Infinite loop
    for (;;) {
        struct timespec nowAbs;
        clock_gettime(CLOCK_REALTIME, &nowAbs);

        struct timespec now = subtractTimespecs(start, nowAbs);
        unsigned long nowTicks = tickGet();

        SensorReadings sensorReadings = SensorsMonitor_read(&(this->_sensorsMonitor));

        TemperatureController_update(&(this->_temperatureController), sensorReadings.temperature, &(effectorStates.heaterState));

        WaterFlowController_update(&(this->_waterFlowController), 
                                   sensorReadings.pressure, 
                                   sensorReadings.waterLevelState,
                                   &(effectorStates.valveStates));

        EffectorsUpdater_write(&(this->_effectorsUpdater), effectorStates);

        LogMessageTimestamp msgTimestamp = {
            .seconds = now,
            .ticks = nowTicks
        };

        if (previousEffectorStates.heaterState != effectorStates.heaterState) {
            LogMessageHeaterAction action;

            if (effectorStates.heaterState == HEATER_ON) {
                action = ACTION_HEATER_ON;
            } else {
                action = ACTION_HEATER_OFF;
            }

            LogMessage logMsg = LogMessage_Heater(action, msgTimestamp);
            VisualizerMessage visMsg = VisualizerMessage_Log(logMsg);
            Visualizer_sendMessage(this->_visualizer, &visMsg);
        }

        if (previousEffectorStates.valveStates.outletValveState != effectorStates.valveStates.outletValveState) {
            LogMessageValveAction action;

            if (effectorStates.valveStates.outletValveState == VALVE_OPEN) {
                action = ACTION_VALVE_OPEN;
            } else {
                action = ACTION_VALVE_CLOSE;
            }

            LogMessage logMsg = LogMessage_Valve(OUTLET_VALVE_ID, action, msgTimestamp);
            VisualizerMessage visMsg = VisualizerMessage_Log(logMsg);
            Visualizer_sendMessage(this->_visualizer, &visMsg);
        }

        int i;
        const int numInletValves = array_size(&(effectorStates.valveStates.inletValveStates));
        for (i = 0; i < numInletValves; i++) {
            ValveState* newValveState = array_get(&(effectorStates.valveStates.inletValveStates), i);
            ValveState* previousValveState = array_get(&(previousEffectorStates.valveStates.inletValveStates), i);

            if (*newValveState != *previousValveState) {
                LogMessageValveAction action;

                if (*newValveState == VALVE_OPEN) {
                    action = ACTION_VALVE_OPEN;
                } else {
                    action = ACTION_VALVE_CLOSE;
                }

                LogMessageValveID valveID;

                if (i == 0) {
                    valveID = INLET_VALVE_1_ID;
                } else {
                    valveID = INLET_VALVE_2_ID;
                }

                LogMessage logMsg = LogMessage_Valve(valveID, action, msgTimestamp);
                VisualizerMessage visMsg = VisualizerMessage_Log(logMsg);
                Visualizer_sendMessage(this->_visualizer, &visMsg);
            }
        }

        if (sensorReadings.waterLevelState != previousWaterLevelState) {
            LogMessageWaterSensorID sensorID;
            LogMessageWaterSensorAction action;

            WaterLevelState prev = previousWaterLevelState;
            WaterLevelState new = sensorReadings.waterLevelState;

            if (new == WATER_LEVEL_LOW && prev == WATER_LEVEL_EMPTY) {
                sensorID = WATER_SENSOR_1_ID;
                action = ACTION_WATER_SENSOR_ROSE_PAST;
            } else if (new == WATER_LEVEL_MID_LOW && prev == WATER_LEVEL_LOW) {
                sensorID = WATER_SENSOR_2_ID;
                action = ACTION_WATER_SENSOR_ROSE_PAST;
            } else if (new == WATER_LEVEL_MID_HIGH && prev == WATER_LEVEL_MID_LOW) {
                sensorID = WATER_SENSOR_3_ID;
                action = ACTION_WATER_SENSOR_ROSE_PAST;
            } else if (new == WATER_LEVEL_HIGH && prev == WATER_LEVEL_MID_HIGH) {
                sensorID = WATER_SENSOR_4_ID;
                action = ACTION_WATER_SENSOR_ROSE_PAST;
            } else if (new == WATER_LEVEL_MID_HIGH && prev == WATER_LEVEL_HIGH) {
                sensorID = WATER_SENSOR_4_ID;
                action = ACTION_WATER_SENSOR_DROPPED_PAST;
            } else if (new == WATER_LEVEL_MID_LOW && prev == WATER_LEVEL_MID_HIGH) {
                sensorID = WATER_SENSOR_3_ID;
                action = ACTION_WATER_SENSOR_DROPPED_PAST;
            } else if (new == WATER_LEVEL_LOW && prev == WATER_LEVEL_MID_LOW) {
                sensorID = WATER_SENSOR_2_ID;
                action = ACTION_WATER_SENSOR_DROPPED_PAST;
            } else if (new == WATER_LEVEL_EMPTY && prev == WATER_LEVEL_LOW) {
                sensorID = WATER_SENSOR_1_ID;
                action = ACTION_WATER_SENSOR_DROPPED_PAST;
            }

            LogMessage logMsg = LogMessage_WaterSensor(sensorID, action, msgTimestamp);
            VisualizerMessage visMsg = VisualizerMessage_Log(logMsg);
            Visualizer_sendMessage(this->_visualizer, &visMsg);
        }

        previousWaterLevelState = sensorReadings.waterLevelState;

        UpdateMessage updateMessage = {
            .sensorReadings = sensorReadings,
            .effectorsStates = effectorStates
        };

        copyEffectorsStates(&previousEffectorStates, &effectorStates);

        VisualizerMessage visMsg = VisualizerMessage_Update(updateMessage);
        Visualizer_sendMessage(this->_visualizer, &visMsg);
        
        taskDelay(CONTROLLER_TASK_DELAY);
    }

    for (;;) {}
}

EffectorsStates initializeEffectorsStates() {
    vector inletValveStatesVec;
    vector_init(&inletValveStatesVec, sizeof(ValveState));

    ValveState open = VALVE_OPEN;
    vector_push(&inletValveStatesVec, &open);
    vector_push(&inletValveStatesVec, &open);

    EffectorsStates effectorStates;
    effectorStates.valveStates.inletValveStates = vector_to_array(&inletValveStatesVec);
    effectorStates.heaterState = HEATER_OFF;
    effectorStates.valveStates.outletValveState = VALVE_CLOSED;

    return effectorStates;
}

void copyEffectorsStates(EffectorsStates* dest, EffectorsStates* src) {
    dest->heaterState = src->heaterState;
    dest->valveStates.outletValveState = src->valveStates.outletValveState;
    
    int i;
    const int numInletValves = array_size(&dest->valveStates.inletValveStates);
    for (i = 0; i < numInletValves; i++) {
        ValveState* destState = array_get(&(dest->valveStates.inletValveStates), i);
        ValveState* srcState = array_get(&(src->valveStates.inletValveStates), i);

        *destState = *srcState;
    }
}
