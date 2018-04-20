# Send Commands To sortic150

These params would be sended via the serial port as JSON:

```
{
  "chassisStart": 23,
  "powerOn": true,
  "handlingUnitInitPosition": 5,
  "rfidSourcePosition": 1,
  {"id":[7,7,6,6,162,231,73,128],"dest":30,"placer":3}
}
```

example change sleepposition:

```
{"placerSleepPosition": 5}
```

example define sortjob:

```
{"dest":"35","handlingUnit":"4","id":[4,186,115,162,231,73,128]}
```

# Configfile

The configfile is located at `Embedded/config.h`
Here you can edit the pins or enable/disable debugmode.
Remove the entrya to disable debugmode and add the entry `DEBUG` to enable it.
