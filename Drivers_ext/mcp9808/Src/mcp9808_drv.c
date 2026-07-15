/**
 * @file    mcp9808_drv.c
 * @brief   Sensor management module for MCP9808.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

/** @section Include */
#include "stm32f4xx_hal.h"
#include "mcp9808_drv.h"
#include "i2c.h"

/** @section Define*/
#define MCP9808_INVALID_TEMP_X10 (INT16_MIN)

/** @section Static Functions */
static Mcp9808Status mcp9808_drv_is_adress_valid(uint8_t addr_7bit);
static Mcp9808Status mcp9808_drv_convert_raw_to_tempx10(const Mcp9808Ctx *ctx, int16_t *out_temp_x10);
static Mcp9808Status mcp9808_drv_read_ambient_temp_raw(const Mcp9808Ctx *ctx);
static Mcp9808Status mcp9808_drv_is_address_valid(uint8_t addr_7bit);
static void mcp9808_drv_update_fault_on_failure(Mcp9808Ctx *ctx, Mcp9808Status error_status);

/**
 * @brief  Validates the given 7-bit I2C address for the MCP9808 sensor.
 * 
 * @param addr_7bit The 7-bit I2C address to validate. 
 * 
 * @return MCP9808_STATUS_OK if conversion succeeded, MCP9808_STATUS_ERR_PARAM if pointer is NULL...
 */
static Mcp9808Status mcp9808_drv_is_address_valid(uint8_t addr_7bit)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;
    
    if (addr_7bit < MCP9808_ADDR_MIN || addr_7bit > MCP9808_ADDR_MAX)
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    return mcp9808_status;
}

/**
 * @brief  Converts raw I2C data from the Ambient Temperature (Ta) register to Celsius x10.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * @param len Length of the rx buffer (expected to be 2).
 * @param out_temp_x10 Pointer to the output temperature value in Celsius x10.
 * 
 * @return MCP9808_STATUS_OK if conversion succeeded, MCP9808_STATUS_ERR_PARAM if pointer is NULL.
 */
static Mcp9808Status mcp9808_drv_convert_raw_to_tempx10(const Mcp9808Ctx *ctx, int16_t *out_temp_x10)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;

    if((ctx == NULL) || (out_temp_x10 == NULL))
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else
    {
        /*
        *   TA: AMBIENT TEMPERATURE REGISTER p.24 datasheet
        *   [15:13] : Flag = TCRIT; TUPPER; TLOWER
        *   [12:0] :  SIGN + TA as signed 13-bit two's complement, Q4 (LSB = 2^-4 °C)
        */
        const uint16_t raw  = (uint16_t)(((uint16_t)ctx->rx_buf[0] << 8) | (uint16_t)ctx->rx_buf[1]);
        const uint16_t ta13 = (uint16_t)(raw & 0x1FFFU); /* Ignore alert flags bits 15..13 (keep sign + temperature bits) */
        
        /* Converted TA p.25 datasheet 
        * Sign-extend 13-bit two's complement into int32 (Q4 units)
        */
        int32_t q4;
        if((ta13 & 0x1000U) != 0U)  /* sign bit TA[12]*/
        {   
            q4 = (int32_t)ta13 - 0x2000; /* subtract 8192 -> sign-extend 13-bit */
        }
        else
        {
            q4 = (int32_t)ta13;
        }

        /* Convert Q4 to x10°C:
        temp(°C) = q4 / 16
        temp_x10 = (q4 * 10) / 16
        Use symmetric rounding (important for negatives)
        */
        int32_t temp_x10;
        if (q4 >= 0)
        {
            temp_x10 = (q4 * 10 + 8) / 16;
        }
        else
        {
            temp_x10 = (q4 * 10 - 8) / 16;
        }

        /* Defensive clamp to int16_t */
        if (temp_x10 > (int32_t)INT16_MAX) 
        { 
            temp_x10 = (int32_t)INT16_MAX; 
        }
        if (temp_x10 < (int32_t)INT16_MIN) 
        { 
            temp_x10 = (int32_t)INT16_MIN; 
        }

        if ((temp_x10 < MCP9808_VALID_TEMP_MIN_X10) || (temp_x10 > MCP9808_VALID_TEMP_MAX_X10))
        {
            mcp9808_status = MCP9808_STATUS_ERR_INVALID_TEMP;
        }
        else
        {
            *out_temp_x10 = (int16_t)temp_x10;
            mcp9808_status = MCP9808_STATUS_OK;
        }
    }

    return mcp9808_status;
}

