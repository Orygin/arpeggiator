//
// Created by Orygin on 18/08/2019.
//

// Synchronization: choose one of two possible options:
#define INT_SYNC
#include "arpEngine.h"


arp a(C, 2, 2, 4, 400, c_phrygian, 0);

void setupTestArpeggiator(void*null)
{

    a.midibegin(3, 17);

    a.setProgression(3);

    for (;;)
    {
        for (int i =0; i<8; i++){
            a.setProgression(i);
            a.play();
        }
    }
}