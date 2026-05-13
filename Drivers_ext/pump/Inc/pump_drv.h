/**
 * @file    pump_drv.h
 * @brief   Sensor management module for pump.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef PUMP_DRV_H
#define PUMP_DRV_H

/** @section Include */
#include <stdbool.h>
#include <stdint.h>
#include "pump_cfg.h"

/** @section Define*/

/** @section Typedef */
typedef enum
{
    PUMP_STATUS_OK = 0,
    PUMP_STATUS_ERR_INIT,
    PUMP_STATUS_ERR_PARAM,
    PUMP_STATUS_ERR_HW,
    PUMP_STATUS_ERR_TIMEOUT
} PumpStatus;


typedef enum
{
    PUMP_STATE_UNINIT = 0,
    PUMP_STATE_OFF,
    PUMP_STATE_ON,
    PUMP_STATE_FAULT,
} PumpState;

typedef struct
{
    GPIO_TypeDef  *gpio_port;
    uint16_t gpio_pin;
    PumpState state;
    uint16_t fault_count;

} PumpCtx;

/** @section Global Functions */
/**
 * @brief Initializes the pump driver context.	  
 *
 * @param ctx Pointer to the pump context structure.
 * @param gpio_port GPIO port where the pump control pin is connected.
 * @param gpio_pin GPIO pin number for the pump control.  
 *
 * @return PUMP_STATUS_OK if initialization succeeded or error status otherwise.
 */
PumpStatus pump_drv_init(PumpCtx *ctx, GPIO_TypeDef  *gpio_port, uint16_t gpio_pin);

/**
 * @brief Sets the pump ON by driving the control pin to the appropriate state. 
 *
 * @param ctx Pointer to the pump context structure.
 *
 * @return PUMP_STATUS_OK if the operation succeeded or error status otherwise.
 */
PumpStatus pump_drv_set_on(PumpCtx *ctx);

/**
 * @brief Sets the pump OFF by driving the control pin to the appropriate state.
 *
 * @param ctx Pointer to the pump context structure.
 *
 * @return PUMP_STATUS_OK if the operation succeeded or error status otherwise.
 */
PumpStatus pump_drv_set_off(PumpCtx *ctx);

/**
 * @brief Gets the current command state of the pump.
 *
 * @param ctx Pointer to the pump context structure.
 * @param pump_state Pointer to a variable where the command state will be stored.
 *
 * @return PUMP_STATUS_OK if the operation succeeded or error status otherwise.
 */
PumpStatus pump_drv_get_command_state(PumpCtx *ctx, PumpState *pump_state);

#endif /* PUMP_DRV_H */
