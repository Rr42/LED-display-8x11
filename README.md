# 8 by 11 LED display
A basic 8x11 LED display that can be driven by an Arduino or a Raspberry Pi board.

The LED matrix and hardware driver are designed using KiCAD (5.1.6) and the layout is made with perfboards in mind, so the front layer can be used as wires and the back layer for solder traces.

### Rendered LED display
![Rendered LED display](./Hardware/LED-display/LED-display-raytraced.png "Rendered LED display")
### Fabricated and assembled LED display
![Fabricated and assembled LED display](./LED-display-fab-assimbled-2.jpg "Fabricated and assembled LED display")

## Using the Arduino library
To include and use the display library in sketches made using the Arduino IDE, first the library folder follow the below steps:
* Copy the `LED8x11_display_lib` folder from `LED-display/Software`
    * To `My Documents\Arduino\libraries` in Windows.
    * To `~/Documents/Arduino/libraries/` in linux. 
> For more details refer to the [libraries page](https://www.arduino.cc/en/hacking/libraries) on the Arduino website.
* Include the library header in the sketch
```cpp
#include <LED8x11_display_lib.h>
```

## Using the Raspberry Pi libraries
### Python
To use the Python display library follow the below steps:
> This Python display library requires the `RPi.GPIO` Python library. To setup `RPi.GPIO` refer to [PyPI/RPi.GPIO](https://pypi.org/project/RPi.GPIO/).
* Move to the `Software/RaspberryPi/Python/LED8x11_display_lib` directory
```
cd Software/RaspberryPi/Python/LED8x11_display_lib
```
* Run the Python setup script
> Admin/root privilages may be required to install the library
```
python3 setup.py install
```
* Import the library in your program
```python
from led8x11displaylib import display8x11
```
### C++
To use the C++ display library follow the below steps:
> This C++ display library requires the `wiringPi` library. To setup `wiringPi` refer to the official [Wiring Pi](http://wiringpi.com/download-and-install/) site.
* Move to the `Software/RaspberryPi/Cpp/LED8x11_display_lib` directory
```
cd Software/RaspberryPi/Cpp/LED8x11_display_lib
```
* Run the make command (this will build the library and all the examples)
```
make all
```
* Include the library header in your program using the following line
```cpp
#include "LED8x11_display_lib.hpp"
```
* While compilint the program link to both `wiringPi` and `led8x11display`
```
g++ myprogram.cpp -o myprogram -L<path/to/Cpp/folder>/build/lib -L/usr/lib -lled8x11display -lwiringPi ...
```

To reset/clean the C++ objectfile run the following in `Software/RaspberryPi/Cpp`:
```
make clean
```

## License
The Software and firmware of this project is licensed under the the MIT license however the hardware is under the [Open Source Hardware](https://www.oshwa.org/definition/) license.

## To do
* Firmware driver and APIs for Raspberry Pi.
    * Example code (Python and C++)
* Documentation for hardware
