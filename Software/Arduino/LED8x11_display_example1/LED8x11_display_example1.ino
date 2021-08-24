/******************************************************************
* File name: LED8x11_display_example1.ino
* Version: v1.0
* DEV: GitHub@Rr42
* Description:
*  Example Arduino program for the 8x11 LED display to switch
*   between 2 patterns.
******************************************************************/

/* Flags */
// #define DEBUG_MODE

/* Display size */
#define LINE_LENGTH 11
#define LINE_COUNT 8

/* Referesh rate data */
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
const int con[CON_LEN] = {
    53, // RESET
    52, // EOUT
    51, // DCLK
    50, // VSYNC
    49, // HSYNC
    48, // DATA
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

/* Function to select next line (loop if end) */
void nextLine() {
    /* Select next scan line */
    switch (next_line)
    {
    case FIRST_LINE:
        /* Send HIGH signal to start scaning display */
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
    digitalWrite(con[EOUT], LOW);
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
    digitalWrite(con[EOUT], LOW);
}

/* Test frame 1 */
const uint8_t display_data[LINE_COUNT][LINE_LENGTH] = { 
//                                               0  1  2  3  4  5  6  7  8  9 10
                                                {1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0}, // 0
                                                {1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1}, // 1
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 2
                                                {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1}, // 3
                                                {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1}, // 4
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
                                                {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, // 6
                                                {1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0}  // 7
                                                };

/* Test frame 2 */
const uint8_t display_data_2[LINE_COUNT][LINE_LENGTH] = { 
//                                               0  1  2  3  4  5  6  7  8  9 10
                                                {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0
                                                {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 1
                                                {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 2
                                                {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 3
                                                {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 4
                                                {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 5
                                                {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 6
                                                {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}  // 7
                                                };

/* Time interval between frame switching in seconds */
#define SWITCH_TIME 2
uint16_t switch_count;
uint16_t switch_count_max;
uint8_t switch_frame;

void setup() {
    /* Initialise data */
    next_line = FIRST_LINE;
    /* Set line time delay from targeted refresh rate */
    if (REFRESH_RATE > 625)
        /* Cap maximum referesh rate to 625Hz (maximum measured referesh rate) */
        line_time_delay = 0;
    else if (REFRESH_RATE < 2)
        /* Cap minimum referesh rate to 2Hz (minimum measured 7.5Hz) */
        line_time_delay = (uint16_t) 65535;
    else
        line_time_delay = (((1.0/REFRESH_RATE)/8.0)-PARASITIC_LINE_TIME*1E-6)*1E+6;

    /* Calculate number of refreshes between switches */
    switch_count_max = SWITCH_TIME/(1.0/REFRESH_RATE);
    /* Initialise frame switching function */
    switch_count = 0;
    switch_frame = 1;

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
    Serial.print(LINE_TIME);
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
        if (switch_frame == 1)
            sendLine(display_data[i]);
        else
            sendLine(display_data_2[i]);
        /* Show line */
        displayRefresh();
        /* Wait to match referesh rate */
        delayMicroseconds(line_time_delay);
    }
    if (++switch_count > switch_count_max) {
        switch_count = 0;
        switch_frame = !switch_frame;
    }
    // displayReset();
}