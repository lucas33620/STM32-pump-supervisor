# STM32 Pump Supervisor instructions

This repository contains embedded C firmware for an STM32F439ZI using
FreeRTOS, STM32 HAL, CAN, I2C and UART.

## Collaboration policy

- Review code only.
- Do not create commits, branches, patches or pull requests.
- Do not modify repository files.
- The repository owner implements all changes.

## Architecture

- App contains product behavior and control logic.
- Services contain cross-cutting managers.
- Drivers contain component-specific logic.
- BSP isolates STM32 HAL access.
- RTOS tasks orchestrate modules but must not contain core business logic.
- App and control logic must not depend directly on STM32 HAL.
- Lower layers must not depend on upper layers.

## Coding conventions

- Folders and files use snake_case.
- Functions use snake_case with a module prefix.
- Types use PascalCase.
- Enum values use uppercase with a module prefix.
- Do not use bool; use uint8_t or explicit enums.

## Code review rules

Focus on actionable embedded firmware defects:

- unsynchronized shared data between RTOS tasks;
- incoherent reads of multi-field structures;
- blocking HAL calls inside critical sections or mutexes;
- deadlocks, starvation and priority inversion;
- incorrect task periods or timeout assumptions;
- unsafe ISR and callback behavior;
- large local variables and task stack pressure;
- unchecked HAL return values;
- direct HAL dependencies outside BSP or approved drivers;
- incorrect CAN, I2C or UART error handling;
- buffer overflows, truncation, sign conversions and unsafe casts;
- modifications outside CubeMX USER CODE sections;
- sensor failures that block unrelated functionality.

For each finding:

- provide severity;
- identify the exact file and line;
- explain the runtime consequence;
- propose focused corrective guidance;
- do not implement the correction.