/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 18/08/2019.
 */

#ifndef ARPEGGIATOR_MENUCONTROLLER_H
#define ARPEGGIATOR_MENUCONTROLLER_H
#include "acommon.h"
#include "Arduino.h"

typedef enum {
    INPUT_ENCODER_TURN = 0,
    INPUT_ENCODER_BUTTON = 1,
    INPUT_NOTE_BUTTON = 2,
    INPUT_BUTTON = 3,
} inputNoticeType;

struct inputUpdateNotice {
    inputNoticeType type;
    uint8_t inputID;
    int value;
};

void taskMenuController(void*);
void menuSendToInputQueue(inputUpdateNotice notice);

#endif //ARPEGGIATOR_MENUCONTROLLER_H
