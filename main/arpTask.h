//
// Created by Orygin on 18/08/2019.
//

#ifndef ARPEGGIATOR_APRTASK_H
#define ARPEGGIATOR_APRTASK_H
#include "acommon.h"
#include "Arduino.h"

typedef enum {
    ARP_MOD_DELAY = 0
} arpModType;

struct arpModNotice {
    arpModType type;
    int value;
};

void setupTestArpeggiator(void*);
void arpSendModNotice(arpModNotice notice);

#endif //ARPEGGIATOR_APRTASK_H
