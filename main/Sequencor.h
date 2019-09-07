/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 07/09/2019.
 */
#include "MIDI.h"
#include "FreeRTOS.h"
#ifndef ARPEGGIATOR_SEQUENCERTASK_H
#define ARPEGGIATOR_SEQUENCERTASK_H

#define SEQUENCOR_LENGTH 4
#define MIDIRXPIN 3
#define MIDITXPIN 17

typedef enum {C, CD, D, DD, E, F, FD, G, GD, A, AD, B} notes;

typedef struct {
    short note;
    int velocity;
    int length;
} Note;

class Sequencer {
public:
    ~Sequencer();
    Sequencer(int midi, int len = 64);

    void Step();
    void Stop();

    Note* getCurrentNote() {
        return &notes[currentNote];
    }
    int getCurrentNoteIdx() {
        return currentNote;
    }

private:
    // READ ONLY FROM STEP
    Note *notes;
    bool running;
    int midiChannel;
    // WRITE ONLY FROM STEP
    int currentNote;
    int currentBeat;
    int length;
};

extern Sequencer* Sequencor[SEQUENCOR_LENGTH];

void startSequencorTask(void*);

#endif //ARPEGGIATOR_SEQUENCERTASK_H
