#!/usr/bin/env bash

if [ "$UID" -ne 0 ]
then
	echo you must root to run this script;
	exit 1
fi

systemctl stop dnsmasq;
systemctl stop hostapd;
systemctl stop wpa_supplicant;
systemctl restart dhcpcd;
systemctl start dnsmasq;
systemctl start hostapd;
echo "hotspot active";
