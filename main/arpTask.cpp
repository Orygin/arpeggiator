//
// Created by Orygin on 18/08/2019.
//

// Synchronization: choose one of two possible options:
#define INT_SYNC

#include "tempoTimer.h"
#include "arpEngine.h"
#include "arpTask.h"
#include "Arduino.h"

arp a(C, 2, 2, 4, 400, c_phrygian, 0);

QueueHandle_t arpModQueue;

void handleArpMod() {
    arpModNotice notice;
    while (xQueueReceive(arpModQueue, &notice, 0))
    {
        switch (notice.type) {
            case ARP_MOD_DELAY:
                a.setDelay(a.getDelay() + notice.value);
                printf("%d %d", notice.value, a.getDelay());
                break;
        }
    }
}
void arpSendModNotice(arpModNotice notice) {
    xQueueSend(arpModQueue, (void*) &notice, 0);
}

void setupTestArpeggiator(void*null)
{
    arpModQueue = xQueueCreate(10, sizeof(arpModNotice));

    a.midibegin(3, 17);

    a.setProgression(3);

    for (;;)
    {
            ulTaskNotifyTake(true, portMAX_DELAY);

            for (int i =0; i<8; i++){
            a.setProgression(i);
            a.play();
            vTaskDelay(100 / portTICK_PERIOD_MS);
            //handleArpMod();
        }
    }
}
