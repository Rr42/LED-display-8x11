/******************************************************************
* File name: LED8x11_display_lib.cpp
* Version: v1.0
* DEV: GitHub@Rr42
* Description:
*  Display library implimentation for the 8x11 LED display.
******************************************************************/

#include "LED8x11_display_lib.h"

/* Next scan line data */
#define LAST_LINE LINE_COUNT
#define FIRST_LINE 1

/* Macro for setup delay */
#define SETUP_DELAY() {delayMicroseconds(1);}

/* Macro to send line select data to display */
#define SEND_H(dat) { \
    digitalWrite(this->HSYNC, LOW); \
    digitalWrite(this->VSYNC, LOW); \
    digitalWrite(this->DATA, dat); \
    SETUP_DELAY(); \
    digitalWrite(this->HSYNC, HIGH); \
    SETUP_DELAY(); \
    digitalWrite(this->HSYNC, LOW); \
    }

/* Constructor to set default values when object is created */
LedDisplay::LedDisplay(uint8_t RESET, uint8_t EOUT, uint8_t DCLK, uint8_t VSYNC, uint8_t HSYNC, uint8_t DATA) : RESET(RESET), EOUT(EOUT), DCLK(DCLK), VSYNC(VSYNC), HSYNC(HSYNC), DATA(DATA) {
    /* Set 100% brightness by default */
    this->brightness = 0;
    /* Set starting line number */
    this->next_line = FIRST_LINE;
    /* Set default refresh rate */
    this->setRefreshRate(DEFAULT_REFRESH_RATE);
}

LedDisplay::~LedDisplay() {
}

/* Function to initialise display with default values */
void LedDisplay::initialise() {
    /* Set output mode for all connector pins */
    pinMode(this->RESET, OUTPUT);
    pinMode(this->EOUT, OUTPUT);
    pinMode(this->DCLK, OUTPUT);
    pinMode(this->VSYNC, OUTPUT);
    pinMode(this->HSYNC, OUTPUT);
    pinMode(this->DATA, OUTPUT);
    /* Initialise all pins */
    digitalWrite(this->RESET, LOW);
    digitalWrite(this->EOUT, HIGH);
    digitalWrite(this->DCLK, LOW);
    digitalWrite(this->VSYNC, LOW);
    digitalWrite(this->HSYNC, LOW);
    digitalWrite(this->DATA, LOW);

    /* Wait for some time */
    SETUP_DELAY();
    /* Reset display */
    this->displayReset();
}

/* Function to initialise display with user defined brightness and refresh rate */
void LedDisplay::initialise(float brightness, float refresh_rate) {
        /* Set output mode for all connector pins */
    pinMode(this->RESET, OUTPUT);
    pinMode(this->EOUT, OUTPUT);
    pinMode(this->DCLK, OUTPUT);
    pinMode(this->VSYNC, OUTPUT);
    pinMode(this->HSYNC, OUTPUT);
    pinMode(this->DATA, OUTPUT);
    /* Initialise all pins */
    digitalWrite(this->RESET, LOW);
    digitalWrite(this->EOUT, HIGH);
    digitalWrite(this->DCLK, LOW);
    digitalWrite(this->VSYNC, LOW);
    digitalWrite(this->HSYNC, LOW);
    digitalWrite(this->DATA, LOW);

    /* Set brightness */
    this->setBrightness(brightness);
    /* Set refresh rate */
    this->setRefreshRate(refresh_rate);

    /* Wait for some time */
    SETUP_DELAY();
    /* Reset display */
    this->displayReset();
}

/* Function to set display brightness (Requires EOUT pin to be PWM compatable) */
void LedDisplay::setBrightness(float brightness) {
    // if (digitalPinToTimer(this->EOUT) != NOT_ON_TIMER)
    this->brightness = 255*(1-brightness);
}

/* Function to set targeted refresh rate (may not always atchive the set refresh rate) */
void LedDisplay::setRefreshRate(float refresh_rate) {
    this->refresh_rate = refresh_rate;
    if (this->refresh_rate > 625)
        /* Cap maximum referesh rate to 625Hz (maximum measured referesh rate) */
        this->line_time_delay = 0;
    else if (this->refresh_rate < 2)
        /* Cap minimum referesh rate to 2Hz (minimum measured 7.5Hz) */
        this->line_time_delay = (uint16_t) 65535;
    else
        this->line_time_delay = (((1.0/this->refresh_rate)/8.0)-PARASITIC_LINE_TIME*1E-6)*1E+6;
}

uint16_t LedDisplay::getLineDelay() {
    return this->line_time_delay;
}

