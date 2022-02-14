#!/usr/bin/env python3

from datetime import datetime

path = './logger.txt'

with open(path, 'a') as logger:
    logger.write(f"{datetime.now()} committed!\n")



