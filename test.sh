#!/usr/bin/env bash

while :
do
    var status=mraa-gpio get 3
    mraa-gpio set 3 ! status
done
