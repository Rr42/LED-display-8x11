"""#####################################################################
# File name: display8x11.py
# Version: v1.0
# Dev: GitHub@Rr42
# Description:
#  Python library for Raspberry Pi to drive the 8x11 LED display.
#####################################################################"""

# GPIO docs: https://sourceforge.net/p/raspberry-gpio-python/wiki/Examples/
# GPIO flow ref: https://sourceforge.net/p/raspberry-gpio-python/wiki/BasicUsage/
# Input ref: https://sourceforge.net/p/raspberry-gpio-python/wiki/Inputs/

import sys
import time
import RPi.GPIO as GPIO

# Callback called when switch is pressed.
def switch_callback(channel):
    print('Switch pressed, exiting.')
    GPIO.cleanup()
    sys.exit(0)

def main():
    print("in led8x11displaylib main")
    # led1 = 18  # Red
    # led2 = 22  # Green
    # led3 = 29  # Yellow
    # switch = 37

    # GPIO.setmode(GPIO.BOARD)
    # GPIO.setup(led1, GPIO.OUT)
    # GPIO.setup(led2, GPIO.OUT)
    # GPIO.setup(led3, GPIO.OUT)
    # GPIO.setup(switch, GPIO.IN)

    # GPIO.add_event_detect(switch, GPIO.FALLING, callback=switch_callback)

    # while True:
    #     for pin in [led1, led2, led3]:
    #         p = GPIO.PWM(pin, 50)
    #         p.start(0)
    #     for dc in range(0, 101, 5):
    #         p.ChangeDutyCycle(dc)
    #         time.sleep(0.05)
    #     for dc in range(100, -1, -5):
    #         p.ChangeDutyCycle(dc)
    #         time.sleep(0.05)

# Main guard
if __name__ == "__main__":
    main()
