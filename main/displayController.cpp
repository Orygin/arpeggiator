/** 
 * Arpeggo project file
 * License GPLv3
 * Created by Orygin on 19/08/2019.
 */

#include "displayController.h"
#include "Adafruit_SSD1306.h"

/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers
 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98
 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!
 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>

#include "inputTask.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
B00000001, B11000000,
B00000001, B11000000,
B00000011, B11100000,
B11110011, B11100000,
B11111110, B11111000,
B01111110, B11111111,
B00110011, B10011111,
B00011111, B11111100,
B00001101, B01110000,
B00011011, B10100000,
B00111111, B11100000,
B00111111, B11110000,
B01111100, B11110000,
B01110000, B01110000,
B00000000, B00110000 };


void setupDisplayController() {
    xSemaphoreTake(i2c_lock, portMAX_DELAY);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        printf("SSD1306 allocation failed\n");
        return;
    }
    xSemaphoreGive(i2c_lock);

    displayDrawText("Starting Arpeggiator...");

    return;
}

void displayDrawText(char text[], int posx, int posy)
{
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(posx, posy);
    display.println(text);
}
void displayDrawTextLn(char text[])
{
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.println(text);
}
void displayUpdate() {
    xSemaphoreTake(i2c_lock, portMAX_DELAY);
    display.display();
    xSemaphoreGive(i2c_lock);
}
void displayClear() {
    display.clearDisplay();
}

void testdrawline() {
    int16_t i;

    display.clearDisplay(); // Clear display buffer

    for(i=0; i<display.width(); i+=4) {
        display.drawLine(0, 0, i, display.height()-1, WHITE);
        display.display(); // Update screen with each newly-drawn line
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    for(i=0; i<display.height(); i+=4) {
        display.drawLine(0, 0, display.width()-1, i, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    vTaskDelay(250 / portTICK_PERIOD_MS);

    display.clearDisplay();

    for(i=0; i<display.width(); i+=4) {
        display.drawLine(0, display.height()-1, i, 0, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    for(i=display.height()-1; i>=0; i-=4) {
        display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    vTaskDelay(250 / portTICK_PERIOD_MS);

    display.clearDisplay();

    for(i=display.width()-1; i>=0; i-=4) {
        display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    for(i=display.height()-1; i>=0; i-=4) {
        display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    vTaskDelay(250 / portTICK_PERIOD_MS);

    display.clearDisplay();

    for(i=0; i<display.height(); i+=4) {
        display.drawLine(display.width()-1, 0, 0, i, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    for(i=0; i<display.width(); i+=4) {
        display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS); // Pause for 2 seconds
}

void testdrawrect(void) {
    display.clearDisplay();

    for(int16_t i=0; i<display.height()/2; i+=2) {
        display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
        display.display(); // Update screen with each newly-drawn rectangle
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testfillrect(void) {
    display.clearDisplay();

    for(int16_t i=0; i<display.height()/2; i+=3) {
        // The INVERSE color is used so rectangles alternate white/black
        display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
        display.display(); // Update screen with each newly-drawn rectangle
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testdrawcircle(void) {
    display.clearDisplay();

    for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
        display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testfillcircle(void) {
    display.clearDisplay();

    for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
        // The INVERSE color is used so circles alternate white/black
        display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
        display.display(); // Update screen with each newly-drawn circle
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testdrawroundrect(void) {
    display.clearDisplay();

    for(int16_t i=0; i<display.height()/2-2; i+=2) {
        display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
                              display.height()/4, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testfillroundrect(void) {
    display.clearDisplay();

    for(int16_t i=0; i<display.height()/2-2; i+=2) {
        // The INVERSE color is used so round-rects alternate white/black
        display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
                              display.height()/4, INVERSE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testdrawtriangle(void) {
    display.clearDisplay();

    for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
        display.drawTriangle(
                display.width()/2  , display.height()/2-i,
                display.width()/2-i, display.height()/2+i,
                display.width()/2+i, display.height()/2+i, WHITE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testfilltriangle(void) {
    display.clearDisplay();

    for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
        // The INVERSE color is used so triangles alternate white/black
        display.fillTriangle(
                display.width()/2  , display.height()/2-i,
                display.width()/2-i, display.height()/2+i,
                display.width()/2+i, display.height()/2+i, INVERSE);
        display.display();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testdrawchar(void) {
    display.clearDisplay();

    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    // Not all the characters will fit on the display. This is normal.
    // Library will draw what it can and the rest will be clipped.
    for(int16_t i=0; i<256; i++) {
        if(i == '\n') display.write(' ');
        else          display.write(i);
    }

    display.display();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testdrawstyles(void) {
    display.clearDisplay();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Hello, world!"));

    display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
    display.println(3.141592);

    display.setTextSize(2);             // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.print(F("0x")); display.println(0xDEADBEEF, HEX);

    display.display();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void testscrolltext(void) {
    display.clearDisplay();

    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.setCursor(10, 0);
    display.println(F("scroll"));
    display.display();      // Show initial text
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Scroll in various directions, pausing in-between:
    display.startscrollright(0x00, 0x0F);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    display.stopscroll();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    display.startscrollleft(0x00, 0x0F);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    display.stopscroll();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    display.startscrolldiagright(0x00, 0x07);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    display.startscrolldiagleft(0x00, 0x07);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    display.stopscroll();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void testdrawbitmap(void) {
    display.clearDisplay();

    display.drawBitmap(
            (display.width()  - LOGO_WIDTH ) / 2,
            (display.height() - LOGO_HEIGHT) / 2,
            logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void uu(){
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();

    // Draw a single pixel in white
    display.drawPixel(10, 10, WHITE);

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // display.display() is NOT necessary after every single drawing command,
    // unless that's what you want...rather, you can batch up a bunch of
    // drawing operations and then update the screen all at once by calling
    // display.display(). These examples demonstrate both approaches...

    testdrawline();      // Draw many lines

    testdrawrect();      // Draw rectangles (outlines)

    testfillrect();      // Draw rectangles (filled)

    testdrawcircle();    // Draw circles (outlines)

    testfillcircle();    // Draw circles (filled)

    testdrawroundrect(); // Draw rounded rectangles (outlines)

    testfillroundrect(); // Draw rounded rectangles (filled)

    testdrawtriangle();  // Draw triangles (outlines)

    testfilltriangle();  // Draw triangles (filled)

    testdrawchar();      // Draw characters of the default font

    testdrawstyles();    // Draw 'stylized' characters

    testscrolltext();    // Draw scrolling text

    testdrawbitmap();    // Draw a small bitmap image

    // Invert and restore display, pausing in-between
    display.invertDisplay(true);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    display.invertDisplay(false);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
