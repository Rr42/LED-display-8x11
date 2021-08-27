/******************************************************************
* File name: LED8x11_pong.ino
* Version: v1.0
* DEV: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display that
*   allows 1 ot 2 player(s) to play pong on the display.
******************************************************************/

/* Includes */
#include <LED8x11_display_lib.h>

/* Single player model for pong */
#define SINGLE_PLAYER_MODE

/* Display brightness (0 to 1) [works with alternative init method] */
#define BRIGHTNESS 0.5
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

/* Buttons for player paddles */
#define P1_UP 12
#define P1_UP_C 11

#define P1_DN 10
#define P1_DN_C 9

#define P2_UP 22
#define P2_UP_C 23

#define P2_DN 31
#define P2_DN_C 35

/* Frames for playing pong */
/* Starting frame:
*       x ↦                                        
*       0   1   2   3   4   5   6   7   8   9   10
*     +-------------------------------------------
* y 0 | 0   1   1   1   1   1   1   1   1   1   0 
* ↧ 1 | 0   0   0   0   0   0   0   0   0   0   0 
*   2 | 0   0   0   0   0   0   0   0   0   0   0 
*   3 | 1   0   0   0   0   0   0   0   0   0   1 
*   4 | 1   0   0   0   0   1   0   0   0   0   1 
*   5 | 0   0   0   0   0   0   0   0   0   0   0 
*   6 | 0   0   0   0   0   0   0   0   0   0   0 
*   7 | 0   1   1   1   1   1   1   1   1   1   0 
*/
#ifndef SINGLE_PLAYER_MODE
const uint16_t start[LINE_COUNT] = { 0b01111111110,
                                     0b00000000000,
                                     0b00000000000,
                                     0b10000000001,
                                     0b10000000001,
                                     0b00000000000,
                                     0b00000000000,
                                     0b01111111110};
#else
const uint16_t start[LINE_COUNT] = { 0b11111111110,
                                     0b10000000000,
                                     0b10000000000,
                                     0b10000000001,
                                     0b10000000001,
                                     0b10000000000,
                                     0b10000000000,
                                     0b11111111110};
#endif

#ifndef SINGLE_PLAYER_MODE
/* Player 1/2 win frame */
const uint16_t win_p[2][LINE_COUNT] = { {0b00000000000,
                                         0b00000000000,
                                         0b01111011000,
                                         0b01001001000,
                                         0b01111001000,
                                         0b01000001000,
                                         0b01000011100,
                                         0b00000000000},
                                        {0b00000000000,
                                         0b00000000000,
                                         0b01111011110,
                                         0b01001000010,
                                         0b01111011110,
                                         0b01000010000,
                                         0b01000011110,
                                         0b00000000000}};
#else
const uint16_t win_c[LINE_COUNT] = {0b11111111111,
                                    0b10000000001,
                                    0b10010001001,
                                    0b10000000001,
                                    0b10011111001,
                                    0b10100000101,
                                    0b10000000001,
                                    0b11111111111};
#endif

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
* 1   2 | -1 -1 -1 | -1 00 +1
*       | 00 dy 00 | -1 dx +1
* 3   4 | +1 +1 +1 | -1 00 +1
*/
#define PLAYER1_ID 1
#define PLAYER2_ID 2
bool player_won;
uint8_t player_won_id;

struct positionVector
{
    /* Init with default starting positions */
    positionVector() : x_pos(5), y_pos(4), x_dir(+1), y_dir(-1) {}
    uint8_t x_pos;
    uint8_t y_pos;
    int8_t x_dir;
    int8_t y_dir;
}ball, paddle[2];

/* Function to model paddle position */
void move_paddle() {
    /* Remove previous paddle positions */
    buffer[paddle[0].y_pos] &= 0b11111111110;
    buffer[paddle[0].y_pos+1] &= 0b11111111110;
#ifndef SINGLE_PLAYER_MODE
    buffer[paddle[1].y_pos] &= 0b01111111111;
    buffer[paddle[1].y_pos+1] &= 0b01111111111;
#endif

    /* Player 1 paddle movement */
    if (digitalRead(P1_UP) == LOW) {
        /* Move up player 1 paddle */
        if (paddle[0].y_pos != Y_MIN)
            paddle[0].y_pos -= 1;
    } else if (digitalRead(P1_DN) == LOW) {
        /* Move down player 1 paddle */
        if (paddle[0].y_pos != Y_MAX-1)
            paddle[0].y_pos += 1;
    }

#ifndef SINGLE_PLAYER_MODE
    /* Player 2 paddle movement */
    if (digitalRead(P2_UP) == LOW) {
        /* Move up player 2 paddle */
        if (paddle[1].y_pos != Y_MIN)
            paddle[1].y_pos -= 1;
    } else if (digitalRead(P2_DN) == LOW) {
        /* Move down player 2 paddle */
        if (paddle[1].y_pos != Y_MAX-1)
            paddle[1].y_pos += 1;
    }
#endif

    /* Update new paddle positions */
    buffer[paddle[0].y_pos] |= 0b00000000001;
    buffer[paddle[0].y_pos+1] |= 0b00000000001;
#ifndef SINGLE_PLAYER_MODE
    buffer[paddle[1].y_pos] |= 0b10000000000;
    buffer[paddle[1].y_pos+1] |= 0b10000000000;
#endif
}

