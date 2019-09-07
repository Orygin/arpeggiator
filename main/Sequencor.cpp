/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 07/09/2019.
 */
#include "Arduino.h"
#include "TempoTimer.h"
#include "Sequencor.h"
#include "stdlib.h"
#include "FreeRTOS.h"


// MIDI
HardwareSerial MySerial(2);
MIDI_CREATE_INSTANCE(HardwareSerial, MySerial, MIDI);
// Sequencor
Sequencer* Sequencor[SEQUENCOR_LENGTH];

short midiByNote (notes note, short octave)
{
    if ((octave < -1)||(octave > 8))
        return -1;

    return (octave+1)*12 + (int)note;
}

Sequencer::Sequencer(int midi, int len) {
    notes =  (Note*)malloc(len * sizeof(Note));
    for (int i =0; i < len; i++) {
        notes[i] = {midiByNote(CD, i%4) , 127 ,0};
    }
    length = len;
    midiChannel = midi;
    currentNote = 0;
    currentBeat = 0;
}
Sequencer::~Sequencer() {
    free(notes);
}

void Sequencer::Step() {
    if (currentBeat == 0){
        MIDI.sendNoteOn(notes[currentNote].note, notes[currentNote].velocity, midiChannel);
        //printf("Midinote: %d\n", notes[currentNote].note);
    }

    if (currentBeat == (BPM_PRECISION/2)) {
        MIDI.sendNoteOff(notes[currentNote].note, notes[currentNote].velocity, midiChannel);
        currentNote++;
    }

    currentBeat++;
    if (currentBeat == BPM_PRECISION)
        currentBeat = 0;
    if (currentNote == length)
        currentNote = 0;
}

void Sequencer::Stop() {
    MIDI.sendNoteOff(notes[currentNote].note, notes[currentNote].velocity, midiChannel);
    currentNote = 0;
    currentBeat = 0;
}

void setupSequencor()
{
    MIDI.begin(4, MIDIRXPIN, MIDITXPIN);

    for (int i = 0; i < SEQUENCOR_LENGTH; i++) {
        Sequencor[i] = new Sequencer(i+1);
    }
}

void startSequencorTask(void*unused)
{
    setupSequencor();

    for (;;)
    {
        ulTaskNotifyTake(true, portMAX_DELAY);

        for (int i = 0; i < SEQUENCOR_LENGTH; i++) {
            if (Sequencor[i] != NULL)
            {
                Sequencor[i]->Step();
            }
        }
    }
}