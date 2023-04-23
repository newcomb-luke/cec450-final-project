#pragma once

#include "LogMessage.h"
#include "Effectors.h"
#include "SensorValues.h"

/**
 * Represents the “update” type of visualizer message
 */
typedef struct {
    EffectorsStates effectorsStates;
    SensorReadings sensorReadings;
} UpdateMessage;

/**
 * Stores the data contained in a visualizer message based on the type
 */
typedef union {
    LogMessage logMsg;
    UpdateMessage updateMsg;
} VisualizerMessageData;

/**
 * Represents the type of a visualizer message
 */
typedef enum {
    VISUALIZER_LOG_MESSAGE = 0,
    VISUALIZER_UPDATE_MESSAGE = 1
} VisualizerMessageType;

/**
 * Represents a message that can be sent to the visualizer
 */
typedef struct {
    VisualizerMessageType msgType;
    VisualizerMessageData msgData;
} VisualizerMessage;

/**
 * Data: msg - The log message to wrap in a VisualizerMessage.
 * Purpose: This method acts like a constructor for VisualizerMessage taking in a LogMessage and setting the msgType to VISUALIZER_LOG_MESSAGE and msgData to the msg.
 * Pre-Condition: none.
 * Post-Condition: none.
 * Returns: VisualizerMessage - The new message.
 */
VisualizerMessage VisualizerMessage_Log(LogMessage msg);

/**
 * Data: msg - The update message to wrap in a VisualizerMessage.
 * Purpose: This method acts like a constructor for VisualizerMessage taking in a UpdateMessage and setting the msgType to VISUALIZER_UPDATE_MESSAGE and msgData to the msg.
 * Pre-Condition: none.
 * Post-Condition: none.
 * Returns: VisualizerMessage - The new message.
 */
VisualizerMessage VisualizerMessage_Update(UpdateMessage msg);
