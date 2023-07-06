/******************************************************************
* File name: LED8x11_display_example2.ino
* Version: v1.2.1
* DEV: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display to
*   adjust brightness. The end result can be jarring as
*   the PWM frequency is not high enough to mask the switching.
*   SEIZURE WARNING!
******************************************************************/

/* Flags */
// #define DEBUG_MODE

/* Display size */
#define LINE_LENGTH 11
#define LINE_COUNT 8

/* Display brightness (0 to 1) */
#define BRIGHTNESS 0.2

/* Refresh rate data */
/* Minimum parasitic time between each line in microseconds */
#define PARASITIC_LINE_TIME 192
/* Minimum parasitic time between each frame in milliseconds (including intermediate resets) */
#define PARASITIC_FRAME_TIME 1.6
/* Targeted display refresh rate in Hz */
#define REFRESH_RATE 60
/* Calculated line time used to match delay */
uint16_t line_time_delay; // Initialised in setup()

/* Next scan line data */
#define LAST_LINE LINE_COUNT
#define FIRST_LINE 1
uint8_t next_line; // Initialised in setup()

/* Connector length */
#define CON_LEN 6
/* Connector pin names */
enum con_pin_names {RESET, EOUT, DCLK, VSYNC, HSYNC, DATA};
/* Assigned connector pin numbers */
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

const int con[CON_LEN] = {
    D12, // RESET
    D11, // EOUT (Use a PWM compatable pin for brightness control)
    D10, // DCLK
    D9, // VSYNC
    D8, // HSYNC
    D7  // DATA
};

/* Macro for setup delay */
#define SETUP_DELAY() {delayMicroseconds(1);}

/* Macro to send line select data to display */
#define SEND_H(dat) { \
    digitalWrite(con[HSYNC], LOW); \
    digitalWrite(con[VSYNC], LOW); \
    digitalWrite(con[DATA], dat); \
    SETUP_DELAY(); \
    digitalWrite(con[HSYNC], HIGH); \
    SETUP_DELAY(); \
    digitalWrite(con[HSYNC], LOW); \
    }

/* Function headers */
void sendLine(const uint8_t[LINE_LENGTH]);
void nextLine();
void displayReset();
void displayRefresh();
void setup();
void loop();

/* Function to send line data to display */
void sendLine(const uint8_t dat[LINE_LENGTH]) {
    /* Prep for sending line data to display */
    digitalWrite(con[HSYNC], LOW);
    digitalWrite(con[VSYNC], LOW);
    digitalWrite(con[DCLK], LOW);

#ifdef DEBUG_MODE
    Serial.print(" {START, ");
    for (int i=LINE_LENGTH-1; i>=0; --i) {
            Serial.print(dat[i]);
            Serial.print(", ");
    }
    Serial.print("END}");
#endif

    /* Send data to display back to front to match orientation */
    for (int i=LINE_LENGTH-1; i>=0; --i) {
        digitalWrite(con[DATA], !dat[i]);
        SETUP_DELAY();
        digitalWrite(con[DCLK], HIGH);
        SETUP_DELAY();
        digitalWrite(con[DCLK], LOW);
    }
    /* Cleanup */
    digitalWrite(con[DATA], LOW);
}

/* Function to send line data to display.
* Overloaded form to accept bit arrays */
void sendLine(const uint16_t dat) {
    /* Prep for sending line data to display */

    digitalWrite(con[HSYNC], LOW);
    digitalWrite(con[VSYNC], LOW);
    digitalWrite(con[DCLK], LOW);

#ifdef DEBUG_MODE
    Serial.print(" {START, ");
    for (uint16_t i=0x1; i<=0x400; i<<=1) {
            Serial.print(!(dat&i));
            Serial.print(", ");
    }
    Serial.print("END}");
#endif

    /* Send data to display back to front to match orientation */
    for (uint16_t i=0x1; i<=0x400; i<<=1) {
        digitalWrite(con[DATA], !(dat&i));
        SETUP_DELAY();
        digitalWrite(con[DCLK], HIGH);
        SETUP_DELAY();
        digitalWrite(con[DCLK], LOW);
    }
    /* Cleanup */
    digitalWrite(con[DATA], LOW);
}

/* Function to select next line (loop if end) */
void nextLine() {
    /* Select next scan line */
    switch (next_line)
    {
    case FIRST_LINE:
        /* Send HIGH signal to start scanning display */
        SEND_H(HIGH);
        /* Shift to select first line */
        SEND_H(LOW);
        break;
    default:
        /* Shift to select next line */
        SEND_H(LOW);
        break;
    }
    /* Increment line number or loop back to first line */
    if (++next_line > LAST_LINE)
        next_line = FIRST_LINE;
}

