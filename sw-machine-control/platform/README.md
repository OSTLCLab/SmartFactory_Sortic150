# Platform

## Requirements
 - (platformio)[http://docs.platformio.org/en/v0.6.0/installation.html]
 - nodered

## Build

Install Libs --> platformio run
Upload --> platformio run -t

## API

 1. S->{"method":"initPosition", "params":"1"}: Wartet, bis der Greifarm in der Initposition ist.
 2. S<-{actualPosition: 5}: Der Greifarm befindet sich an Position 5 (Startposition).
 3. S->{"method":"askForSortJob", "params":{"dest":"?","handlingUnit":"?","id":[4,43,117,162,231,73,128]}}: Wohin gehört das Packet mit dieser id?
 4. S<-{"dest":16,"handlingUnit":3, "id":[4,43,117,162,231,73,128]}:Das Packet mit der id gehört an dest 16 handlingUnit 3.
 5. S->{"method":"pickup", "params":1}: Wurde das Packet entnommen?
 6. S<-{actualPosition: 2}: Der Greifarm ist in der Position 2.
 7. S->{"method":"drop", "params":0}: Hat der Greifarm das Packet abgelegt?
 8. S<-{actualPosition: 3}: Der Greifarm hat das Packet abgelegt und befindet sich an der Position 3.

S<-{"getState":true}: Schalte Statusmeldungen ein (0 Waiting, 1 Finish, 2 Running, 3 Invalid). 
S->{"method":"response", "params":{"success":true}}: Ok, alles klar.