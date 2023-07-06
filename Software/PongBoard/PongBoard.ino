/******************************************************************
* File name: PongBoard.ino
* Version: v1.0
* DEV: GitHub@Rr42
* Description:
*  Firmware for the Pong game board using the 8x11 LED display
******************************************************************/

/* Includes */
#include <LED8x11_display_lib.h>

/* Alternative display method */
#define ALTERNATIVE_DISPLAY

/* Alternative init method */
#define ALTERNATIVE_INIT

/* Arduino Nano pin mapping */
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

/* Display frame rate [works with alternative init method] */
#define FRAME_RATE 75

/* Display brightness (0 to 1) [works with alternative init method] */
float brightness = 1.0;

/* Set single player mode */
bool single_player_mode = true;

enum pinNames {BCNTL, BPOW, LLB, ULB, UMB, URB, LRB, DDATA, DHSYNC, DVSYNC, DDCLK, DEOUT, DRST};
int pinList[] = {A0_D14,  // [A0] Brightness control
                 D13,     // [D13] Brightness POT power
                 D2,      // [D2] Lower left button (click to GND)
                 D3,      // [D3] Upper left button (click to GND)
                 D4,      // [D4] Upper middle button (click to GND)
                 D5,      // [D5] Upper right button (click to GND)
                 D6,      // [D6] Lower right button (click to GND)
                 D7,      // [D7] Display DATA
                 D8,      // [D8] Display HSYNC
                 D9,      // [D9] Display VSYNC
                 D10,     // [D10] Display DCLK
                 D11,     // [D11] Display EOUT
                 D12};    // [D12] Display RST

/* Test frame as a bit array */
const uint16_t display_data[LINE_COUNT] = { 0b10101010101,  // 0
                                            0b10101010101,  // 1
                                            0b01010101010,  // 2
                                            0b10101010101,  // 3
                                            0b10101010101,  // 4
                                            0b01010101010,  // 5
                                            0b10101010101,  // 6
                                            0b10101010101}; // 7

#define DISPLAY_TIME 0.1

/* Create Display driver object */
LedDisplay my_display(
    pinList[DRST], // RESET
    pinList[DEOUT], // EOUT (Use a PWM compatable pin for brightness control)
    pinList[DDCLK], // DCLK
    pinList[DVSYNC], // VSYNC
    pinList[DHSYNC], // HSYNC
    pinList[DDATA]  // DATA
);

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
const uint16_t start_2p[LINE_COUNT] = { 0b01111111110,
                                        0b00000000000,
                                        0b00000000000,
                                        0b10000000001,
                                        0b10000000001,
                                        0b00000000000,
                                        0b00000000000,
                                        0b01111111110};

