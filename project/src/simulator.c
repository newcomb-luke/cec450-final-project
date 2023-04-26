#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <simulator.h>



typedef void* TASK_ID;

typedef struct {
    EffectorsMonitor effectorsMonitor;
    SensorsUpdater sensorsUpdater;
    EffectorStates latestEffectorStates;
    PressureSim pressureSim;
    float waterLevel;
    TASK_ID taskID;
} simulator;

int main(int N, int k,double tankTemp, double inletTemp){
    int inletValves[N];
    double waterLevelSesnors[k]; 
    double TANK_RADIUS = 17.84124;
    int TANK_HEIGHT = 50;
    double WATER_INLET_TEMP=50;
    double TANK_VOLUME = ((M_PI * TANK_RADIUS * TANK_RADIUS) * TANK_HEIGHT) / 1000;
    int BASE_TANK_PRESSURE = 130;
    int EXPLOSION_TANK_PRESSURE = 360;
    int WATER_BOILING_POINT = 100;
    int SIMULATION_LOOP_DELAY = 5;
    double waterTemp = WATER_INLET_TEMP;
    double waterVolume = 0;
    double pressureFromTemperature = 0;
    double lastTimestamp = clock_gettime();

    while (true) {
        double currentTime = clock_gettime();
        double deltaTime = currentTime - lastTimestamp;
        double lastTimestamp = currentTime;

        double inletRate = calculateInletRate(inletValves, outletValve);
        double inletWater = inletRate * deltaTime;

        double tankMass = calculateMassOfWater(waterVolume);
        double inletMass = calculateMassOfWater(inletWater);

        double waterTemp = calculateMixedWaterTemp(tankMass, inletMass, tankTemp, inletTemp);

        double waterVolume = waterVolume + inletWater;
        double waterVolume = max(min(waterVolume, TANK_VOLUME), 0);
        
        double pressureFromVolume = calculatePressureFromVolume(tankVolume);
        pressureFromTemperature = calculatePressureFromTemperature(
            pressureFromTemperature, deltaTime, waterTemp);
        
        pressureFromVolume, pressureFromTemperature = calculateOutletValvePressureRelief(outletValve,
            pressureFromVolume, pressureFromTemperature);
        
        double tankPressure = pressureFromVolume + pressureFromTemperature + BASE_TANK_PRESSURE;
        
        double tempDelta = calculateHeaterTempDelta(heaterState, deltaTime, waterMass);
        waterTemp = min(waterTemp + tempDelta, WATER_BOILING_POINT);

        pressureSensor.set(tankPressure);
        temperatureSensor.set(waterTemp);

        for (int i = 0; i<k - 1; i++){
            levelSensors[i].update(waterVolume);
        }

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

 double calculateInletRate(int N){
    double INLET_VALVE_RATE = 0.5;
    double OUTLET_VALVE_RATE = 0.5;

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

double calculateMassOfWater(double waterVolume){

    int LITERS_PER_CUBIC_METER = 1000;
    int DENSITY_OF_WATER = 1000;

    double volumeCubicMeters = waterVolume / LITERS_PER_CUBIC_METER;
    double waterMass = volumeCubicMeters * DENSITY_OF_WATER;

    return waterMass;
}

double calculateMixedWaterTemp(double tankMass, double inletMass, double tankTemp, double inletTemp){

   double newTankTemp = tankTemp;

    if (inletMass > 0 && tankMass > 0) {
	   double kgDegreesK = tankMass * tankTemp + inletMass * inletTemp;
	    newTankTemp = kgDegreesK / (tankMass + inletMass);
    }

    return newTankTemp;
}

 double calculatePressureFromVolume(double waterVolume){

int SECOND_TO_HIGHEST_VOLUME = 30;
int SCALE_FACTOR = 6;

if (waterVolume > SECOND_TO_HIGHEST_VOLUME){
	double emptyVolume = waterVolume - SECOND_TO_HIGHEST_VOLUME;
	return emptyVolume * SCALE_FACTOR;
}

return 0;
}

double calculatePressureFromTemperature(double pressureFromTempurature, double deltaTime, double waterTemp){

    int EVAPORATION_FACTOR = 2;
    int EVAPORATION_START_TEMP = 85;

    double tempDiff = waterTemp - EVAPORATION_START_TEMP;
    double evaporationPressureDiff = tempDiff * EVAPORATION_FACTOR * deltaTime;
    double pressureFromTemperature = pressureFromTemperature + evaporationPressureDiff;

    return max(pressureFromTemperature, 0);
}

double calculateOutletValvePressureRelief(double outletValve, double pressureFromVolume, double pressureFromTemperature){

    int OUTLET_VALVE_PRESSURE_RELIEF_RATE = 2;

    if (outletValve.isOpen() ){
        pressureFromTemperature = max(pressureFromTemperature - 
    OUTLET_VALVE_PRESSURE_RELIEF_RATE, 0);
        pressureFromVolume = max(pressureFromVolume -
                        OUTLET_VALVE_PRESSURE_RELIEF_RATE, 0);
    }

    return (pressureFromVolume, pressureFromTemperature);
}

double calculateHeaterTempDelta(bool heaterState, double deltaTime, double waterMass){

    int HEATER_WATTAGE = 4000;
    int WATER_SPECIFIC_HEAT=100;
    double deltaTemp = 0;

    if (heaterState = true) {
        deltaTemp = (deltaTime * HEATER_WATTAGE) / (waterMass * WATER_SPECIFIC_HEAT);
    }

    return deltaTemp;
}

