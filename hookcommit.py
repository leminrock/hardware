#!/usr/bin/env python3

import os
import shutil
import subprocess as sp
from datetime import datetime

path = './logger.txt'

with open(path, 'a') as logger:
    logger.write(f"{datetime.now()} committed on {os.uname().machine}\n")


source = './network_button/'
dest = '/etc/systemd/system/'
filename = 'button_network.service'

#shutil.copyfile(source+filename, dest+filename)
sp.run(f"sudo cp {source + filename} {dest + filename}".split())
