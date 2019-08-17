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
#include "rotaryEnc.h"
#include "arpTask.h"


extern "C" {
void app_main();
}

void app_main() {
    printf("Hello world!\n");
    printf("Welcome to this Apreggiator first build!\n");

    TaskHandle_t xHandleRotary = NULL;
    TaskHandle_t xHandleArp = NULL;
    //xTaskCreatePinnedToCore(setupTestRotaryEnc, "RotaryEnc", 5000, ( void * ) 1, 1, &xHandleRotary, 1);
    xTaskCreatePinnedToCore(setupTestArpeggiator, "Arp", 5000, (void*)1, 2, &xHandleArp, 0);
    //setupTestRotaryEnc();
    //initArduino();
}
