#include "SensorsMonitor.h"

void SensorsMonitor_init(SensorsMonitor* this, ReadersPackage readersToMonitor) {
    this->_readers = readersToMonitor;
}

void SensorsMonitor_start(SensorsMonitor* this) {
    WaterLevelReader_start(this->_readers.waterLevelReader);

    SensorReader_start(this->_readers.temperatureReader);

    SensorReader_start(this->_readers.pressureReader);
}

SensorReadings SensorsMonitor_read(SensorsMonitor* this) {
    SensorReadings readings;

    readings.waterLevelState = WaterLevelReader_read(this->_readers.waterLevelReader);
    readings.temperature = SensorReader_read(this->_readers.temperatureReader);
    readings.pressure = SensorReader_read(this->_readers.pressureReader);

    return readings;
}