/**
 * @brief  Reads raw ambient temperature data from the MCP9808 sensor via I2C and stores it in the context's rx buffer.
 * 
 * @param ctx Pointer to the MCP9808 context structure.
 * 
 * @return MCP9808_STATUS_OK if conversion succeeded, MCP9808_STATUS_ERR_PARAM if pointer is NULL...
 */
static Mcp9808Status mcp9808_drv_read_ambient_temp_raw(const Mcp9808Ctx *ctx)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;
    HAL_StatusTypeDef hal_status;

    if (ctx == NULL)
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else
    {
        /* Read ambient temperature register 
        *  Warning : this function may block for a while if the sensor is not responding, due to HAL timeouts. */
        hal_status = HAL_I2C_Mem_Read(&hi2c1, (ctx->i2c_addr_7bit << 1), (uint16_t)MCP9808_REG_AMBIENT_TEMP, I2C_MEMADD_SIZE_8BIT, ctx->rx_buf, (uint16_t)MCP9808_TEMP_BYTES, (uint32_t)MCP9808_I2C_TIMEOUT_MS);
        if (hal_status == HAL_OK)
        {
            mcp9808_status = MCP9808_STATUS_OK;
        }
        else if (hal_status == HAL_TIMEOUT)
        {
            mcp9808_status = MCP9808_STATUS_ERR_TIMEOUT;
        }
        else
        {
            mcp9808_status = MCP9808_STATUS_ERR_I2C;
        }
    }

    return mcp9808_status;
}

static void mcp9808_drv_update_fault_on_failure(Mcp9808Ctx *ctx, Mcp9808Status error_status)
{
    ctx->last_temp_x10 = MCP9808_INVALID_TEMP_X10;
    ctx->last_error = error_status;

    if (ctx->consecutive_fail_count < UINT8_MAX)
    {
        ctx->consecutive_fail_count++;
    }

    if (ctx->consecutive_fail_count >= MCP9808_MAX_CONSEC_FAIL )
    {
        if (ctx->state != MCP9808_STATE_FAULT)
        {
            if (ctx->fault_count < UINT16_MAX)
            {
                ctx->fault_count++;
            }
        }
        
        ctx->state = MCP9808_STATE_FAULT;
        ctx->active_error = error_status;
    }
    else
    {
        ctx->state = MCP9808_STATE_READY;
    }
}

/** @section Global Functions */
/**
 * @brief Initialize the driver context with the given I2C address.
 * 
 * @note This function does not perform any I2C communication. It only initializes the context structure.
 */
Mcp9808Status mcp9808_drv_init(Mcp9808Ctx *ctx, uint8_t addr_7bit)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;

    if (ctx == NULL)
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
        /* Cannot initialize ctx */
    }
    else if (mcp9808_drv_is_address_valid(addr_7bit) != MCP9808_STATUS_OK)
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
        ctx->state = MCP9808_STATE_UNINIT; /* Set state to uninitialized */
    }
    else
    {
        ctx->i2c_addr_7bit = addr_7bit;
        ctx->state = MCP9808_STATE_READY;
        ctx->last_temp_x10 = MCP9808_INVALID_TEMP_X10;
        ctx->fault_count = 0U;
        ctx->consecutive_fail_count = 0U;
        ctx->active_error = MCP9808_STATUS_OK;
        ctx->last_error = MCP9808_STATUS_OK;

        ctx->rx_buf[0] = 0U;
        ctx->rx_buf[1] = 0U;

        mcp9808_status = MCP9808_STATUS_OK;
    }

    return mcp9808_status;
}

/** @brief Gets the temperature from the MCP9808 sensor and stores it in the context's temperature buffer.
 */
Mcp9808Status mcp9808_drv_get_temperature_x10(Mcp9808Ctx *ctx, int16_t *out_temp_x10)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;
    int16_t temp_x10;
    temp_x10 = 0;

    if ((ctx == NULL) || (out_temp_x10 == NULL))
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else if (ctx->state == MCP9808_STATE_UNINIT)
    {
        mcp9808_status = MCP9808_STATUS_ERR_INIT;
    }
    /* If 3 consecutive failures => Fault */
    else if (ctx->state == MCP9808_STATE_FAULT)
    {
        mcp9808_status = MCP9808_STATUS_ERR_FAULT;
    }
    else
    {
        /* Read ambient temperature raw data */
        mcp9808_status = mcp9808_drv_read_ambient_temp_raw(ctx);

        if (mcp9808_status == MCP9808_STATUS_OK)
        {
            /* Convert raw temperature data to x10 format */
            mcp9808_status = mcp9808_drv_convert_raw_to_tempx10(ctx, &temp_x10);

            if (mcp9808_status == MCP9808_STATUS_OK)
            {
                ctx->last_temp_x10 = temp_x10;
                *out_temp_x10 = temp_x10;

                ctx->consecutive_fail_count = 0U;
                ctx->active_error = MCP9808_STATUS_OK;
                ctx->state = MCP9808_STATE_READY;
            }
            else
            {
                mcp9808_drv_update_fault_on_failure(ctx, mcp9808_status);
                
            }
        }
        else
        {
            mcp9808_drv_update_fault_on_failure(ctx, mcp9808_status);
        }
    }
    
    return mcp9808_status;
}

