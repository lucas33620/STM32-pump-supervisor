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

/** @section Include */
#include <stdint.h>
#include <limits.h>
#include "mcp9808_cfg.h"

/** @section Define*/
#define SENSOR_MCP9808_ADDR_MIN      (0x18U) /* The final address is 0x18 + A2 + A1 + A0.  */
#define SENSOR_MCP9808_ADDR_MAX      (0x1FU)  
#define SENSOR_MCP9808_REG_TA        (0x05U)   /* Ambient Temperature (TA) */

/** @section Typedef */

/** @section Global Functions */
/**
 * @brief ping I2C of component  	  
 *
 * @param  
 *
 * @return 0 = No detected and 1 = Detected
 *
 * @note
 */
uint8_t mcp9808_drv_is_detected(void);

#endif /* MCP9808_DRV_H */