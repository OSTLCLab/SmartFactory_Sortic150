# structure

Sortic150 consists of several modules located in the sw-modules directory.
The individual modules are presented below:

# module-chassis

## Requirements

- [platformio](http://docs.platformio.org/en/v0.6.0/installation.html)

## Build

Build&Upload
`python -m platformio run -t upload`

## API IN

    Run action with index 0 with params 100.
    `a0[100]`

## API Out

    Sends all the sensor values as shown below:
    `[s0,s1,...,sn]`

# module-raspbi

## Requirements

- RaspberryPi(Model 3B+)
- SD-Card(16Gb)
- Touch-Screen

## Setup Instructions

### Before you begin

1. [Download Raspbian Buster with desktop](https://downloads.raspberrypi.org/raspbian_latest)
2. Follow Installing instructions [installing-images](https://www.raspberrypi.org/documentation/installation/installing-images/README.md)
3. Insert the sd-card and put the device together(plug-in the usb cables etc.)

### Set Up The RaspberryPi

1. Connect the RaspberryPi with the Internet
2. Install latest the Node-Red [follow instructions](https://nodered.org/docs/getting-started/raspberrypi)

### Configure Node-Red

1. Copy/paste the node-red.json into running node-red
2. Install node-red-dashboard
3. Autostart node-red on boot: sudo systemctl enable nodered.service

### Finaly

1. [Autostart Chromium in Kioskmode](https://itrig.de/index.php?/archives/2309-Raspberry-Pi-3-Kiosk-Chromium-Autostart-im-Vollbildmodus-einrichten.html)

### Testing

ip_raspbi:1880/ui

Show connected devices
`python -m platformio device list`

Show API OUT from device(needs sudo)
`python -m platformio device monitor`
