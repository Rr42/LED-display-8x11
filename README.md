# 8 by 11 LED display
A basic 8x11 LED display that can be driven by an Arduino or a Raspberry Pi board.

The LED matrix and hardware driver are designed using KiCAD (5.1.6) and the layout is made with perfboards in mind, so the front layer can be used as wires and the back layer for solder traces.

> The `Part-list.xlsx` spreadsheet does not reflect the BOM of this project.

To do:
* Firmware driver and APIs for Raspberry Pi.
    * Example code (Python and C++)
* APIs for Arduino.
    * Example code (Arduino C++)
* Add silkscreen for J1 connector pin functions.

![Rendered LED display](./Hardware/LED-display/LED-display-raytraced.png "Rendered LED display")
