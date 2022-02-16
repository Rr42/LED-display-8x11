/******************************************************************
* File name: LED8x11_display_example1.hpp
* Version: v1.0
* Dev: GitHub@Rr42
* Description:
*  Header for example Arduino program for the 8x11 LED display
*   that shows an animated ball bouncing around in a box.
******************************************************************/
#ifndef __LED8X11_DISPLAY_EXAMPLE1__

#define __LED8X11_DISPLAY_EXAMPLE1__
/* Includes */
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "LED8x11_display_lib.hpp"

/* Display brightness (0 to 1) */
#define BRIGHTNESS 0.7
/* Display frame rate */
#define FRAME_RATE 75

/* Definitions and datatupes for Animation logic */
#define X_MAX LINE_LENGTH-1
#define X_MIN 0
#define Y_MAX LINE_COUNT-1
#define Y_MIN 0
/* Directions:
* 1 2 3 | -1 -1 -1 | -1 00 +1
* 4   5 | 00 dy 00 | -1 dx +1
* 6 7 8 | +1 +1 +1 | -1 00 +1
*/
struct positionVector {
    /* Init with default starting positions */
    positionVector() : x_pos(5), y_pos(4), x_dir(+1), y_dir(-1) {}
    uint8_t x_pos;
    uint8_t y_pos;
    int8_t x_dir;
    int8_t y_dir;
};

/* Header for messages */
#define MSG_HEADER "[LED8x11_display_example1] "

/* Inline functions */
extern inline int random(int min, int max);
extern inline void message(const char* msg);
extern inline void cleanup(void);

/* Animation logic functions */
extern void move_ball(bool reflect=false);
extern void init_bouncy(void);

/* Animation core functions */
extern void setup(void);
extern void loop(void);

/* Interrupt signal handler */
extern void signal_callback_handler(int signal);

/* Program entry point */
int main(void);

#endif
