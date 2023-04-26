#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <Simulator.h>
#include <PressureSim.h>

#define     TANK_RADIUS             (17.84124)
#define     TANK_HEIGHT             (50) 
#define     BASE_TANK_PRESSURE      (13)
#define     EXPLOSION_TANK_PRESSURE (360)
#define     WATER_BOILING_POINT     (100)
#define     SIMULATION_LOOP_DELAY   (5)
#define     TANK_VOLUME             (((M_PI * TANK_RADIUS * TANK_RADIUS) * TANK_HEIGHT) / 1000)
#define     INLET_VALVE_RATE        (0.5)
#define     OUTLET_VALVE_RATE       (0.5)
#define     INLET_WATER_TEMP        (20.0)

// void Simulator_init(EffectorsPackage effectors, SensorsPackage sensors)
// Data: effectors - The effectors this simulation will read as input.
// sensors - The sensors this simulation will update as output.
// Purpose: This method initializes a Simulator class and acts like a constructor.
// Pre-Condition: Simulator is uninitialized.
// Post-Condition: effectorsMonitor is initialized using effectors. sensorsUpdater is initialized using sensors. pressureSim is initialized. waterLevel is set to 0.0. taskID is NULL. Simulator is initialized. Simulator is not started.
// Returns: none.
void Simulator_init(Simulator* this, EffectorsPackage effectors, SensorePackage sensors){
    EffectorsMonitor_init(&this->_effectorsMonitor, effectors);
    SensorsUpdated_init(&this->_sensorsUpdater,sensors);
    PressureSim_init(this-> _pressureSim)
    this->_waterLevel=0.0;
    this->_taskID=NULL;
}

// void Simulator_start()
// Purpose: This method starts the simulator’s internal task.
// Pre-Condition: Simulator is initialized. Simulator is not started.
// Post-Condition: taskID is not NULL. Simulator is started.
// Returns: none.
void Simulator_start(Simulator* this){
    _Simulator_loop();
    this->_taskID=taskSpawn("simulator");
}

// void _Simulator_loop()
// Purpose: This method is the Simulator’s main loop that is the entry point of a task.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: never.
int _Simulator_loop(Simulator* this){
   
    float temperature = WATER_INLET_TEMP;
    float waterVolume = 0;
    float lastTimestamp = clock_gettime();

    while (true) {
        float currentTime = clock_gettime();
        float deltaTime = currentTime - lastTimestamp;
        float lastTimestamp = currentTime;

        float inletRate = calculateInletRate();//add valve
        float inletWater = inletRate * deltaTime;

        float inletMass = calculateMassOfWater(inletWater);

        float waterTemp = _Simulutor_calculateMixedWaterTemp(this, temperature, inletMass);

        waterVolume = waterVolume + inletWater;
        waterVolume = max(min(waterVolume, TANK_VOLUME), 0);
        
        float pressure = PressureSim_Update(&this->PressureSim* this)+ BASE_TANK_PRESSURE;
        
        
        float tempDelta = calculateHeaterTempDelta(heaterState, deltaTime, waterMass);//fix not bool
        waterTemp = min(waterTemp + tempDelta, WATER_BOILING_POINT);
        
        SensorUpdates sensorUpdates;
        sensorUpdates.waterLevel = waterVolume;
        sensorUpdates.temperature = temperature;
        sensorUpdates.pressure = pressure;
        SensorsUpdater_write(&this->sensorsUpdater,sensorUpdates);

        if (tankPressure > EXPLOSION_TANK_PRESSURE){
            sendExplodedSignal();
            break;
        }

        taskDelay(SIMULATION_LOOP_DELAY);  
    }
    while (true){

     }
     return 0;
}
// float _Simulator_calculateInletRate()
// Purpose: This method calculates the inlet rate of water into the tank using valve states.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: float - the inlet rate in Liters per second.
double calculateInletRate(ValveStates states){
    

    double inletRate = 0;

    for(int i = 0; i<= N - 1; i++){ 
	    if (inletValves[i].isOpen() ){
		    inletRate = inletRate + INLET_VALVE_RATE;
        }
    }

    if ( outletValve.isOpen() ) {
        inletRate = inletRate - OUTLET_VALVE_RATE;
    }
    return inletRate;

}
// float _Simulator_calculateMixedWaterTemp(float tankTemperature, float inletMass)
// Data: tankTemperature - The temperature of the water in the tank in degrees Celsius
// inletMass - The mass of the current amount of water that is entering the tank.
// Purpose: This method calculates the final temperature of mixing inlet water into the tank.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: float - the final temperature after mixing.
float _Simulator_calculateMixedWaterTemp(Simulator* this, float tankTemperature, float inletMass){

   float newTankTemp = tankTemperature;
   float tankMass = calculateMassOfWater(this->_waterLevel)
    if (inletMass > 0 && tankMass > 0) {
	   double kgDegreesK = tankMass * tankTemperature + inletMass * INLET_WATER_TEMP;
	    newTankTemp = kgDegreesK / (tankMass + inletMass);
    }

    return newTankTemp;
}
// float _Simulator_calculateWaterHeating()
// Purpose: This method calculates the change in water temperature for this next update.
// Pre-Condition: Simulator is initialized. Simulator is started.
// Post-Condition: none.
// Returns: float - the change in water temperature due to the heater.

double calculateMassOfWater(double waterVolume){

    int LITERS_PER_CUBIC_METER = 1000;
    int DENSITY_OF_WATER = 1000;

    double volumeCubicMeters = waterVolume / LITERS_PER_CUBIC_METER;
    double waterMass = volumeCubicMeters * DENSITY_OF_WATER;

    return waterMass;
}


double calculateHeaterTempDelta(HeaterState heaterState, double deltaTime, double waterMass){

    int HEATER_WATTAGE = 4000;
    int WATER_SPECIFIC_HEAT=100;
    double deltaTemp = 0;

    if (heaterState == HEATER_ON) {
        deltaTemp = (deltaTime * HEATER_WATTAGE) / (waterMass * WATER_SPECIFIC_HEAT);
    }

    return deltaTemp;
}

