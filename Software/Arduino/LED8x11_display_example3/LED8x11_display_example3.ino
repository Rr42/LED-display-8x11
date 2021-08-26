/******************************************************************
* File name: LED8x11_display_example3.ino
* Version: v1.0
* DEV: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display that shows
*   an animated ball bouncing around in a box.
******************************************************************/

/* Includes */
#include <LED8x11_display_lib.h>

/* Alternative display method */
#define ALTERNATIVE_DISPLAY

/* Display brightness (0 to 1) [works with alternative init method] */
#define BRIGHTNESS 0.7
/* Display frame rate [works with alternative init method] */
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
                                     0b10000000001,
                                     0b10000000001,
                                     0b10000000001,
                                     0b10000100001,
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

/* Game logic */
#define X_MAX 10
#define X_MIN 0
#define Y_MAX 7
#define Y_MIN 0
/* Directions:
* 1 2 3 | -1 -1 -1 | -1 00 +1
* 4   5 | 00 dy 00 | -1 dx +1
* 6 7 8 | +1 +1 +1 | -1 00 +1
*/
struct positionVector
{
    /* Init with default starting positions */
    positionVector() : x_pos(5), y_pos(4), x_dir(+1), y_dir(-1) {}
    uint8_t x_pos;
    uint8_t y_pos;
    int8_t x_dir;
    int8_t y_dir;
}ball;

void move_ball(bool reflect=false) {
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
        // ball.x_pos += ball.x_dir;
        move_again = true;
    }
    if (ball.y_pos == Y_MAX || ball.y_pos == Y_MIN) {
        ball.y_dir *= -1;
        // ball.y_pos += ball.y_dir;
        move_again = true;
    }

    /* If an edge was hit move the ball */
    if (move_again)
        move_ball(true);

    /* Set new position in buffer */
    buffer[ball.y_pos] |= (0x1<<ball.x_pos);
}

void init_bouncy(){
    /* Set the random seed using a some (hopefully) unconnected pin */
    randomSeed(analogRead(0));

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

void setup() {
    /* Initialise with custom brightness and frame rate */
    my_display.initialise(BRIGHTNESS, FRAME_RATE);

    /* Initialise bouncy ball */
    init_bouncy();

    /* Load starting frame into buffer */
    for (uint8_t i=0; i<LINE_COUNT; ++i)
        buffer[i] = start[i];
}

void loop() {
    my_display.showFrame(buffer, BUFFER_REFRESH_DELAY);
    move_ball();
}
