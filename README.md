# 8 by 11 LED display
A basic 8x11 LED display that can be driven by an Arduino or a Raspberry Pi board.

The LED matrix and hardware driver are designed using KiCAD (5.1.6) and the layout is made with perfboards in mind, so the front layer can be used as wires and the back layer for solder traces.

> The `Part-list.xlsx` spreadsheet does not reflect the BOM of this project.

### Rendered LED display
![Rendered LED display](./Hardware/LED-display/LED-display-raytraced.png "Rendered LED display")
### Fabricated and assembled LED display
![Fabricated and assembled LED display](./LED-display-fab-assimbled-2.jpg "Fabricated and assembled LED display")

## License
The Software and firmware of this project is licensed under the the MIT license however the hardware is under the [Open Source Hardware](https://www.oshwa.org/definition/) license.

## To do
* Firmware driver and APIs for Raspberry Pi.
    * Example code (Python and C++)
* APIs for Arduino.
    * Example code (Arduino C++)
* Silkscreen for J1 connector pin functions.
* Documentation for hardware
