# Naming Conventions

## Folders

Use `snake_case`.

Examples:
- `App/control_logic`
- `Services/fault`
- `Drivers/mcp9808/mcp9008_drv.c`
- `BSP/mcp9808/mcp9808_bsp.c`

## Files

Use `snake_case`.

Examples:
- `pump_control_logic.c`
- `fault_manager.h`
- `mcp9808_drv.c`
- `bsp_uart.c`
- `task_supervisor.c`

## Functions

Use `snake_case` with a module prefix.

Examples:
- `pump_control_logic_update()`
- `fault_manager_raise()`
- `mcp9808_drv_read_temperature()`
- `bsp_can_send()`
- `task_supervisor_entry()`

## Types

Use `PascalCase`.

Examples:
- `SystemMode`
- `FaultId`
- `PumpState`

## Enums

Use uppercase with a module prefix.

Examples:
- `SYSTEM_MODE_INIT`
- `FAULT_ID_SENSOR_TIMEOUT`
- `PUMP_STATE_ON`

## General Rules

- Keep names short and explicit
- Do not mix naming styles for the same category
- Use one naming rule consistently across the whole project