/* Function to move the ball or return winning player number */
uint8_t move_ball(bool reflect=false) {
    /* Remove previous position in buffer if ball is not being reflected */
    if (!reflect)
        buffer[ball.y_pos] &= ( ( ~(0x1<<ball.x_pos) )|0b10000000001 );

    /* Move */
    ball.x_pos += ball.x_dir;
    ball.y_pos += ball.y_dir;

    /* Reflect */
    bool move_again = false;
    /* Player sides flipped by display algo! */
    if (ball.x_pos == X_MAX) {
#ifndef SINGLE_PLAYER_MODE

        /* Check if ball hits player 2 paddle */
        if (ball.y_pos == paddle[1].y_pos || ball.y_pos == paddle[1].y_pos+1) {
            ball.x_dir *= -1;
            move_again = true;
        } else
            return PLAYER1_ID;
#else
        ball.x_dir *= -1;
        move_again = true;
#endif
    } else if (ball.x_pos == X_MIN) {
        /* Check if ball hits player 1 paddle */
        if (ball.y_pos == paddle[0].y_pos || ball.y_pos == paddle[0].y_pos+1) {
            ball.x_dir *= -1;
            move_again = true;
        } else
            return PLAYER2_ID;
    }

    if (ball.y_pos == Y_MAX || ball.y_pos == Y_MIN) {
        ball.y_dir *= -1;
        move_again = true;
    }

    /* If an edge/paddle was hit move the ball */
    if (move_again)
        return move_ball(true);

    /* Set new position in buffer */
    buffer[ball.y_pos] |= (0x1<<ball.x_pos);
    return 0;
}

/* Function to initilise pong */
void init_pong(){
    /* Setup player paddles */
    pinMode(P1_UP, INPUT);
    pinMode(P1_UP_C, OUTPUT);
    /* Set hot side of player 1 up button */
    digitalWrite(P1_UP_C, LOW);
    /* Pull down cool side of player 1 up button */
    digitalWrite(P1_UP, HIGH);

    pinMode(P1_DN, INPUT);
    pinMode(P1_DN_C, OUTPUT);
    /* Set hot side of player 1 down button */
    digitalWrite(P1_DN_C, LOW);
    /* Pull down cool side of player 1 down button */
    digitalWrite(P1_DN, HIGH);

#ifndef SINGLE_PLAYER_MODE
    pinMode(P2_UP, INPUT);
    pinMode(P2_UP_C, OUTPUT);
    /* Set hot side of player 2 up button */
    digitalWrite(P2_UP_C, LOW);
    /* Pull down cool side of player 2 up button */
    digitalWrite(P2_UP, HIGH);

    pinMode(P2_DN, INPUT);
    pinMode(P2_DN_C, OUTPUT);
    /* Set hot side of player 2 down button */
    digitalWrite(P2_DN_C, LOW);
    /* Pull down cool side of player 2 down button */
    digitalWrite(P2_DN, HIGH);
#endif

    /* Set the random seed using a some (hopefully) unconnected pin */
    randomSeed(analogRead(0));

    /* Set a random starting position for the ball */
    // ball.x_pos = random(X_MIN+2, X_MAX-2);
    // ball.y_pos = random(Y_MIN+2, Y_MAX-2);

    /* Setup playter 1 paddle */
    paddle[0].x_dir = 0;
    paddle[0].y_dir = 0;
    paddle[0].x_pos = X_MAX;
    paddle[0].y_pos = 3;

    /* Setup playter 2 paddle */
    paddle[1].x_dir = 0;
    paddle[1].y_dir = 0;
    paddle[1].x_pos = X_MIN;
    paddle[1].y_pos = 3;

    /* Set a random starting direction for the ball */
    switch(random(1, 5)) {
    case 1:
        ball.x_dir=-1;
        ball.y_dir=-1;
        break;
    case 2:
        ball.x_dir=+1;
        ball.y_dir=-1;
        break;
    case 3:
        ball.x_dir=-1;
        ball.y_dir=+1;
        break;
    case 4:
        ball.x_dir=+1;
        ball.y_dir=+1;
        break;
    }

    player_won = false;
    player_won_id = 0;
}

/* Duration of time to wait before switching to next updated buffer */
#define BUFFER_REFRESH_DELAY 0.15

void setup() {
    /* Initialise with custom brightness and frame rate */
    my_display.initialise(BRIGHTNESS, FRAME_RATE);

    /* Initialise pong */
    init_pong();

    /* Load starting frame into buffer */
    for (uint8_t i=0; i<LINE_COUNT; ++i)
        buffer[i] = start[i];
}

void loop() {
    my_display.showFrame(buffer, BUFFER_REFRESH_DELAY);
    if (player_won == false) {
        move_paddle();
        player_won_id = move_ball();
        if (player_won_id != 0) {
            player_won = true;
            /* Load player win frame into buffer */
            for (uint8_t i=0; i<LINE_COUNT; ++i)
#ifndef SINGLE_PLAYER_MODE
                buffer[i] = win_p[player_won_id-1][i];
#else
                buffer[i] = win_c[i];
#endif
        }
    }
}
