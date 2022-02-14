#!/usr/bin/env python3

from datetime import datetime

path = './logger.txt'

with open(path, 'a') as logger:
    loffer.write(f"{datetime.now()} committed!\n")



