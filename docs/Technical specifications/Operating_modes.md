# Operating Modes

## Mode definitions

| Mode          | Description                                                                                                                                                                          | Entry                                                                                  | Exit                                                                      | Expected beahvior                                                                                                                                                                                         |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| INIT          | System startup phase. The reset cause is identified and stored before entering operational mode.                                                                                     | - Power-up<br>- Reset event                                                            | Initialization completed successfully                                     | - No normal thermal control yet<br>- Diagnostic communication may already start<br>- Internal service and task supervision are initialized                                                                |
| NORMAL        | Nominal operating mode. The system periodically acquires temperature data, applies pump control logic, supervises internal task health and transmits status and diagnostic over CAN. | - Successful completion of INIT<br>- Reset event                                       | - Critical fault detected <br>- Reset event                               | - Temperature acquisition active<br>- Pump control active<br>- CAN heartbeat/status active<br>- Fault monitoring active<br>- Watchdog refresh allowed only *(if supervision conditions are satisfied)*    |
| DEGRADED      | Restricted operating mode entered after a critical fault or a fault requiring safe beahvior. the system keeps diagnostic communication active while applying a safe actuator policy  | - Critical fault raised<br>- Fault policy requests degraded operation                  | - Reset event<br>- Explicit recovery condition *(if supported by policy)* | - Diagnostic communication remains active<br>- Actuator behavior is restricted or forced to safe state<br>- Fault status remains available<br>- System supervision remains active *(as long as possible)* |
| FAULT_LATCHED | Latched fault mode used when system must block return to nominal beahvior until explicit recovery or reset. This mode is intended for severe or persistent faults.                   | - Latched critical fault raised<br>- Fault policy requires manual or explicit recovery | - Reset event<br>- Explicit clear/recovery command                        | - Normal control is inhibited<br>- Safe actuator state is maintained<br>- Diagnostic communication remains active if possible<br>- Active latched fault is reported over CAN                              |

## Transition logic

- INIT -> Normal *if initialization succeeds*

- NORMAL -> DEGRADED *if a critical recoverable fault is detected*

- NORMAL -> FAULT_LATCHED *if a latched critical fault is detected*

- DEGRADED -> NORMAL *only if fault disappears and recovery policy allows it*

- DEGRADED -> FAULT_LATCHED *if fault severity increases or recovery fails*

- Any mode -> INIT *after reset
