
#include "FreeRTOS.h"

#ifndef ARPEGGIATOR_ROTARYENC_H
#define ARPEGGIATOR_ROTARYENC_H

void setupInputs();
void startInputTask(void*);

extern SemaphoreHandle_t i2c_lock;

#endif //ARPEGGIATOR_ROTARYENC_H

