# Introducton

This is the condbase of our Lego sorting robot for an educational setup as OST.
While there is another experimental implementaiton of this robot in the rep SmartFactory_SorticRoboter, this is the current stable code that is used in production. For installation, please use the "production" branch to get stable code with the latest bugfixes.


# structure

Sortic150 consists of several modules located in the sw-modules directory.
The individual modules are presented below:

# module-chassis and module-roboter-arm

## Requirements

- [platformio](http://docs.platformio.org/en/v0.6.0/installation.html)

## Build

Build&Upload
`python -m platformio run -t upload`

## API chassis

IN
Run action with index 0 with params 100.
`a0[100]`

Out
Sends all the sensor values as shown below:
`[s0,s1,...,sn]`

## API roboter-arm

### IN

Pickup 0: left, 1: right
`pickup(0)`
`pickup(1)`

Drop 0: left, 1:right
`drop(0)`
`drop(1)`

### Out

Success:
`success(1)`
Fail:
`success(0)`

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
3. [Autostart Chromium in Kioskmode](https://itrig.de/index.php?/archives/2309-Raspberry-Pi-3-Kiosk-Chromium-Autostart-im-Vollbildmodus-einrichten.html)

### Configure Node-Red

1. Copy/paste the node-red.json into running node-red
2. Install node-red-dashboard
3. Autostart node-red on boot: sudo systemctl enable nodered.service

## Testing

ip_raspbi:1880/ui

Show connected devices
`python -m platformio device list`

Show API OUT from device(needs sudo)
`python -m platformio device monitor`
