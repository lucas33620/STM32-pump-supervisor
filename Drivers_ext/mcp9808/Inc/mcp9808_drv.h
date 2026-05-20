/**
 * @file    mcp9808_drv.h
 * @brief   Sensor management module for MCP9808.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef MCP9808_DRV_H
#define MCP9808_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include "mcp9808_cfg.h"

#define MCP9808_VALID_TEMP_MIN_X10 (-400) /* -40.0°C */
#define MCP9808_VALID_TEMP_MAX_X10 (1250) /* 125.0°C */
#define MCP9808_ADDR_MIN          (0x18U)
#define MCP9808_ADDR_MAX          (0x1FU)
#define MCP9808_REG_AMBIENT_TEMP  (0x05U)
#define MCP9808_TEMP_BYTES       (2U)

typedef enum
{
    MCP9808_STATUS_OK = 0,
    MCP9808_STATUS_ERR_INIT,
    MCP9808_STATUS_ERR_FAULT,
    MCP9808_STATUS_ERR_PARAM,
    MCP9808_STATUS_ERR_I2C,
    MCP9808_STATUS_ERR_TIMEOUT,
    MCP9808_STATUS_ERR_INVALID_TEMP
} Mcp9808Status;

typedef enum
{
    MCP9808_STATE_UNINIT = 0,
    MCP9808_STATE_READY,
    MCP9808_STATE_FAULT
} Mcp9808State;

typedef struct
{
    uint8_t i2c_addr_7bit;
    uint8_t rx_buf[MCP9808_TEMP_BYTES];
    int16_t last_temp_x10;

    uint16_t fault_count;
    uint8_t consecutive_fail_count;

    /* Diagnosis */
    Mcp9808State state;
    Mcp9808Status active_error;
    Mcp9808Status last_error;

} Mcp9808Ctx;

/** @brief Initializes the MCP9808 driver context.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * @param addr_7bit I2C address of the sensor (7-bit).
 * 
 * @return MCP9808_STATUS_OK if initialization succeeded, MCP9808_STATUS_ERR_PARAM if a parameter is invalid...
 */
Mcp9808Status mcp9808_drv_init(Mcp9808Ctx *ctx, uint8_t addr_7bit);

/** @brief Gets the temperature from the MCP9808 sensor and stores it in the context's temperature buffer.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * @param out_temp_x10 Pointer to a variable where the temperature will be stored (in x10 format).
 * 
 * @return MCP9808_STATUS_OK if the temperature was read successfully, MCP9808_STATUS_ERR_PARAM if a parameter is invalid...
 */
Mcp9808Status mcp9808_drv_get_temperature_x10(Mcp9808Ctx *ctx, int16_t *out_temp_x10);

/** @brief Gets the state of the MCP9808 sensor.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * @param out_state Pointer to a variable where the state will be stored.
 * 
 * @return MCP9808_STATUS_OK if the state was read successfully, MCP9808_STATUS_ERR_PARAM if a parameter is invalid...
 */
Mcp9808Status mcp9808_drv_get_state(const Mcp9808Ctx *ctx, Mcp9808State *out_state);

/** @brief Gets the fault count of the MCP9808 sensor.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * @param out_fault_count Pointer to a variable where the fault count will be stored.
 * 
 * @return MCP9808_STATUS_OK if the fault count was read successfully, MCP9808_STATUS_ERR_PARAM if a parameter is invalid...
 */
Mcp9808Status mcp9808_drv_get_fault_count(const Mcp9808Ctx *ctx, uint16_t *out_fault_count);

/** @brief Gets the active error status of the MCP9808 sensor.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * @param out_active_error Pointer to a variable where the active error status will be stored.
 * 
 * @return MCP9808_STATUS_OK if the active error status was read successfully, MCP9808_STATUS_ERR_PARAM if a parameter is invalid...
 */
Mcp9808Status mcp9808_drv_get_active_error(Mcp9808Ctx *ctx, Mcp9808Status *out_active_error);

/** @brief Gets the last error status of the MCP9808 sensor.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * @param out_last_error Pointer to a variable where the last error status will be stored.
 * 
 * @return MCP9808_STATUS_OK if the last error status was read successfully, MCP9808_STATUS_ERR_PARAM if a parameter is invalid...
 */
Mcp9808Status mcp9808_drv_get_last_error(Mcp9808Ctx *ctx, Mcp9808Status *out_last_error);

/** @brief Attempts to recover the MCP9808 sensor from a fault state by reinitializing it.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * 
 * @return MCP9808_STATUS_OK if recovery succeeded, MCP9808_STATUS_ERR_PARAM if a parameter is invalid...
 */
Mcp9808Status mcp9808_drv_recover(Mcp9808Ctx *ctx);

#endif