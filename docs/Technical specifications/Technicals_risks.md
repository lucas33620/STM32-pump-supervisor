# Technical Risks

| ID    | Risk                                                                          | Impact | Probability | Mitigation                                                               |
| ----- | ----------------------------------------------------------------------------- | ------ | ----------- | ------------------------------------------------------------------------ |
| R-001 | Hardware integration issues may delay bring-up.                               | High   | Medium      | Use simple components and validate each interface early.                 |
| R-002 | CAN integration with DiagPack CAN may require additional debugging effort.    | High   | Medium      | Freeze a minimal CAN protocol early and test it in phase 1.              |
| R-003 | RTOS and supervision logic may become too complex for the available schedule. | Medium | Medium      | Keep the task architecture small and incremental.                        |
| R-004 | Scope growth may prevent project completion within one month.                 | High   | High        | Limit the project to the defined subsystem and selected fault scenarios. |

## Technical Constraints

- The project shall be completed in approximately one month.
- The system shall remain limited to one STM32 target, one primary sensor, one actuator, and one CAN interface.
- The CAN interface shall remain compatible with DiagPack CAN.
- The implementation shall prioritize diagnostic observability and robustness over feature richness.
