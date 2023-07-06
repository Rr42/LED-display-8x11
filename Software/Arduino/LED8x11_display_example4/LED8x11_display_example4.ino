/******************************************************************
* File name: LED8x11_display_example4.ino
* Version: v1.1
* DEV: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display that shows
*   an animated ball bouncing around in a box.
******************************************************************/

/* Includes */
#include <LED8x11_display_lib.h>

/* Display brightness (0 to 1) */
#define BRIGHTNESS 1
/* Display frame rate */
#define FRAME_RATE 75

/* Create Display driver object */
LedDisplay my_display(
    53, // RESET
    03, // EOUT (Use a PWM compatable pin for brightness control)
    51, // DCLK
    50, // VSYNC
    49, // HSYNC
    48  // DATA
);

/* Frames for playing bouncy */
/* Starting frame */
const uint16_t start[LINE_COUNT] = { 0b11111111111,
                                     0b11111111111,
                                     0b11111111111,
                                     0b11111111111,
                                     0b11111111111,
                                     0b11111111111,
                                     0b11111111111,
                                     0b11111111111};

/* Frame buffer */
uint16_t buffer[LINE_COUNT] = { 0b00000000000,  // 0
                                0b00000000000,  // 1
                                0b00000000000,  // 2
                                0b00000000000,  // 3
                                0b00000000000,  // 4
                                0b00000000000,  // 5
                                0b00000000000,  // 6
                                0b00000000000}; // 7
//                                0123456789X

void setup() {
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
    TCCR3B |= 0b001;
̥
    /* Initialise with custom brightness and frame rate */
    my_display.initialise(BRIGHTNESS, FRAME_RATE);

    /* Initialise bouncy ball */
    // init_bouncy();

    /* Load starting frame into buffer */
    for (uint8_t i=0; i<LINE_COUNT; ++i)
        buffer[i] = start[i];
}

void loop() {
    my_display.showFrame(buffer, 1);
    // move_ball();
}
