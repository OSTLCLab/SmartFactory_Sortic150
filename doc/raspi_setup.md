# Raspberry PI Bluetooth Setup

## commands

    sudo apt-get install bluetooth bluez libbluetooth-dev libudev-dev
    
    sudo apt-get install libcap2-bin
    
    npm install noble
    
    sudo setcap cap_net_raw+eip $(eval readlink -f `which node`)


## bluetoothctl-Kommando

    bluetoothctl
ist zur interaktiven Konfiguration gedacht. Nach dem Start gelangen Sie in einen Kommandomodus. Die weitere Vorgehensweise zur Verbindung eines Bluetooth-Geräts sieht so aus:

     scan on
     pair [mac-adresse]
     connect [mac-adresse]
     trust [mac-adresse]


## node-red

A Node-RED node that uses the Noble to interact with BLE devices that support the UART profile by Nordic Semiconductor

    cd $HOME/.node-red
    npm install node-red-contrib-ble-uart

