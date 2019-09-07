/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 19/08/2019.
 */

#ifndef ARPEGGIATOR_TEMPOTIMER_H
#define ARPEGGIATOR_TEMPOTIMER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define BPM_PRECISION 128
extern int BPM;

void setupTempoTimer(TaskHandle_t task);
void setBPM(int val);

#endif //ARPEGGIATOR_TEMPOTIMER_H
