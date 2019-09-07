/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 19/08/2019.
 */

#ifndef ARPEGGIATOR_DISPLAYTASK_H
#define ARPEGGIATOR_DISPLAYTASK_H



void setupDisplayController();
void displayDrawText(char text[], int posx = 0, int posy = 0);
void displayDrawTextLn(char text[]);
void displayUpdate();
void displayClear();

#endif //ARPEGGIATOR_DISPLAYTASK_H
