#include "Visualizer.h"
#include "DisplayManager.h"
#include "Effectors.h"
#include "SensorValues.h"
#include <taskLib.h>

#define VISUALIZER_MAX_MESSAGES 200

void _Visualizer_loop(Visualizer* this);

void Visualizer_init(Visualizer* this) {
    DisplayManager_init(&(this->_displayManager));

    this->_updateQueue = msgQCreate(VISUALIZER_MAX_MESSAGES, sizeof(VisualizerMessage), MSG_Q_FIFO);

    this->_visualizerTask = NULL;
}

void Visualizer_start(Visualizer* this) {
    this->_visualizerTask = taskSpawn("visualizerTask", 100, 0, 0x2000, (FUNCPTR) _Visualizer_loop, (size_t) this, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void Visualizer_sendMessage(Visualizer* this, const VisualizerMessage* msg) {
    
}

void _Visualizer_loop(Visualizer* this) {
    printf("Visualizer task started\n");

    DisplayManager_draw(&this->_displayManager);

    int c = 10;
    int open = 0;
    WaterLevelState waterLevel = WATER_LEVEL_EMPTY;
    float temp = 123.0;
    float pressure = 0.0;

    // Infinite loop
    for (;;) {
        taskDelay(40);

        DisplayManager_draw(&this->_displayManager);

        if (c == 0) {
            if (open == 1) {
                DisplayManager_setHeater(&(this->_displayManager), HEATER_ON);
                DisplayManager_setInletValve1(&(this->_displayManager), VALVE_CLOSED);
                DisplayManager_setInletValve2(&(this->_displayManager), VALVE_OPEN);
                DisplayManager_setOutletValve(&(this->_displayManager), VALVE_CLOSED);
            } else {
                DisplayManager_setHeater(&(this->_displayManager), HEATER_OFF);
                DisplayManager_setInletValve1(&(this->_displayManager), VALVE_OPEN);
                DisplayManager_setInletValve2(&(this->_displayManager), VALVE_CLOSED);
                DisplayManager_setOutletValve(&(this->_displayManager), VALVE_OPEN);
            }
            open = !open;

            DisplayManager_setWaterLevel(&(this->_displayManager), waterLevel);
            DisplayManager_setTemperature(&(this->_displayManager), temp);
            DisplayManager_setPressure(&(this->_displayManager), pressure);
            temp -= 5.0;
            pressure += 10.05;

            if (waterLevel == WATER_LEVEL_EMPTY) {
                waterLevel = WATER_LEVEL_LOW;
            } else if (waterLevel == WATER_LEVEL_LOW) {
                waterLevel = WATER_LEVEL_MID_LOW;
            } else if (waterLevel == WATER_LEVEL_MID_LOW) {
                waterLevel = WATER_LEVEL_MID_HIGH;
            } else if (waterLevel == WATER_LEVEL_MID_HIGH) {
                waterLevel = WATER_LEVEL_HIGH;
            } else if (waterLevel == WATER_LEVEL_HIGH) {
                waterLevel = WATER_LEVEL_EMPTY;
            }
        }

        c--;

        if (c < 0) {
            c = 10;
        }
    }
}
