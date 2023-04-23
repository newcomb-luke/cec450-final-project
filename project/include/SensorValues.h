#pragma once

/**
 * Represents the most high-resolution water level that the 
 * controller can see. Produced by the SensorMonitor by
 * reading the WaterLevelSensors
 */
typedef enum {
    WATER_LEVEL_HIGH = 4,
    WATER_LEVEL_MID_HIGH = 3,
    WATER_LEVEL_MID_LOW = 2,
    WATER_LEVEL_LOW = 1,
    WATER_LEVEL_EMPTY = 0
} WaterLevelState;
