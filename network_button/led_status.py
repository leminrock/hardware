#!/usr/bin/env python3

import os
import sys
import mraa
import time

ROOT_PERMISSION = "YOU MUST TO BE ROOT TO RUN THIS FILE"
ARGUMENT_MISSING = "BLINKTYPE ARGUMENT MISSING"
POSSIBLE_ARGUMENTS = "PLEASE CHOOSE ARGUMENT IN\non | off | fast | slow"
PIN = 3
FAST_DELAY = 0.2
SLOW_DELAY = 0.75


def led_config(pin):
    led = mraa.Gpio(pin)
    led.dir(mraa.DIR_OUT)
    return led


def led_on(led):
    led.write(1)


def led_off(led):
    led.write(0)


def led_fast_blink(led):
    state = 0
    while True:
        state = not state
        led.write(state)
        time.sleep(FAST_DELAY)


def led_slow_blink(led):
    state = 0
    while True:
        state = not state
        led.write(state)
        time.sleep(SLOW_DELAY)


STATES = {
    "on": led_fixed,
    "off": led_off,
    "fast": led_fast_blink,
    "slow": led_slow_blink
}

if __name__ == '__main__':
    if os.getuid() != 0:
        print(ROOT_PERMISSION)
        sys.exit(1)
    if len(sys.argv) < 2:
        print(ARGUMENT_MISSING)
        sys.exit(1)
    else:
        blink = sys.argv[1]

    try:
        STATES[blink]()
    except KeyError:
        print(POSSIBLE_ARGUMENTS)
