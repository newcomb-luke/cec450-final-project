#pragma once

#include "Effectors.h"

/**
 * Provides an interface for the controller to update effector states
 */
typedef struct {
    // The collection of effectors this is responsible for updating
    EffectorsPackage effectors;
    // The previous effector states, used to compare to the new ones in 
    // order to prevent duplicate data from being written
    EffectorsStates previousStates;
} EffectorsUpdater;

/**
 * Data: effectorsToUpdate - The collection of effectors that this updater is responsible for.
 * Purpose: This method initializes a EffectorsUpdater class and acts like a constructor.
 * Pre-Condition: EffectorsUpdater is uninitialized.
 * Post-Condition: effectors is initialized to effectorsToUpdate, and previousStates is set to CLOSED for valves and OFF for the heater. EffectorsUpdater is initialized.
 * Returns: none.
 */
void EffectorsUpdater_init(EffectorsUpdater* this, EffectorsPackage effectorsToUpdate);

/**
 * Data: newStates - The new set of effector states to write to each effector.
 * Purpose: This method updates the states for each of the effectors in the system.
 * Pre-Condition: EffectorsUpdater is initialized.
 * Post-Condition: previousStates is set to the values of newStates. If effector states changed, then they are updated.
 * Returns: none.
 */
void EffectorsUpdater_write(EffectorsUpdater* this, EffectorsStates newStates);
