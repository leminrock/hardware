#!/usr/bin/env python3

import os
import sys
import nmcli
import subprocess as sp


def verify_wifi(): 
    state = 0
    if nmcli.device.wifi():
        print("wifi active")
        state = 1
    else:
        print("wifi not active")

    return state

def verify_hotspot():
    state = 0
    res = sp.check_output(['systemctl','show','hostapd']).decode('utf-8')
    res_list = [x.split('=') for x in res.split('\n')]
    response = [x[1] for x in res_list if x[0] == 'ActiveState'][0]
    
    if response == 'active':
        print('hotspot active')
        state = 1
    else:
        print("hotspot not active")

    return state

if __name__=='__main__':
    if os.getuid() != 0:
        print("you must to be root")
        sys.exit(1)

    if verify_wifi() or verify_hotspot():
        print("something moves")
    else:
        print("all dies")
    
