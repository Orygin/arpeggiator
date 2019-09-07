/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 19/08/2019.
 */

#include "tempoTimer.h"
#include "acommon.h"
#include "Sequencor.h"

int BPM;

TaskHandle_t xTaskToWake;

void tempoTimerISR(void*arg)
{
    // Advance tempo
    xTaskNotifyGive(xTaskToWake);
    //  reset alarm
    timer_set_alarm(TIMER_GROUP_TEMPO, TIMER_ALARM_EN);
    TIMERG0.int_clr_timers.t1 = 1;
}
void setupTempoTimer(TaskHandle_t task) {
    xTaskToWake = task;
    BPM = DEFAUTLT_BPM;
    timer_config_t config = {
            .alarm_en = true,				//Alarm Enable
            .counter_en = false,			//If the counter is enabled it will start incrementing / decrementing immediately after calling timer_init()
            .intr_type = TIMER_INTR_LEVEL,	//Is interrupt is triggered on timer’s alarm (timer_intr_mode_t)
            .counter_dir = TIMER_COUNT_UP,	//Does counter increment or decrement (timer_count_dir_t)
            .auto_reload = true,			//If counter should auto_reload a specific initial value on the timer’s alarm, or continue incrementing or decrementing.
            .divider = 80   				//Divisor of the incoming 80 MHz (12.5nS) APB_CLK clock. E.g. 80 = 1uS per timer tick
    };
    int tempoRate = (60000000/(BPM*BPM_PRECISION));

    timer_init(TIMER_GROUP_TEMPO, &config);
    timer_set_counter_value(TIMER_GROUP_TEMPO, 0);
    timer_set_alarm_value(TIMER_GROUP_TEMPO, tempoRate);
    timer_group_intr_enable(TIMER_GROUP_TEMPO);
    timer_isr_register(TIMER_GROUP_TEMPO, tempoTimerISR, NULL, 0, NULL);
    timer_set_alarm(TIMER_GROUP_TEMPO, TIMER_ALARM_EN);

    timer_start(TIMER_GROUP_TEMPO);
}

void setBPM(int nbpm)
{
    BPM = nbpm;
    int tempoRate = (60000000/BPM*BPM_PRECISION);
    timer_set_alarm_value(TIMER_GROUP_TEMPO, tempoRate);
}