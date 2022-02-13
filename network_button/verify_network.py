#!/usr/bin/env python3

import os
import sys
import nmcli
import subprocess as sp


def verify_wifi():
    return nmcli.radio.wifi()


def verify_hotspot():
    res = sp.check_output(['systemctl', 'show', 'hostapd']).decode('utf-8')
    res_list = [x.split('=') for x in res.split('\n')]
    response = [x[1] for x in res_list if x[0] == 'ActiveState'][0]
    return response == 'active'


if __name__ == '__main__':
    if os.getuid() != 0:
        print("you must to be root")
        sys.exit(1)

    print("wifi:", verify_wifi())
    print("hotspot:", verify_hotspot())
