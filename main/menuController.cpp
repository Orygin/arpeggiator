/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 18/08/2019.
 */
#include <stdio.h>
#include "menuController.h"
#include "displayController.h"
#include "Sequencor.h"

#define MENU_REFRESHMS 50

void menuUpdateSequenceDisplay();

QueueHandle_t menuInputQueue;
char menuTxtBuffer[64];
void taskMenuController(void*unused)
{
    // Create queue
    menuInputQueue = xQueueCreate(8, sizeof(inputUpdateNotice));

    inputUpdateNotice incomingNotice;
    for (;;)
    {
        displayClear();
        menuUpdateSequenceDisplay();

        if(xQueueReceive(menuInputQueue, &incomingNotice, MENU_REFRESHMS / portTICK_PERIOD_MS))
        {
            printf("%d %d %d\n", incomingNotice.inputID, incomingNotice.value, incomingNotice.type);
            sprintf(menuTxtBuffer, "%d %d %d\n", incomingNotice.inputID, incomingNotice.value, incomingNotice.type);
            displayDrawTextLn(menuTxtBuffer);
        }

        displayUpdate();
    }
}

void menuSendToInputQueue(inputUpdateNotice notice)
{
    xQueueSend(menuInputQueue, (void*) &notice, portMAX_DELAY);
}

int currentSequence = 0;
void menuUpdateSequenceDisplay() {
    Sequencer *sq = Sequencor[currentSequence];
    if (sq == NULL) {
        return;
    }
    int note = sq->getCurrentNoteIdx();

    sprintf(menuTxtBuffer, "note: %d", note);

    displayDrawText(menuTxtBuffer);
}