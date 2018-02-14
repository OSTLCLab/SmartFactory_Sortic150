# Send Commands To sortic150

These params would be sended via the serial port as JSON:
```
{
  "chassisStart": 510,
  "unknownPosition": 510,
  "rfidCount": 4,
  "powerOn": true,
  "placerSleepPosition": 5,
  "rfidSourcePosition": 1,
  "rfids": [
    {"id":[4,135,115,120,162,231,73,128],"dest":400,"placer":1},
    { "id": [4, 42, 117, 211, 162, 231, 73, 128], "dest": 300, "placer": 1 },
    { "id": [4, 161, 115, 94, 162, 231, 73, 128], "dest": 200, "placer": 1 },
    { "id": [1, 2, 3, 4, 5, 6, 7, 8], "dest": 400, "placer": 1 } 
  ]
}
```

Remove the white spaces and send each param one by one.

example new rfid chip
```
{"id":[7,7,6,6,162,231,73,128],"dest":400,"placer":1}
```

example update rfid chip
```
{"id":[4,135,115,120,162,231,73,128],"dest":510,"placer":1}
```

example change sleepposition:
```
{"placerSleepPosition": 5}
```

# Configfile
The configfile is located at ``Embedded/config.h``
Here you can edit the pins or enable/disable debugmode.
Remove the entrya to disable debugmode and add the entry ``DEBUG`` to enable it.
