//#define ENC_DECODER (1 << 2)

#include <stdio.h>
#include "inputTask.h"
#include "driver/timer.h"
#include "acommon.h"
#include "ClickEncoder.hpp"
#include "Arduino.h"
#include "menuController.h"
#include "Adafruit_MCP23017.h"

SemaphoreHandle_t i2c_lock;

// PIN A, PIN B, PIN BTN, STEPS PR NOTCH, ACTIVE LOW
#define MENU_ENCODER1_SETUP     39, 36, 34, 4,  true
#define MENU_ENCODER2_SETUP     0, 8, 1, 2,  false, &mcp
#define MAX_ENCODERS 1

Adafruit_MCP23017 mcp;

ClickEncoder encoders[MAX_ENCODERS] = {
        // Menu encoder
        ClickEncoder(MENU_ENCODER2_SETUP),
        // Encoders 1 to 5
        /*NULL,
        NULL,
        NULL,
        NULL,
        NULL*/
};

QueueHandle_t rotQueue;
int interrupt = 1;
void rotaryEncInterrupt(void*arg)
{
    // Service encoders
    for (int i=0; i<MAX_ENCODERS; i++)
    {
        //encoders[i].service();
    }

    // Send back tick to task and reset alarm
    xQueueSendFromISR(rotQueue, (void*)&interrupt, pdFALSE);
    timer_set_alarm(TIMER_GROUP_IO, TIMER_ALARM_EN);
    TIMERG1.int_clr_timers.t0 = 1;
}

void processInputs() {
    int delta, btn;
    for (uint8_t i=0; i<MAX_ENCODERS; i++)
    {
        delta = encoders[i].getValue();
        if (delta != 0){
            printf("Delta %d\n", delta);
            inputUpdateNotice notice = {INPUT_ENCODER_TURN, i, delta};
            menuSendToInputQueue(notice);
        }

        btn = (int)encoders[i].getButton();
        if (btn != 0)
        {
            printf("button %d\n", btn);
            inputUpdateNotice notice = {INPUT_ENCODER_BUTTON, i, btn};
            menuSendToInputQueue(notice);
        }
    }
}

void setupInputs() {
    // Setup IO
    pinMode(21, INPUT_PULLUP);
    pinMode(22, INPUT_PULLUP);
    mcp.begin();
    mcp.pullUp(1, HIGH);
    mcp.pullUp(0, HIGH);
    mcp.pullUp(8, HIGH);

    // Prepare i2c lock
    i2c_lock = xSemaphoreCreateBinary();
    xSemaphoreGive(i2c_lock);

    // Setup Encoders
    xSemaphoreTake(i2c_lock, portMAX_DELAY);
    for (int i = 0; i < MAX_ENCODERS; i++) {
        encoders[i].setup();
    }
    xSemaphoreGive(i2c_lock);
    //encoder.setAccelerationEnabled(false);

    // Setup Time queue that triggers encoder scan service
    rotQueue = xQueueCreate(1, sizeof(int));
    if (rotQueue == NULL) {
        printf("ERROR !");
        return;
    }

    printf("Setup input timer!\n");

    timer_config_t config = {
            .alarm_en = true,                //Alarm Enable
            .counter_en = false,            //If the counter is enabled it will start incrementing / decrementing immediately after calling timer_init()
            .intr_type = TIMER_INTR_LEVEL,    //Is interrupt is triggered on timer’s alarm (timer_intr_mode_t)
            .counter_dir = TIMER_COUNT_UP,    //Does counter increment or decrement (timer_count_dir_t)
            .auto_reload = true,            //If counter should auto_reload a specific initial value on the timer’s alarm, or continue incrementing or decrementing.
            .divider = 80                //Divisor of the incoming 80 MHz (12.5nS) APB_CLK clock. E.g. 80 = 1uS per timer tick
    };
    timer_init(TIMER_GROUP_IO, &config);
    timer_set_counter_value(TIMER_GROUP_IO, 0);
    timer_set_alarm_value(TIMER_GROUP_IO, 1000);
    timer_group_intr_enable(TIMER_GROUP_IO);
    timer_isr_register(TIMER_GROUP_IO, rotaryEncInterrupt, NULL, 0, NULL);
    timer_set_alarm(TIMER_GROUP_IO, TIMER_ALARM_EN);
}

void startInputTask(void*unused)
{
    timer_start(TIMER_GROUP_IO);
    printf("Started input task&timer\n");

    for (;;)
    {
        int tick;
        // Wait for tick
        xQueueReceive(rotQueue, &(tick), portMAX_DELAY);

        xSemaphoreTake(i2c_lock, portMAX_DELAY);
        for (int i=0; i<MAX_ENCODERS; i++)
        {
            encoders[i].service();
        }
        xSemaphoreGive(i2c_lock);

        processInputs();
    }
}