/* Function to reset all display LEDs */
void displayReset() {
    /* Disable line output */
    digitalWrite(con[EOUT], HIGH);
    /* Prep for sending reset signal */
    digitalWrite(con[VSYNC], LOW);
    digitalWrite(con[HSYNC], LOW);
    digitalWrite(con[DCLK], LOW);
    digitalWrite(con[DATA], LOW);
    /* Send reset signal */
    digitalWrite(con[RESET], LOW);
    SETUP_DELAY();
    digitalWrite(con[RESET], HIGH);
    /* Reset next line number */
    next_line = FIRST_LINE;
    /* Enable output */
    analogWrite(con[EOUT], 255*(1-BRIGHTNESS));
}

/* Function to update display to show latest data */
void displayRefresh() {
    /* Disable line output */
    digitalWrite(con[EOUT], HIGH);
    /* Move to next line */
    nextLine();
    /* Latch new data to output */
    digitalWrite(con[VSYNC], LOW);
    SETUP_DELAY();
    digitalWrite(con[VSYNC], HIGH);
    SETUP_DELAY();
    digitalWrite(con[VSYNC], LOW);
    /* Enable output */
    analogWrite(con[EOUT], 255*(1-BRIGHTNESS));
}

/* Test frame 1 */
// const uint8_t display_data[LINE_COUNT][LINE_LENGTH] = { 
//                                                 {1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0},  // 0
//                                                 {1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},  // 1
//                                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 2
//                                                 {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},  // 3
//                                                 {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},  // 4
//                                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 5
//                                                 {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},  // 6
//                                                 {1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0}}; // 7
// //                                               0  1  2  3  4  5  6  7  8  9 10

/* Test frame 1 as a bit array */
const uint16_t display_data[LINE_COUNT] = { 0b11001110010,  // 0
                                            0b11000100011,  // 1
                                            0b00000000000,  // 2
                                            0b10001110001,  // 3
                                            0b10001110001,  // 4
                                            0b00000000000,  // 5
                                            0b01000100001,  // 6
                                            0b10001110010}; // 7

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
    /* Erase */
    TCCR2B &= ~0b111;
    /* Set */
    TCCR2B |= 0b001;

    /* Initialise data */
    next_line = FIRST_LINE;
    /* Set line time delay from targeted refresh rate */
    if (REFRESH_RATE > 625)
        /* Cap maximum refresh rate to 625Hz (maximum measured refresh rate) */
        line_time_delay = 0;
    else if (REFRESH_RATE < 2)
        /* Cap minimum refresh rate to 2Hz (minimum measured 7.5Hz) */
        line_time_delay = (uint16_t) 65535;
    else
        line_time_delay = (((1.0/REFRESH_RATE)/8.0)-PARASITIC_LINE_TIME*1E-6)*1E+6;

    /* Set output mode for all connector pins */
    for (uint8_t i=0; i<CON_LEN; ++i) {
        pinMode(con[i], OUTPUT);
    }
    /* Initialise all pins */
    digitalWrite(con[RESET], LOW);
    digitalWrite(con[EOUT], HIGH);
    digitalWrite(con[DCLK], LOW);
    digitalWrite(con[VSYNC], LOW);
    digitalWrite(con[HSYNC], LOW);
    digitalWrite(con[DATA], LOW);

#ifdef DEBUG_MODE
    Serial.begin(9600);
    Serial.print("Line time: ");
    Serial.print(line_time_delay);
    Serial.print("\n");
#endif

    /* Wait for some time */
    SETUP_DELAY();

    /* Reset display */
    displayReset();
}

void loop() {
    /* Loop through each line in test frame */
    for (uint8_t i=0; i<LINE_COUNT; ++i) {

#ifdef DEBUG_MODE
            Serial.print("Line[");
            Serial.print(i);
            Serial.print("]|CL[");
            Serial.print(next_line);
            Serial.print("]:");
            Serial.print("\n");
#endif

        /* Send ith line to display */
        sendLine(display_data[i]);
        /* Show line */
        displayRefresh();
        /* Wait to match refresh rate */
        delayMicroseconds(line_time_delay);
    }
    /* Optional display reset between each frame (will slightly effect display framerate) */
    displayReset();
}
