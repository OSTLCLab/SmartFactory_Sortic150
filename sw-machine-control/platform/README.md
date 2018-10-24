# Platform

## Requirements
 - (platformio)[http://docs.platformio.org/en/v0.6.0/installation.html]
 - nodered

## Build

Install Libs --> platformio run
Upload --> platformio run -t

## API IN 

    Run action with index 0 with params 100.
    ``` a0[100]```

    Create a step with 2 conditions. If sensor with index 1 is equal to 0 
    and sensor with index 1 is greater than 38 then perform action with index 1 with params 100.
    ``` s[2(1a0&1c38)1(100)]```

### API Out

    Sends all the sensor values as shown below:
    ``` [s0,s1,...,sn] ```