# Introducton

This is the codebase of our Lego sorting robot for an educational setup as OST.
While there is another experimental implementaiton of this robot in the rep SmartFactory_SorticRoboter, this is the current stable code that is used in production. For installation, please use the "production" branch to get stable code with the latest bugfixes.


# structure

Sortic150 consists of several modules located in the sw-# Introducton

This is the codebase of our Lego sorting robot for an educational setup as OST.
While there is another experimental implementaiton of this robot in the rep SmartFactory_SorticRoboter, this is the current stable code that is used in production. For installation, please use the "production" branch to get stable code with the latest bugfixes.

This branch designed to use with a PLC compact CPU 1512C-1 PN
to resemble an real decentralized factory. 

# structure

Sortic150 consists of several modules located in the sw-modules directory.
The individual modules are presented below:

# module-chassis and module-roboter-arm

## Requirements

- [platformio](http://docs.platformio.org/en/v0.6.0/installation.html)

## Build


## API chassis

Write into memory of the PLC (DB100) with OPCua or RFC1006-communication protocol

OPCua ns=3;s="Data_Block1"."BoxAt"

IN		type		Offset	
	
done 		Bool		0  		
distanz		Int16		6
BoxAt		Int16		8		
getPackage	Bool		10	

OUT		type		Offset

sollPos	Int16		2

## API roboter-arm

gets Data with OPCua

### IN

sollPos (Int)
'1 	pickup back'
'2 	put down back'
'3 	pickup front'
'4 	put down front'

### Out

done (Bool)

## API package-reader

Sends Data with MQTT

### IN

-

### Out

getPackage	Bool


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

# module-iot2050

Image from SIEMENS website. (https://support.industry.siemens.com/cs/document/109780231/simatic-iot2050-sd-card-beispielimage?dti=0&lc=de-CH)

Setup like module-raspbi. Node-red is pre-installed

differents to Raspberry Pi: 
- Nodes for Hardware-Pins (for Arduino Shields)
- in python scipts you'll need the mraa-Library to connect to GPIO's
- ...
modules directory.
The individual modules are presented below:

# module-chassis and module-roboter-arm

## Requirements

- [platformio](http://docs.platformio.org/en/v0.6.0/installation.html)

## Build

Build&Upload
`python -m platformio run -t upload`

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
