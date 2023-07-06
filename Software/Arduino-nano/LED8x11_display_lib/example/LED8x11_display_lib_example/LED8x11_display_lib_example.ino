/******************************************************************
* File name: LED8x11_display_driver_lib.ino
* Version: v1.1
* DEV: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display to
*   adjust brightness. The end result can be jarring as
*   the PWM frequency is not high enough to mask the switching.
*   SEIZURE WARNING!
******************************************************************/

/* Includes */
#include <LED8x11_display_lib.h>

/* Alternative display method */
#define ALTERNATIVE_DISPLAY

/* Alternative init method */
#define ALTERNATIVE_INIT

/* Display brightness (0 to 1) [works with alternative init method] */
#define BRIGHTNESS 0.5
/* Display frame rate [works with alternative init method] */
#define FRAME_RATE 75

/* Test frame 1 */
const uint8_t display_data_1[LINE_COUNT][LINE_LENGTH] = { 
                                                {1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0},  // 0
                                                {1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},  // 1
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 2
                                                {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},  // 3
                                                {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},  // 4
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 5
                                                {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},  // 6
                                                {1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0}}; // 7
//                                               0  1  2  3  4  5  6  7  8  9 10

/* Test frame 2 as a bit array */
const uint16_t display_data_2[LINE_COUNT] = { 0b10101010101,  // 0
                                              0b10101010101,  // 1
                                              0b01010101010,  // 2
                                              0b10101010101,  // 3
                                              0b10101010101,  // 4
                                              0b01010101010,  // 5
                                              0b10101010101,  // 6
                                              0b10101010101}; // 7

/* Create Display driver object */
LedDisplay my_display(
    53, // RESET
    03, // EOUT (Use a PWM compatable pin for brightness control)
    51, // DCLK
    50, // VSYNC
    49, // HSYNC
    48  // DATA
);

/* Duration of time the frame is shown on display */
#define DISPLAY_TIME 1
bool switch_frame;

void setup() {
#ifndef ALTERNATIVE_INIT
    /* Initialise with defaults */
    my_display.initialise();
#else
    /* Set PWM frequency */
    /* Atmega 2560 timers:
        timer 0 -> controls pin 13, 4
        timer 1 -> controls pin 12, 11
        timer 2 -> controls pin 10, 9
        timer 3 -> controls pin 5, 3, 2 (Used in this example)
        timer 4 -> controls pin 8, 7, 6 */
    /* Prescaler values:
        prescaler = 1 —> PWM frequency is 31000 Hz (Used in this example)
        prescaler = 2 —> PWM frequency is 4000 Hz
        prescaler = 3 —> PWM frequency is 490 Hz (default value)
        prescaler = 4 —> PWM frequency is 120 Hz
        prescaler = 5 —> PWM frequency is 30 Hz
        prescaler = 6 —> PWM frequency is <20 Hz */
    // Erase
    TCCR2B &= ~0b111;
    // Set
    TCCR2B |= 0b001;

    /* Initialise with custom brightness and frame rate */
    my_display.initialise(BRIGHTNESS, FRAME_RATE);
#endif

    switch_frame = false;
}

void loop() {
#ifndef ALTERNATIVE_DISPLAY
    /* Loop through each line in test frame */
    for (uint8_t i=0; i<LINE_COUNT; ++i) {
        /* Send ith line to display */
        my_display.sendLine(display_data_1[i]);
        /* Show line */
        my_display.displayRefresh();
        /* Wait to match referesh rate */
        delayMicroseconds(my_display.getLineDelay());
    }
    /* Optional display reset between each frame (will slightly effect display framerate) */
    my_display.displayReset();
#else
    /* Alternative simpler method to display frames */
    if (switch_frame)
        my_display.showFrame(display_data_1, DISPLAY_TIME);
    else
        my_display.showFrame(display_data_2, DISPLAY_TIME);
    switch_frame = !switch_frame;
#endif
}
