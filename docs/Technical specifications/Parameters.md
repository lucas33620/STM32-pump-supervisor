# Parameters

## Timing parameters

| Parameter                                  | Value                             | Notes                                                |
| ------------------------------------------ | --------------------------------- | ---------------------------------------------------- |
| Temperature acquisition                    | 100 ms                            |                                                      |
| Sensor communication fault confirmation    | 3 consecutive failed acquisitions | Equivalent to about 300 ms nominal detection time    |
| Invalid temperature confirmation           | 2 consecutive acquisitions        |                                                      |
| Actuator command response time             | 100 ms max                        | From threshold condition detection to command update |
| Critical task hearthbeat update periode    | 200 ms max                        |                                                      |
| Missing task heartbeat detection threshold | 400 ms                            | Beyond this, supervision fault is raised             |
| Heartbeat CAN transmission period          | 500 ms +- 50ms                    | Sent in INIT, NORMAL, and DEGRADED                   |
| Status CAN transmission period             | 500 ms +- 50ms                    | Include temperature, mode, actuator state            |
| Fault CAN reporting delay                  | 500 ms max                        | From fault detection to CAN visibility               |
| Watchdog inhibition decision time          | 400 ms max                        | After 2 consecutives failed supervision checks       |

## Control parameters

| Parameter                          | Value           | Description                                                       |
| ---------------------------------- | --------------- | ----------------------------------------------------------------- |
| Pump ON threshold                  | 40°C            | Temperature above which the actuator is commanded ON              |
| Pump OFF threshold                 | 35°C            | Temperature below which the actuator is commanded OFF             |
| Overtemperature threshold          | 70°C            | Temperature above which an overtemperature fault is raised        |
| Overtemperature recovery threshold | 60°C            | Temperature below which overtemperature recovery becomes possible |
| Valid temperature range            | -20°C to +100°C | Application-level valid sensor range                              |
