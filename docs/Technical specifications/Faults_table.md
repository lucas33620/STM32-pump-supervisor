# Faults table

| Fault ID | Fault Name                 | Trigger Condition                                                                      | Severity | Mode Reaction                         | CAN visbility                                 |
| -------- | -------------------------- | -------------------------------------------------------------------------------------- | -------- | ------------------------------------- | --------------------------------------------- |
| F001     | Sensor communication fault | 3 consecutive sensor acquisition failures or timeouts                                  | High     | Enter DEGRADED                        | Reported in fault status message and snapshot |
| F002     | Temperature out of range   | 2 consecutive temperature values below -20°C or above +100°C                           | Medium   | Enter DEGRADED                        | Reported in fault status message and snapshot |
| F003     | Overtemperature Fault      | Temperature exceeds configured overtemperature threshold                               | High     | Enter DEGRADED                        | Reported in fault status message and snapshot |
| F004     | Pump command no effect     | Actuator command ON but expected thermal effect not observed within validation windows | Medium   | Raise fault                           | Reported in fault status message and snapshot |
| F005     | Task supervision fault     | Critical task heatbeat missing for more than 400 ms                                    | High     | Degraded and inhibit watchdog refresh | Reported in fault status message and snapshot |
| F006     | Invalid diagnostic command | Unsupported or malformed CAN diagnostic command received                               | Low      | No actuator change                    | Visible through diagnostic counter            |
