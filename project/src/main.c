#include "Effectors.h"
#include "EffectorsMonitor.h"
#include "EffectorsUpdater.h"
#include "LogMessage.h"
#include "SensorValues.h"
#include "Sensors.h"
#include "SensorsUpdater.h"
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

#define WATER_LEVEL_SENSOR_LOW_HEIGHT      (10.0)
#define WATER_LEVEL_SENSOR_MID_LOW_HEIGHT  (20.0)
#define WATER_LEVEL_SENSOR_MID_HIGH_HEIGHT (30.0)
#define WATER_LEVEL_SENSOR_HIGH_HEIGHT     (40.0)

#define TANK_INITIAL_TEMPERATURE           (23.5)
#define TANK_INITIAL_PRESSURE              (101.325)

struct timespec main_subtractTimespecs(struct timespec before, struct timespec after);
SensorsPackage initializeSensors();
EffectorsPackage initializeEffectors();

int main() {
    // Necessary for the emulator to start counting ticks
    tickGet();

    struct timespec startTime;

    clock_gettime(CLOCK_REALTIME, &startTime);

    Visualizer* visualizer = malloc(sizeof(Visualizer));
    Visualizer_init(visualizer);
    Visualizer_start(visualizer);

    SensorsPackage sensors = initializeSensors();
    EffectorsPackage effectors = initializeEffectors();

    EffectorsMonitor* effectorsMonitor = malloc(sizeof(EffectorsMonitor));
    EffectorsMonitor_init(effectorsMonitor, effectors);

    EffectorsUpdater* effectorsUpdater = malloc(sizeof(EffectorsUpdater));
    EffectorsUpdater_init(effectorsUpdater, effectors);

    // Infinite loop
    for (;;) {}

    return 0;
}

SensorsPackage initializeSensors() {
    vector waterLevelSensorsVec;
    vector_init(&waterLevelSensorsVec, sizeof(WaterLevelSensor));

    float waterLevelSensorLocations[4] = { WATER_LEVEL_SENSOR_LOW_HEIGHT, 
                                           WATER_LEVEL_SENSOR_MID_LOW_HEIGHT, 
                                           WATER_LEVEL_SENSOR_MID_HIGH_HEIGHT, 
                                           WATER_LEVEL_SENSOR_HIGH_HEIGHT };

    int i;
    for (i = 0; i < 4; i++) {
        WaterLevelSensor sensor;
        WaterLevelSensor_init(&sensor, waterLevelSensorLocations[i]);
        
        vector_push(&waterLevelSensorsVec, &sensor);
    }

    array waterLevelSensors = vector_to_array(&waterLevelSensorsVec);

    Sensor* temperatureSensor = malloc(sizeof(Sensor));
    Sensor_init(temperatureSensor, TANK_INITIAL_TEMPERATURE);

    Sensor* pressureSensor = malloc(sizeof(Sensor));
    Sensor_init(pressureSensor, TANK_INITIAL_PRESSURE);

    SensorsPackage sensors = {
        .waterLevelSensors = waterLevelSensors,
        .temperatureSensor = temperatureSensor,
        .pressureSensor = pressureSensor 
    };

    return sensors;
}

EffectorsPackage initializeEffectors() {
    vector inletValvesVec;
    vector_init(&inletValvesVec, sizeof(Valve));

    Valve inletValve1;
    Valve_init(&inletValve1);
    Valve inletValve2;
    Valve_init(&inletValve2);

    vector_push(&inletValvesVec, &inletValve1);
    vector_push(&inletValvesVec, &inletValve2);

    array inletValves = vector_to_array(&inletValvesVec);

    Valve* outletValve = malloc(sizeof(Valve));
    Valve_init(outletValve);

    Heater* heater = malloc(sizeof(Heater));
    Heater_init(heater);

    EffectorsPackage effectors = {
        .heater = heater,
        .inletValves = inletValves,
        .outletValve = outletValve
    };

    return effectors;
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
