/******************************************************************
* File name: LED8x11_display_example3.ino
* Version: v1.1
* DEV: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display that shows
*   an animated ball bouncing around in a box.
******************************************************************/

/* Includes */
#include <LED8x11_display_lib.h>

/* Display brightness (0 to 1) */
#define BRIGHTNESS 0.6
/* Display frame rate */
#define FRAME_RATE 75

#define A5_D19 19
#define A4_D18 18
#define A3_D17 17
#define A2_D16 16
#define A1_D15 15
#define A0_D14 14
#define D13 13
#define D12 12
#define D11 11
#define D10 10
#define D9  9
#define D8  8
#define D7  7
#define D6  6
#define D5  5
#define D4  4
#define D3  3
#define D2  2

/* Create Display driver object */
LedDisplay my_display(
    D12, // RESET
    D11, // EOUT (Use a PWM compatable pin for brightness control)
    D10, // DCLK
    D9, // VSYNC
    D8, // HSYNC
    D7  // DATA
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

/* Game logic */
#define X_MAX LINE_LENGTH-1
#define X_MIN 0
#define Y_MAX LINE_COUNT-1
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

/* Function to move the ball */
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
void init_bouncy(){
    /* Set the random seed using a some (hopefully) unconnected pin */
    randomSeed(analogRead(0));

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
    TCCR2B |= 0b001;

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
