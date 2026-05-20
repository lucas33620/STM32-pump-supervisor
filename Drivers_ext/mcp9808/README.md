# MCP9808 Driver — Fault Handling & Recovery Policy

## Purpose

This module reads temperature from the MCP9808 sensor and applies a simple hardening policy to avoid reacting to isolated I2C glitches.

## Fault handling policy

- A single read failure does not immediately put the driver into fault state.
- The driver enters `FAULT` only after 3 consecutive read failures.
- `active_error` represents the current confirmed fault.
- `last_error` keeps the last observed error in sticky form, even after recovery.
- Recovery is not automatic.
- Recovery must be explicitly requested by the application through `mcp9808_drv_recover()`.
- After a successful recovery, the driver returns to `READY`.
- `active_error` is cleared only after a successful temperature read.

## Driver states

| State | Meaning | Action |
|---|---|---|
| `MCP9808_STATE_UNINIT` | Driver not initialized | Call `mcp9808_drv_init()` |
| `MCP9808_STATE_READY` | Driver ready for normal reads | Normal operation |
| `MCP9808_STATE_FAULT` | Confirmed fault after repeated failures | Application may request recovery |

## Error codes

| Code | Meaning | Priority | Recommended action |
|---|---|---:|---|
| `MCP9808_STATUS_OK` | No error | Low | None |
| `MCP9808_STATUS_ERR_PARAM` | Invalid parameter passed to API | High | Fix caller code |
| `MCP9808_STATUS_ERR_INIT` | Driver not initialized | High | Initialize driver before use |
| `MCP9808_STATUS_ERR_I2C` | I2C communication error | Medium | Check wiring, power, pull-ups, sensor presence |
| `MCP9808_STATUS_ERR_TIMEOUT` | I2C timeout detected | Medium | Check bus lock, sensor response, cabling |
| `MCP9808_STATUS_ERR_INVALID_TEMP` | Temperature value outside allowed application range | Medium | Check sensor data validity and environment |
| `MCP9808_STATUS_ERR_FAULT` | Driver is in confirmed fault state | High | Call `mcp9808_drv_recover()` if recovery is allowed |

## Support guide

| Situation observed | Likely cause | Priority | What to do |
|---|---|---:|---|
| One isolated read failure | Temporary bus glitch | Low | Monitor only |
| Repeated read failures | Sensor missing, bus issue, power issue | High | Inspect hardware, then request recovery |
| Driver in `FAULT` | 3 consecutive failures confirmed | High | Call `mcp9808_drv_recover()` |
| Recovery fails | Sensor still absent or bus still broken | Critical | Check hardware before retry |
| Recovery succeeds but `last_error` remains set | Expected behavior | Low | Keep as diagnostic history |
| Invalid temperature reported | Sensor data not plausible for application | Medium | Check raw data and sensor conditions |

## Expected application behavior

- Poll temperature normally when state is `READY`
- Do not treat a single transient read failure as a confirmed fault
- If state becomes `FAULT`, stop relying on sensor value
- Decide at application level whether recovery is allowed
- Log `active_error`, `last_error`, and `fault_count` for support/debug