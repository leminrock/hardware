#!/usr/bin/env python3

import mraa
import time
import sys

class Counter:
    start = 0
    end = 0

c = Counter()

# GPIO
pin = 5;

def isr_routine(gpio):
    if not gpio.read():
        c.start = time.time()
        while True:
            if not gpio.read():
                past = time.time() - c.start
                if past >= 2:
                    print('attivato!')
                    break
                else:
                    time.sleep(0.1)
            else:
                print('sollevato')
                break

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

