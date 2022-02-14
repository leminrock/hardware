#!/usr/bin/env python3

import os
from datetime import datetime

path = './logger.txt'

with open(path, 'a') as logger:
    logger.write(f"{datetime.now()} committed on {os.uname().machine}\n")



