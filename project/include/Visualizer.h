#pragma once

#include "DisplayManager.h"
#include "VisualizerMessaage.h"
#include <taskLib.h>
#include <msgQLib.h>

/**
 * Core of the visualization subsytem. Receives visualization updates and 
 * commands the display manager
 */
typedef struct {
    // The display manager that does the drawing
    DisplayManager _displayManager;
    // The message queue of VisualizerMessages
    MSG_Q_ID _updateQueue;
    // The task ID of the visualizer’s internal task
    TASK_ID _visualizerTask;
} Visualizer;

/**
 * Purpose: This method initializes a Visualizer class and acts like a constructor.
 * Pre-Condition: Visualizer is uninitialized.
 * Post-Condition: Visualizer is initialized. displayManager is initialized. updateQueue is not NULL and initialized. visualizerTask is NULL. Visualizer is not started.
 * Returns: none.
 */
void Visualizer_init(Visualizer* this);

/**
 * Purpose: This method starts the Visualizer’s internal task.
 * Pre-Condition: Visualizer is initialized. Visualizer is not started.
 * Post-Condition: Visualizer is started. visualizerTask is not NULL and the task is running.
 * Returns: none.
 */
void Visualizer_start(Visualizer* this);

/**
 * Data: msg - A new message to be sent to the visualization subsystem.
 * Purpose: This method sends a new message to the visualization subsystem if room is available on the queue.
 * Pre-Condition: Visualizer is initialized.
 * Post-Condition: The message has been added to the update message queue if room was available.
 * Returns: none.
 */
void Visualizer_sendMessage(Visualizer* this, const VisualizerMessage* msg);
