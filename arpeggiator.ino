#include "libraries/encoder/ClickEncoder.h"
#include "engine.h"
arp a(C, 5, 2, 6, 200, c_harmonic, 0);
bool button_pressed;
int ButtonVal;

#define baseNotepin 1
#define baseOctavepin 5
#define octaveShiftpin 0
#define stepspin 4
#define indelaypin 3
#define orderpin 7
#define modepin 6
#define syncinpin 3

#define LEDPin 13

// Synchronization: choose one of two possible options:
#define EXT_SYNC
//#define INT_SYNC

void setup()
{
  a.midibegin();
  // LED pin
  pinMode(LEDPin, OUTPUT);
  
  // Initialize pins for 2-pins pushbuttons with pullup enabled
  for (unsigned i=6;i<13;i++)
  {
    pinMode(i,INPUT_PULLUP);
    //pinMode(i, INPUT);
    //digitalWrite(i, 1);
  }
  button_pressed = false;
  ButtonVal = 1;
}

void loop()
{
    if (button_pressed)
    {
      a.setProgression(ButtonVal-1);
      button_pressed = false;
      
      // Switch on LED
      digitalWrite(LEDPin, HIGH);
      a.play();
      
      // Switch off LED
      digitalWrite(LEDPin, LOW);
    }
}
