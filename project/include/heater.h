#include <semlib.h>

enum HeaterState {
	Off = 0,
	On = 1
};

typedef struct Heater {
	HeaterState heaterState;
	SEM_ID dataMutex;
} Heater;

void Heater_init(Heater* this);
HeaterState Heater_read(Heater* this);
void Heater_write(Heater* this, HeaterState newValue);

#endif