#!/usr/bin/env python3

import os
import mraa
import time
import sys

class Counter:
    start = 0
    led = mraa.Gpio(3)
    led.dir(mraa.DIR_OUT)

c = Counter()

# GPIO
pin = 5;
led = 3

def isr_routine(gpio):
    if not gpio.read():
        c.start = time.time()
        while True:
            if not gpio.read():
                past = time.time() - c.start
                if past >= 2:
                    print('attivato!')
                    c.led.write(not c.led.read())
                    #a_led.write(not a_led.value())
                    break
                else:
                    time.sleep(0.1)
            else:
                print('sollevato')
                break

if __name__=='__main__':
    if os.getuid():
        print('Please execute as root')
        sys.exit(1)

    try:
        # initialise GPIO
        x = mraa.Gpio(pin)

        print("Starting ISR for pin " + repr(pin))

        # set direction and edge types for interrupt
        x.dir(mraa.DIR_IN)
        x.isr(mraa.EDGE_BOTH, isr_routine, x)

        # wait until ENTER is pressed
        var = input("Press ENTER to stop")
        x.isrExit()
    except ValueError as e:
        print(e)

