#include <semlib.h>

enum ValveState {
	Closed = 0,
	Open = 1
};

typedef struct Valve {
	ValveState valveState;
	SEM_ID dataMutex;
} Valve;

void Valve_init(Valve* this);
ValveState Valve_read(Valve* this);
void Valve_write(Valve* this, ValveState newValue);

#endif