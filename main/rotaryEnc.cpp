
#include <stdio.h>
#include "rotaryEnc.h"
#include "driver/timer.h"
#include "acommon.h"
#include "ClickEncoder.hpp"

#define ENCODER_PINA     13
#define ENCODER_PINB     14
#define ENCODER_BTN      0

#define ENCODER_STEPS_PER_NOTCH    4

ClickEncoder encoder = ClickEncoder(ENCODER_PINA,ENCODER_PINB,ENCODER_BTN,ENCODER_STEPS_PER_NOTCH);

QueueHandle_t rotQueue;
int interrupt = 1;
void rotaryEncInterrupt(void*arg)
{
    //printf("Interrupt");
    //(rotQueue != NULL)
    xQueueSendFromISR(rotQueue, (void*)&interrupt, pdFALSE);
    timer_set_alarm(TIMER_GROUP_IO, TIMER_ALARM_EN);
    TIMERG1.int_clr_timers.t0 = 1;
}

void setupTestRotaryEnc(void*unused) {

    // Setup Encoder
    encoder.setAccelerationEnabled(false);

    // Setup Time queue that triggers encoder scan service
    rotQueue = xQueueCreate(10, sizeof(int));
    if (rotQueue == NULL)
    {
        printf("ERROR !");
        return;
    }
#ifdef INCLUDE_vTaskSuspend
    printf("Setting  timer!\n");
#endif

    timer_config_t config = {
            .alarm_en = true,				//Alarm Enable
            .counter_en = false,			//If the counter is enabled it will start incrementing / decrementing immediately after calling timer_init()
            .intr_type = TIMER_INTR_LEVEL,	//Is interrupt is triggered on timer’s alarm (timer_intr_mode_t)
            .counter_dir = TIMER_COUNT_UP,	//Does counter increment or decrement (timer_count_dir_t)
            .auto_reload = true,			//If counter should auto_reload a specific initial value on the timer’s alarm, or continue incrementing or decrementing.
            .divider = 80   				//Divisor of the incoming 80 MHz (12.5nS) APB_CLK clock. E.g. 80 = 1uS per timer tick
    };
    timer_init(TIMER_GROUP_IO, &config);
    timer_set_counter_value(TIMER_GROUP_IO, 0);
    timer_set_alarm_value(TIMER_GROUP_IO, 1000000);
    timer_group_intr_enable(TIMER_GROUP_IO);
    timer_isr_register(TIMER_GROUP_IO, rotaryEncInterrupt, NULL, 0, NULL);
    timer_set_alarm(TIMER_GROUP_IO, TIMER_ALARM_EN);

    timer_start(TIMER_GROUP_IO);

    for (;;)
    {
        int tick;

        xQueueReceive(rotQueue, &(tick), portMAX_DELAY);
        //encoder.service();
        printf("hello\n");
        //vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}