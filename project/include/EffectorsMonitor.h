#pragma once

#include "Effectors.h"

/**
 * Provides an interface for the simulation to read effector states
 */
typedef struct {
    // Stores the effectors that this should read
    EffectorsPackage effectors;
} EffectorsMonitor;

/**
 * Data: effectorsToMonitor - The collection of effectors to be read from.
 * Purpose: This method initializes a EffectorsMonitor class and acts like a constructor.
 * Pre-Condition: EffectorsMonitor is uninitialized.
 * Post-Condition: effectors is initialized to the value of effectorsToMonitor. EffectorsMonitor is initialized.
 * Returns: none.
 */
void EffectorsMonitor_init(EffectorsMonitor* this, EffectorsPackage effectorsToMonitor);

/**
 * Data: states - The collection of effectors states to populate with new data.
 * Purpose: This method reads the states for each of the effectors in the system.
 * Pre-Condition: EffectorsMonitor is initialized.
 * Post-Condition: All effectors in the effectors variable have been read from, and all newDataSem semaphores are empty. states is updated with current states.
 * Returns: none.
 */
void EffectorsMonitor_read(EffectorsMonitor* this, EffectorsStates* states);
