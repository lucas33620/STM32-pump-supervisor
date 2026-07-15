# Operating Modes

## Mode definitions

| Mode          | Description                                                                                                                                                                          | Entry                                                                                  | Exit                                                                      | Expected beahvior                                                                                                                                                                                         |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| INIT          | System startup phase. The reset cause is identified and stored before entering operational mode.                                                                                     | - Power-up<br>- Reset event                                                            | Initialization completed successfully                                     | - No normal thermal control yet<br>- Diagnostic communication may already start<br>- Internal service and task supervision are initialized                                                                |
| NORMAL        | Nominal operating mode. The system periodically acquires temperature data, applies pump control logic, supervises internal task health and transmits status and diagnostic over CAN. | - Successful completion of INIT<br>- Reset event                                       | - Critical fault detected <br>- Reset event                               | - Temperature acquisition active<br>- Pump control active<br>- CAN heartbeat/status active<br>- Fault monitoring active<br>- Watchdog refresh allowed only *(if supervision conditions are satisfied)*    |
| DEGRADED      | Restricted operating mode entered after a critical fault or a fault requiring safe beahvior. the system keeps diagnostic communication active while applying a safe actuator policy  | - Critical fault raised<br>- Fault policy requests degraded operation                  | - Reset event<br>- Explicit recovery condition *(if supported by policy)* | - Diagnostic communication remains active<br>- Actuator behavior is restricted or forced to safe state<br>- Fault status remains available<br>- System supervision remains active *(as long as possible)* |
| FAULT_LATCHED | Latched fault mode used when system must block return to nominal beahvior until explicit recovery or reset. This mode is intended for severe or persistent faults.                   | - Latched critical fault raised<br>- Fault policy requires manual or explicit recovery | - Reset event<br>- Explicit clear/recovery command                        | - Normal control is inhibited<br>- Safe actuator state is maintained<br>- Diagnostic communication remains active if possible<br>- Active latched fault is reported over CAN                              |

## Transition logic

| Source        | Event / Condition               | Destination   | Action                |
| ------------- | ------------------------------- | ------------- | --------------------- |
| INIT          | Initialization successful       | NORMAL        | publish boot OK trace |
| INIT          | Critical initialization failure | FAULT_LATCHED | store critical fault  |
| NORMAL        | Recoverable fault confirmed     | DEGRADED      | limit operation       |
| NORMAL        | Critical fault confirmed        | FAULT_LATCHED | stop pump command     |
| DEGRADED      | Nominal recovery condition met  | NORMAL        | clear degraded state  |
| DEGRADED      | Critical fault confirmed        | FAULT_LATCHED | enter safe state      |
| FAULT_LATCHED | User reset / clear authorized   | INIT          | reinitialize system   |

## Module interactions

| Module              | Relation with `operating_modes`                | Purpose                                                                                    |
| ------------------- | ---------------------------------------------- | ------------------------------------------------------------------------------------------ |
| thermal_supervision | provides status / fault conditions             | Supplies recoverable or critical conditions used for mode transitions                      |
| pump_control_logic  | consumes current system mode                   | Adapts pump command behavior depending on `INIT`, `NORMAL`, `DEGRADED`, or `FAULT_LATCHED` |
| fault_manager       | provides fault severity and active fault state | Supports transition decisions toward `DEGRADED` or `FAULT_LATCHED`                         |
| diag_manager        | consumes current mode                          | Reports current operating mode and transition-related status through diagnostics           |
| watchdog_manager    | may provide supervision fault status           | Can contribute to critical transition decisions if supervision fails                       |

## Design rules

- `operating_modes` owns the current system mode
- Only `operating_modes` is allowed to change the current mode
- Other application modules can request or justify a transition, but do not switch modes directly
- The current mode is exposed through a dedicated interface for application and diagnostic use
