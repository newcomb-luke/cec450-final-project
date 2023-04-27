#include "Controller.h"
#include "Effectors.h"
#include "EffectorsMonitor.h"
#include "EffectorsUpdater.h"
#include "SensorReaders.h"
#include "SensorValues.h"
#include "Sensors.h"
#include "SensorsMonitor.h"
#include "Simulator.h"
#include "vector.h"
#include "array.h"
#include "Visualizer.h"

#include <stdio.h>
#include <stdlib.h>
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
ReadersPackage initializeReaders(SensorsPackage sensors);

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
    ReadersPackage readers = initializeReaders(sensors);

    Simulator* simulator = malloc(sizeof(Simulator));
    Simulator_init(simulator, effectors, sensors);
    Simulator_start(simulator);

    ControllerInputs controllerInputs = {
        .visualizer = visualizer,
        .sensors = readers,
        .effectors = effectors
    };

    Controller* controller = malloc(sizeof(Controller));
    Controller_init(controller, controllerInputs);
    Controller_start(controller);

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

ReadersPackage initializeReaders(SensorsPackage sensors) {
    WaterLevelReader* waterLevelReader = malloc(sizeof(WaterLevelReader));
    WaterLevelReader_init(waterLevelReader, sensors.waterLevelSensors);

    SensorReader* temperatureReader = malloc(sizeof(SensorReader));
    SensorReader_init(temperatureReader, sensors.temperatureSensor, TANK_INITIAL_TEMPERATURE);

    SensorReader* pressureReader = malloc(sizeof(SensorReader));
    SensorReader_init(pressureReader, sensors.pressureSensor, TANK_INITIAL_PRESSURE);

    ReadersPackage readers = {
        .waterLevelReader = waterLevelReader,
        .temperatureReader = temperatureReader,
        .pressureReader = pressureReader
    };

    return readers;
}
