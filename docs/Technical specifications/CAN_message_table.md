# CAN message table

## Sumarize

- 4 message send periodically

- 2 command/respond message

- Only one simple format with 8 octets maximum

## Assumptions

- Standart CAN ID : 11 bits

- DLC : 8 bytes max

- Endianness: little-endian for multi-byte numeric values

- Temperature unit: 0.1°C signed integer

- Timing values and counters are unsigned unless otherwise stated

## Message overview

| CAN ID | Name           | Direction | Period / Trigger                                   | Purpose                                          |
| ------ | -------------- | --------- | -------------------------------------------------- | ------------------------------------------------ |
| 0x180  | HEATBEAT       | TX        | Every 500 ms                                       | Reports global system liveness                   |
| 0x181  | THERMAL_STATUS | TX        | Every 500 ms                                       | Reports temperature and actuator status          |
| 0x182  | FAULT_STATUS   | TX        | Every 500 ms when fault active and on fault change | Reports active fault summary                     |
| 0x183  | SYSTEM_METRIC  | TX        | Every 1s                                           | Reports supervision and runtime health data      |
| 0x280  | DIAG_COMMAND   | RX        | On request                                         | Receives diagnostic and fault injection commands |
| 0x281  | DIAG_RESPONSE  | Tx        | On request / event                                 | Returns command result or snapshot               |

## HEARTBEAT (0x180)

### Purpose

Reports system liveness, current operating mode, and reset-related information.

| Byte | Signal              | Type  | Description                                                                                                          |
| ---- | ------------------- | ----- | -------------------------------------------------------------------------------------------------------------------- |
| 0    | Alive counter       | uint8 | Incrementing heartbeat counter. Wrapping from 255 to 0                                                               |
| 1    | System mode         | uint8 | - 0 = INIT<br/>- 1 = NORMAL<br/>- 2 = DEGRADED<br/>- 3 = FAULT_LATCHED                                               |
| 2    | Reset cause         | uint8 | - 0 = unknow<br/>- 1 = Power-on <br/>- 2 = Software reset<br/>- 3 = Watchdog<br/>- 4 = Brown-out<br/>- 5 = Pin reset |
| 3    | Global status flags | uint8 | Bit field (see below)                                                                                                |
| 4    | Uptime LSB          | uint8 | Uptime in seconds                                                                                                    |
| 5    | Uptime MSB          | uint8 | Uptime in seconds                                                                                                    |
| 6    | Reserved            | uint8 | Set to 0                                                                                                             |
| 7    | Reserved            | uint8 | Set to 0                                                                                                             |

### Global status flags (byte 3)

- bit 0 : Sensor valid

- bit 1 : Actuator ON

- bit 2 : Fault active

- bit 3: watchdog refresh allowed

- bit 4...7 : reserved

## THERMAL_STATUS (0x181)

### Purpose

Reports the current measured temperature and thermal control state.

| Byte | Signal                     | Type  | Description                                                                                          |
| ---- | -------------------------- | ----- | ---------------------------------------------------------------------------------------------------- |
| 0    | Temperature LSB            | int16 | Current temperature in 0.1°C                                                                         |
| 1    | Temperature MSB            | int16 | Current temperature in 0.1°C                                                                         |
| 2    | Pump Command               | uint8 | - 0=OFF<br/>- 1=ON                                                                                   |
| 3    | Thermal State              | uint8 | - 0=Below ON threshold<br/>- 1=Between thresholds<br/>- 2=Above ON threshold<br/>- 3=Overtemperature |
| 4    | Activation Threshold LSB   | int16 | Pump ON threshold in 0.1°C                                                                           |
| 5    | Activation Threshold MSB   | int16 | Pump ON threshold in 0.1°C                                                                           |
| 6    | Deactivation Threshold LSB | int16 | Pump OFF threshold in 0.1°C                                                                          |
| 7    | Deactivation Threshold MSB | int16 | Pump OFF threshold in 0.1°C                                                                          |

### Note :

- Pump ON threshold = 40.0°C -> 400

- Pump OFF threshold = 35.0°C -> 350

## FAULT_STATUS (0x182)

### Purpose

Reports the currently active fault summary.

