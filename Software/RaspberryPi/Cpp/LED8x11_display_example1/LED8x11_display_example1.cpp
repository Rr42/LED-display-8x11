/******************************************************************
* File name: LED8x11_display_example1.cpp
* Version: v1.0
* Dev: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display that shows
*   an animated ball bouncing around in a box.
******************************************************************/

/* Includes */
#include "LED8x11_display_example1.hpp"

/* Function to generate random numbers within given range */
inline int random(int min, int max) {
    return rand() % max + min;
}
/* Function to print messages to the user */
inline void message(const char* msg) {
    std::cout << MSG_HEADER << msg << std::endl;
}

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
                                     0b10000000001,
                                     0b10000000001,
                                     0b10000000001,
                                     0b10000000001,
                                     0b10000000001,
                                     0b10000000001,
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

/* Animation logic */
/* Directions:
* 1 2 3 | -1 -1 -1 | -1 00 +1
* 4   5 | 00 dy 00 | -1 dx +1
* 6 7 8 | +1 +1 +1 | -1 00 +1
*/
struct positionVector ball;

/* Function to move the ball */
void move_ball(bool reflect) {
    /* Remove previous position in buffer if ball is not being reflected */
    if (!reflect)
        buffer[ball.y_pos] &= ( ( ~(0x1<<ball.x_pos) )|0b10000000001 );

    /* Move */
    ball.x_pos += ball.x_dir;
    ball.y_pos += ball.y_dir;

    /* Reflect */
    bool move_again = false;
    if (ball.x_pos == X_MAX || ball.x_pos == X_MIN) {
        ball.x_dir *= -1;
        move_again = true;
    }
    if (ball.y_pos == Y_MAX || ball.y_pos == Y_MIN) {
        ball.y_dir *= -1;
        move_again = true;
    }

    /* If an edge was hit move the ball */
    if (move_again)
        move_ball(true);

    /* Set new position in buffer */
    buffer[ball.y_pos] |= (0x1<<ball.x_pos);
}

/* Function to initilise the bouncy ball */
void init_bouncy() {
    /* Set the random seed current time */
    srand(time(0));

    /* Set a random starting position for the ball */
    ball.x_pos = random(X_MIN+2, X_MAX-2);
    ball.y_pos = random(Y_MIN+2, Y_MAX-2);

    /* Set a random starting direction for the ball */
    switch(random(1, 9)) {
    case 1:
        ball.x_dir=-1;
        ball.y_dir=-1;
        break;
    case 2:
        ball.x_dir=0;
        ball.y_dir=-1;
        break;
    case 3:
        ball.x_dir=+1;
        ball.y_dir=-1;
        break;
    case 4:
        ball.x_dir=-1;
        ball.y_dir=0;
        break;
    case 5:
        ball.x_dir=+1;
        ball.y_dir=0;
        break;
    case 6:
        ball.x_dir=-1;
        ball.y_dir=+1;
        break;
    case 7:
        ball.x_dir=0;
        ball.y_dir=+1;
        break;
    case 8:
        ball.x_dir=+1;
        ball.y_dir=+1;
        break;
    }
}

/* Duration of time to wait before switching to next updated buffer */
#define BUFFER_REFRESH_DELAY 0.15

/* Function to do first time setup */
void setup() {
    /* Initialise with custom brightness and frame rate */
    my_display.initialise(BRIGHTNESS, FRAME_RATE);

    /* Initialise bouncy ball */
    init_bouncy();

    /* Load starting frame into buffer */
    for (uint8_t i=0; i<LINE_COUNT; ++i)
        buffer[i] = start[i];
}

/* Function to implement main logic loop */
void loop() {
    while (true) {
        my_display.showFrame(buffer, BUFFER_REFRESH_DELAY);
        move_ball();
    }
}

/* Function to cleanup  exit */
void cleanup() {
   message("Cleaning up...");
   /* Cleanup code */
   message("Cleanup done");
}

/* Interrupt signal handler */
void signal_callback_handler(int signum) {
    message("Interrupt received!");
    cleanup();
    /* Terminate program */
    message("Exiting...");
    exit(signum);
}

/* Program entry point */
int main() {
    /* Register signal and signal handler */
    signal(SIGINT, signal_callback_handler);

    message("Starting animation...");

    /* Run first time setup */
    // setup();

    /* The endless loop */
    // loop();
    message("[TEST] call to loop()");
    // while (true)
    //     std::cout << "loop" << std::endl;

    /* Cleanup */
    cleanup();
    return EXIT_SUCCESS;
}