/** @brief Gets the state of the MCP9808 sensor.
 */
Mcp9808Status mcp9808_drv_get_state(const Mcp9808Ctx *ctx, Mcp9808State *out_state)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;

    if ((ctx == NULL) || (out_state == NULL))
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else if (ctx->state == MCP9808_STATE_UNINIT)
    {
        mcp9808_status = MCP9808_STATUS_ERR_INIT;
    }
    else
    {
        *out_state = ctx->state;
        mcp9808_status = MCP9808_STATUS_OK;
    }

    return mcp9808_status;
}

/** @brief Gets the fault count of the MCP9808 sensor.
 */
Mcp9808Status mcp9808_drv_get_fault_count(const Mcp9808Ctx *ctx, uint16_t *out_fault_count)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;

    if ((ctx == NULL) || (out_fault_count == NULL))
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else if (ctx->state == MCP9808_STATE_UNINIT)
    {
        mcp9808_status = MCP9808_STATUS_ERR_INIT;
    }
    else
    {
        *out_fault_count = ctx->fault_count;
        mcp9808_status = MCP9808_STATUS_OK;
    }
    return mcp9808_status;
}

/** @brief Gets the active error status of the MCP9808 sensor.
 */
Mcp9808Status mcp9808_drv_get_active_error(const Mcp9808Ctx *ctx, Mcp9808Status *out_active_error)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;

    if ((ctx == NULL) || (out_active_error == NULL))
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else if (ctx->state == MCP9808_STATE_UNINIT)
    {
        mcp9808_status = MCP9808_STATUS_ERR_INIT;
    }
    else
    {
        *out_active_error = ctx->active_error;
        mcp9808_status = MCP9808_STATUS_OK;
    }
    return mcp9808_status;
}

/** @brief Gets the last error status of the MCP9808 sensor.
 */
Mcp9808Status mcp9808_drv_get_last_error(const Mcp9808Ctx *ctx, Mcp9808Status *out_last_error)
{
    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;

    if ((ctx == NULL) || (out_last_error == NULL))
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else if (ctx->state == MCP9808_STATE_UNINIT)
    {
        mcp9808_status = MCP9808_STATUS_ERR_INIT;
    }
    else
    {
        *out_last_error = ctx->last_error;
        mcp9808_status = MCP9808_STATUS_OK;
    }
    return mcp9808_status;
}

/** @brief Attempts to recover the MCP9808 sensor from a fault state by reinitializing it.
 */
Mcp9808Status mcp9808_drv_recover(Mcp9808Ctx *ctx)
{

    Mcp9808Status mcp9808_status = MCP9808_STATUS_OK;
    HAL_StatusTypeDef hal_status;

    if (ctx == NULL)
    {
        mcp9808_status = MCP9808_STATUS_ERR_PARAM;
    }
    else
    {
        hal_status = HAL_I2C_DeInit(&hi2c1);
        if (hal_status != HAL_OK)
        {
            mcp9808_status = MCP9808_STATUS_ERR_I2C;
        }
        else
        {
            hal_status = HAL_I2C_Init(&hi2c1);
            if (hal_status != HAL_OK)
            {
                mcp9808_status = MCP9808_STATUS_ERR_I2C;
            }
            else
            {
                /* Ping the sensor to check if it's responsive after I2C reinit */
                hal_status = HAL_I2C_IsDeviceReady(&hi2c1, (ctx->i2c_addr_7bit << 1), 3U, (uint32_t)MCP9808_I2C_TIMEOUT_MS);
                if (hal_status != HAL_OK)
                {
                    mcp9808_status = MCP9808_STATUS_ERR_I2C;
                }
                else
                {
                    ctx->consecutive_fail_count = 0U;
                    ctx->state = MCP9808_STATE_READY;

                    /* Note : only valid temperature readings allow to proceed active error = MCP9808_STATUS_OK */
                }
            }
        }
    }

    return mcp9808_status;
}