| Byte | Signal          | Type  | Description                                      |
| ---- | --------------- | ----- | ------------------------------------------------ |
| 0    | Active fault id | uint8 | 0 if no active fault else fault code (see below) |
| 1    | Fault state     | uint8 | - 0 = Inactive<br/>- 1= Active<br/>- Latched     |
| 2    | Fault Counter   | uint8 | Number of occurrences, saturating counter        |
| 3    | Fault Flag      | uint8 | Bit field (see below)                            |
| 4    | Reserved        | uint8 | Set to 0                                         |
| 5    | Reserved        | uint8 | Set to 0                                         |
| 6    | Reserved        | uint8 | Set to 0                                         |
| 7    | Reserved        | uint8 | Set to 0                                         |

### Fault ID mapping (byte 0)

- 0 = No active fault

- 1 = F001 sensor communication fault

- 2 = F002 Temperature out of range

- 3 = F003 Overtemperature fault

- 4 = F004 Pump command no effect

- 5 = F005 Task supervision fault

- 6 = F006 Invalid diagnostic command

### Fault flags (byte 4)

- bit 0 : Fault present

- bit 1 : Degraded reaction applied

- bit 2 : Actuator forced safe

- bit3 : Watchdog inhibited

- bit4..7: reserved

## SYSTEM_METRICS (0x183)

### Purpose

Reports lightweight runtime health and supervision metrics.

| Byte | Signal | Type | Description |
| ---- | ------ | ---- | ----------- |
|      |        |      |             |
|      |        |      |             |
|      |        |      |             |
|      |        |      |             |
|      |        |      |             |

## DIAG_COMMAND (0x280)

### Purpose

Receives diagnostic requests, fault injection commands, and service actions from DiagPack CAN for exemple or bench test.

| Byte | Signal      | Type  | Description      |
| ---- | ----------- | ----- | ---------------- |
| 0    | Command ID  | uint8 | Request action   |
| 1    | Parameter 1 | uint8 | Command-specific |
| 2    | Parameter 2 | uint8 | Command-specific |
| 3    | Parameter 3 | uint8 | Command-specific |
| 4    | Parameter 4 | uint8 | Command-specific |
| 5    | Parameter 5 | uint8 | Command-specific |
| 6    | Parameter 6 | uint8 | Command-specific |
| 7    | Parameter 7 | uint8 | Command-specific |

### Command

- 0x01 : Clear clearable fault

- 0x02 : Request diagnostic snapshot 

- 0x10 : Inject sensor communication fault (F001)

- 0x11 : Inject temperature out of range (F002)

- 0x12 : Inject overtemperature (F003)

- 0x13 : Inject pump command no effect (F004)

- 0x14 : Inject task supervision fault (F005)

- 0x15 : Inject Task supervision fault (F006)

- 0x16 : Inject invalid diagnostic command (F007)

- 0x20 = Clear injected faults / return to nominal test condition

### Rules

- Valid command shall take 200 ms to respond when applicable

- Invalid frame shall increment the invalide command counter (F007)

## DIAG_RESPONSE (0x281)

### Purpose

Returns immediate command acknowledgment or a compact diagnostic snapshot.

| Byte | Signal             | Type  | Description                                                            |
| ---- | ------------------ | ----- | ---------------------------------------------------------------------- |
| 0    | Response Type      | uint8 | - 0=ACK<br/>- 1=NACK<br/>- 2=SNAPSHOT                                  |
| 1    | Related Command ID | uint8 | Echo of command ID if applicable                                       |
| 2    | Result Code        | uint8 | - 0=OK<br/>- 1=Unsupported<br/>- 2=Invalid Parameter<br/>- 3=Busy      |
| 3    | System Mode        | uint8 | - 0 = INIT<br/>- 1 = NORMAL<br/>- 2 = DEGRADED<br/>- 3 = FAULT_LATCHED |
| 4    | Active Fault ID    | uint8 | Current active fault                                                   |
| 5    | Pump Command       | uint8 | - 0=OFF<br/>- 1=ON                                                     |
| 6    | Temperature LSB    | int16 | Current temperature in 0.1°C                                           |
| 7    | Temperature MSB    | int16 | Current temperature in 0.1°C                                           |


