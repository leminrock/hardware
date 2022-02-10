sudo -i -- sh -c 'systemctl stop wpa_supplicant && systemctl stop dnsmasq && systemctl stop hostapd && systemctl restart dhcpcd && systemctl start dnsmasq && systemctl start hostapd'