/* Function to show a frame for a given period of time in seconds */
void LedDisplay::showFrame(const uint8_t frame[LINE_COUNT][LINE_LENGTH], float time) {
    /* Calculate number of frames to be displayed to match time interval */
    uint16_t switch_count_max = time/(1.0/this->refresh_rate);
    /* Initialise frame switching function */
    uint16_t switch_count = 0;
    /* Display frame till specified time runs out by counting frames */
    while (++switch_count < switch_count_max) {
        /* Loop through each line in frame */
        for (uint8_t i=0; i<LINE_COUNT; ++i) {
            /* Send ith line to display */
            this->sendLine(frame[i]);
            /* Show line */
            this->displayRefresh();
            /* Wait to match referesh rate */
            delayMicroseconds(this->line_time_delay);
        }
    }
}

/* Function to show a frame for a given period of time in seconds.
* Overloaded form to accept bit arrays */
void LedDisplay::showFrame(const uint16_t frame[LINE_COUNT], float time) {
    /* Calculate number of frames to be displayed to match time interval */
    uint16_t switch_count_max = time/(1.0/this->refresh_rate);
    /* Initialise frame switching function */
    uint16_t switch_count = 0;
    /* Display frame till specified time runs out by counting frames */
    while (++switch_count < switch_count_max) {
        /* Loop through each line in frame */
        for (uint8_t i=0; i<LINE_COUNT; ++i) {
            /* Send ith line to display */
            this->sendLine(frame[i]);
            /* Show line */
            this->displayRefresh();
            /* Wait to match referesh rate */
            delayMicroseconds(this->line_time_delay);
        }
    }
}

/* Function to send line data to display */
void LedDisplay::sendLine(const uint8_t line[LINE_LENGTH]) {
    /* Prep for sending line data to display */
    digitalWrite(this->HSYNC, LOW);
    digitalWrite(this->VSYNC, LOW);
    digitalWrite(this->DCLK, LOW);

    /* Send data to display back to front to match orientation */
    for (int i=LINE_LENGTH-1; i>=0; --i) {
        digitalWrite(this->DATA, !line[i]);
        SETUP_DELAY();
        digitalWrite(this->DCLK, HIGH);
        SETUP_DELAY();
        digitalWrite(this->DCLK, LOW);
    }
    /* Cleanup */
    digitalWrite(this->DATA, LOW);
}

/* Function to send line data to display.
* Overloaded form to accept bit arrays */
void LedDisplay::sendLine(const uint16_t line) {
    /* Prep for sending line data to display */

    digitalWrite(this->HSYNC, LOW);
    digitalWrite(this->VSYNC, LOW);
    digitalWrite(this->DCLK, LOW);

    /* Send data to display back to front to match orientation */
    for (uint16_t i=0x1; i<=0x400; i<<=1) {
        digitalWrite(this->DATA, !(line&i));
        SETUP_DELAY();
        digitalWrite(this->DCLK, HIGH);
        SETUP_DELAY();
        digitalWrite(this->DCLK, LOW);
    }
    /* Cleanup */
    digitalWrite(this->DATA, LOW);
}

/* Function to select next line (loop if end) */
void LedDisplay::nextLine() {
    /* Select next scan line */
    switch (this->next_line)
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
    if (++this->next_line > LAST_LINE)
        this->next_line = FIRST_LINE;
}

/* Function to reset all display LEDs */
void LedDisplay::displayReset() {
    /* Disable line output */
    digitalWrite(this->EOUT, HIGH);
    /* Prep for sending reset signal */
    digitalWrite(this->VSYNC, LOW);
    digitalWrite(this->HSYNC, LOW);
    digitalWrite(this->DCLK, LOW);
    digitalWrite(this->DATA, LOW);
    /* Send reset signal */
    digitalWrite(this->RESET, LOW);
    SETUP_DELAY();
    digitalWrite(this->RESET, HIGH);
    /* Reset next line number */
    this->next_line = FIRST_LINE;
    /* Enable output */
    analogWrite(this->EOUT, this->brightness);
}

/* Function to update display to show latest data */
void LedDisplay::displayRefresh() {
    /* Disable line output */
    digitalWrite(this->EOUT, HIGH);
    /* Move to next line */
    this->nextLine();
    /* Latch new data to output */
    digitalWrite(this->VSYNC, LOW);
    SETUP_DELAY();
    digitalWrite(this->VSYNC, HIGH);
    SETUP_DELAY();
    digitalWrite(this->VSYNC, LOW);
    /* Enable output */
    analogWrite(this->EOUT, this->brightness);
}
