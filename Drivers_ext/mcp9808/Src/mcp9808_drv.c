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
#define MCP9808_I2C_ADDR_HAL     (SENSOR_MCP9808_ADDR_MIN << 1)

/** @section Typedef */

/** @section Static Functions */

/** @section Global Functions */
/**
 * @brief ping I2C of component  	  
 *
 * @param  
 */
uint8_t mcp9808_drv_is_detected(void)
{
    HAL_StatusTypeDef hal_status;
    uint8_t is_detected = 0;

    /* Ensure that sensor is ready on I2C bus */
    hal_status = HAL_I2C_IsDeviceReady(&hi2c1, MCP9808_I2C_ADDR_HAL, 3U, 100U);

    if (hal_status == HAL_OK)
    {
        is_detected = 1;
    }
    else
    {
        is_detected = 0;
    }

    return is_detected;
}
