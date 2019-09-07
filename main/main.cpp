/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "inputTask.h"
#include "menuController.h"
#include "displayController.h"
#include "tempoTimer.h"
#include "Sequencor.h"

extern "C" {
void app_main();
}

void app_main() {
    printf("Hello world!\n");
    printf("Welcome to this Apreggiator first build!\n");


    printf("Setup inputs\n");
    setupInputs();
    printf("Setup display\n");
    setupDisplayController();

    printf("Start sequencer task\r");
    TaskHandle_t xHandleSequencer = NULL;
    xTaskCreatePinnedToCore(startSequencorTask, "SEQUENCE", 5000, (void*)1, 1, &xHandleSequencer, 0);

    printf("Setup tempo\n");
    setupTempoTimer(xHandleSequencer);

    TaskHandle_t xHandleInput = NULL;
    TaskHandle_t xHandleMenu = NULL;
    xTaskCreatePinnedToCore(taskMenuController, "MENU", 5000, ( void * ) 1, 1, &xHandleMenu, 1);
    xTaskCreatePinnedToCore(startInputTask, "INPUT", 5000, ( void * ) 1, 1, &xHandleInput, 1);
    //xTaskCreatePinnedToCore(setupTestArpeggiator, "Arp", 5000, (void*)1, 2, &xHandleArp, 0);
}