const uint16_t start_1p[LINE_COUNT] = { 0b11111111110,
                                        0b10000000000,
                                        0b10000000000,
                                        0b10000000001,
                                        0b10000000001,
                                        0b10000000000,
                                        0b10000000000,
                                        0b11111111110};

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
/* Computer win frame */
const uint16_t win_c[LINE_COUNT] = {0b11111111111,
                                    0b10000000001,
                                    0b10010001001,
                                    0b10000000001,
                                    0b10011111001,
                                    0b10100000101,
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
* 1   2 | -1 -1 -1 | -1 00 +1
*       | 00 dy 00 | -1 dx +1
* 3   4 | +1 +1 +1 | -1 00 +1
*/
#define PLAYER1_ID 1
#define PLAYER2_ID 2
bool player_won;
uint8_t player_won_id;


struct positionVector{
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
    if (!single_player_mode){
        buffer[paddle[1].y_pos] &= 0b01111111111;
        buffer[paddle[1].y_pos+1] &= 0b01111111111;
    }

    /* Player 1 paddle movement */
    if (digitalRead(pinList[URB]) == LOW) {
        /* Move up player 1 paddle */
        if (paddle[0].y_pos != Y_MIN)
            paddle[0].y_pos -= 1;
    } else if (digitalRead(pinList[LRB]) == LOW) {
        /* Move down player 1 paddle */
        if (paddle[0].y_pos != Y_MAX-1)
            paddle[0].y_pos += 1;
    }

    if (!single_player_mode){
      /* Player 2 paddle movement */
      if (digitalRead(pinList[ULB]) == LOW) {
            /* Move up player 2 paddle */
            if (paddle[1].y_pos != Y_MIN)
                paddle[1].y_pos -= 1;
      } else if (digitalRead(pinList[LLB]) == LOW) {
            /* Move down player 2 paddle */
            if (paddle[1].y_pos != Y_MAX-1)
                paddle[1].y_pos += 1;
      }
    }
    /* Update new paddle positions */
    buffer[paddle[0].y_pos] |= 0b00000000001;
    buffer[paddle[0].y_pos+1] |= 0b00000000001;
    if (!single_player_mode){
        buffer[paddle[1].y_pos] |= 0b10000000000;
        buffer[paddle[1].y_pos+1] |= 0b10000000000;
    }
}

/* Function to move the ball or return winning player number */
uint8_t move_ball(bool reflect=false) {
    /* Remove previous position in buffer if ball is not being reflected */
    if (!reflect)
        buffer[ball.y_pos] &= ( ( ~(0x1<<ball.x_pos) )|0b10000000001 );

    /* Move */
    int tmp_x = ball.x_pos;
    int tmp_y = ball.y_pos;
    ball.x_pos += ball.x_dir;
    ball.y_pos += ball.y_dir;

    /* Reflect */
    bool move_again = false;
    /* Player sides flipped by display algo! */
    if (!reflect){
        if (ball.x_pos == X_MAX) {
            ball.x_dir *= -1;
            if (!single_player_mode)
                /* Check if ball hits player 2 paddle (include the -1 part to make it easy mode) */
                if (ball.y_pos == paddle[1].y_pos || ball.y_pos == paddle[1].y_pos+1 || ball.y_pos == paddle[0].y_pos-1)
                    move_again = true;
                else
                    return PLAYER1_ID;
            else
                move_again = true;
        } else if (ball.x_pos == X_MIN) {
            ball.x_dir *= -1;
            /* Check if ball hits player 1 paddle (include the -1 part to make it easy mode) */
            if (ball.y_pos == paddle[0].y_pos || ball.y_pos == paddle[0].y_pos+1 || ball.y_pos == paddle[0].y_pos-1)
                move_again = true;
            else
                return PLAYER2_ID;
        }
    }

    if (ball.y_pos == Y_MAX || ball.y_pos == Y_MIN) {
        ball.y_dir *= -1;
        move_again = true;
    }

    /* If an edge/paddle was hit move the ball */
    if (move_again){
        /* Go back to one move before reflecting */
        ball.x_pos = tmp_x;
        ball.y_pos = tmp_y;
        /* Reflect */
        return move_ball(true);
    }

    /* Set new position in buffer */
    buffer[ball.y_pos] |= (0x1<<ball.x_pos);
    return 0;
}

/* Function to initialise pong */
void init_pong(){
    /* Setup player paddles */
    pinMode(pinList[ULB], INPUT_PULLUP);
    pinMode(pinList[LLB], INPUT_PULLUP);
    pinMode(pinList[URB], INPUT_PULLUP);
    pinMode(pinList[LRB], INPUT_PULLUP);

    /* Set the random seed using a some (hopefully) unconnected pin */
    randomSeed(analogRead(1));

    /* Set a starting position for the ball */
    ball.x_pos = (X_MIN+X_MAX)/2;
    ball.y_pos = (Y_MIN+Y_MAX)/2;
    // ball.x_pos = random(X_MIN+2, X_MAX-2);
    // ball.y_pos = random(Y_MIN+2, Y_MAX-2);

    /* Setup player 1 paddle */
    paddle[0].x_dir = 0;
    paddle[0].y_dir = 0;
    paddle[0].x_pos = X_MAX;
    paddle[0].y_pos = 3;

    /* Setup player 2 paddle */
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
#define BUFFER_REFRESH_DELAY 0.1
#define MOVE_DELAY 0.3

float move_time_counter = 0;

void setup() {
    /* Initialise buttons */
    pinMode(pinList[UMB], INPUT_PULLUP);

    /* Initialise brightness control */
    pinMode(pinList[BCNTL], INPUT);
    pinMode(pinList[BPOW], OUTPUT);
    digitalWrite(pinList[BPOW], HIGH);

    /* Get brightness */
    brightness = analogRead(pinList[BCNTL])/256.0;

    /* Initialise with custom brightness and frame rate */
    my_display.initialise(brightness, FRAME_RATE);

    /* Initialise pong */
    init_pong();

    /* Load starting frame into buffer */
    if (single_player_mode){
        for (uint8_t i=0; i<LINE_COUNT; ++i)
            buffer[i] = start_1p[i];
    }else{
        for (uint8_t i=0; i<LINE_COUNT; ++i)
            buffer[i] = start_2p[i];
    }
}

void loop() {
    /* Check if mode button is pressed */
    if (!digitalRead(pinList[UMB])){
        /* Toggle mode */
        single_player_mode = !single_player_mode;
        /* Re-initialise pong */
        init_pong();
        /* Load new starting frame into buffer */
        if (single_player_mode){
            for (uint8_t i=0; i<LINE_COUNT; ++i)
                buffer[i] = start_1p[i];
        }else{
            for (uint8_t i=0; i<LINE_COUNT; ++i)
                buffer[i] = start_2p[i];
        }
    }
    my_display.showFrame(buffer, BUFFER_REFRESH_DELAY);
    if (player_won == false) {
        move_paddle();
        /* Move ball based on an independent delay */
        move_time_counter += BUFFER_REFRESH_DELAY;
        if (move_time_counter >= MOVE_DELAY){
            player_won_id = move_ball();
            move_time_counter = 0;
        }
        if (player_won_id != 0) {
            player_won = true;
            /* Load player win frame into buffer */
            if (!single_player_mode)
                for (uint8_t i=0; i<LINE_COUNT; ++i)
                    buffer[i] = win_p[player_won_id-1][i];
            else
                for (uint8_t i=0; i<LINE_COUNT; ++i)
                    buffer[i] = win_c[i];
        }
    }
}
