/**
 * Arpeggo project header file
 * License GPLv3
 * by Orygin (Louis Geuten)
*/

/**
 * @brief Selects a fixed hardware timer group and id for each components needing a timer
 */
#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define DEFAUTLT_BPM 120

#define TIMER_GROUP_MIDI TIMER_GROUP_0,TIMER_0
#define TIMER_GROUP_TEMPO TIMER_GROUP_0,TIMER_1
#define TIMER_GROUP_IO TIMER_GROUP_1,TIMER_0