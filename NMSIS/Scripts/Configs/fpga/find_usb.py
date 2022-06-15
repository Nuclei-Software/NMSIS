#!/bin/env python3
import os
import sys
import tempfile
import pprint
import serial
import serial.tools.list_ports
import json
#from prettytable import PrettyTable

def parse_usbinfo(origin):
    bus_splits = origin["bus"].split()
    bus, device = bus_splits[1], bus_splits[3].strip(":")
    vid = origin["idvendor"].split()[1]
    pid = origin["idproduct"].split()[1]
    manufact = " ".join(origin["imanufacturer"].split()[2:])
    product = " ".join(origin["iproduct"].split()[2:])
    if len(origin["iserial"].split()) > 2:
        serial = origin["iserial"].split()[2]
    else:
        serial = ""
    return {"bus": bus, "device": device, "pid": pid, "vid": vid, "serial": serial, "manufacturer": manufact, "product": product}

def find_usbinfo():
    cmdlog = tempfile.mktemp()
    os.system("lsusb -v 2>/dev/null | grep -i -E \"^bus |iserial|idvendor|idproduct|imanufacturer|iproduct\" > %s" % (cmdlog))
    usb_devices = []
    INFOCNT = 6
    with open(cmdlog, "r") as cf:
        lines = cf.readlines()
        for i in range(0, int(len(lines) / INFOCNT)):
            bus = lines[i*INFOCNT]
            if "Linux Foundation" in bus:
                continue
            if "VMware, Inc" in bus:
                continue
            if "VirtualBox" in bus:
                continue
            try:
                origin = {"bus": lines[i*INFOCNT].strip(), "idvendor": lines[i*INFOCNT + 1].strip(), \
                          "idproduct": lines[i*INFOCNT + 2].strip(), "iserial": lines[i*INFOCNT + 5].strip(), \
                          "imanufacturer": lines[i*INFOCNT + 3], "iproduct": lines[i*INFOCNT + 4]}
                parsed = parse_usbinfo(origin)
                device = {"origin": origin, "parsed": parsed}
            except:
                continue
            usb_devices.append(device)

    os.remove(cmdlog)
    return usb_devices

def find_usb_serport(ser):
    comports = serial.tools.list_ports.comports()
    serports = []
    for port in comports:
        cur_serno = port.serial_number
        if cur_serno is None:
            continue
        if cur_serno == ser:
            serports.append(port.device)
    return serports


if __name__ == '__main__':
    usb_devices = find_usbinfo()
    usb_infos = dict()
    for usb in usb_devices:
        parsed = usb["parsed"]
        serports = find_usb_serport(parsed["serial"])
        parsed["serports"] = serports
        loc = "%s:%s" % (parsed["bus"], parsed["device"])
        usb_infos[loc] = {"serial": parsed["serial"], \
                          "serports": serports, \
                          "pid": parsed["pid"], \
                          "vid": parsed["vid"], \
                          "manufacturer": parsed["manufacturer"], \
                          "product": parsed["product"], \
                          "info": usb["origin"]["bus"]}

    # save as json
    with open("usb_info.json", "w") as uf:
        json.dump(usb_infos, uf, indent=4)
    pprint.pprint(usb_infos)
