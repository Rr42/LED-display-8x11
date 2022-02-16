/******************************************************************
* File name: LED8x11_display_lib.h
* Version: v1.0
* Dev: GitHub@Rr42
* Description:
*  Display library header for the 8x11 LED display.
******************************************************************/
#ifndef __LED8X11_DISPLAY_LIB__

#define __LED8X11_DISPLAY_LIB__
/* Includes */
#include <wiringPi.h>
#include <inttypes.h>

/* Display size */
#define LINE_LENGTH 11
#define LINE_COUNT 8

/* Referesh rate calibration data */
/* Minimum parasitic time between each line in microseconds */
#define PARASITIC_LINE_TIME 192
/* Minimum parasitic time between each frame in milliseconds (including intermediate resets) */
#define PARASITIC_FRAME_TIME 1.6
/* Targeted default display refresh rate in Hz */
#define DEFAULT_REFRESH_RATE 60

/* Connector length */
#define CON_LEN 6

/* Display driver class */
class LedDisplay {
private:
    /* Display brightness (0 to 1) */
    float brightness;
    /* Calculated line time used to match delay */
    uint16_t line_time_delay;
    /* Display refresh rate in Hz */
    float refresh_rate;
    /* Storage for next line index */
    uint8_t next_line; // Initialised in setup()
    /* Connector pin map */
    const uint8_t RESET, EOUT, DCLK, VSYNC, HSYNC, DATA;

protected:
    /* Protected function headers */
    void nextLine(void);

public:
    /* Class constructor */
    LedDisplay(uint8_t RESET, uint8_t EOUT, uint8_t DCLK, uint8_t VSYNC, uint8_t HSYNC, uint8_t DATA);
    /* Class destructor */
    ~LedDisplay(void);

    /* Public function headers */
    void showFrame(const uint8_t frame[LINE_COUNT][LINE_LENGTH], float time);
    void showFrame(const uint16_t frame[LINE_COUNT], float time);
    void initialise(float brightness, float refresh_rate);
    void sendLine(const uint8_t line[LINE_LENGTH]);
    void setRefreshRate(float refresh_rate);
    void setBrightness(float brightness);
    void sendLine(const uint16_t line);
    uint16_t getLineDelay(void);
    void displayRefresh(void);
    void displayReset(void);
    void initialise(void);
};
#endif
