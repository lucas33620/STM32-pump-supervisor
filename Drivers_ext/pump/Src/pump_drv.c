/**
 * @file    pump_drv.c
 * @brief   Sensor management module for pump.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

/** @section Include */
#include "stm32f4xx_hal.h"
#include "pump_drv.h"

/** @section Define*/

/** @section Static Functions */

/** @section Global Functions */
/**
 * @brief Initializes the pump driver context.
 */
PumpStatus pump_drv_init(PumpCtx *ctx, GPIO_TypeDef *gpio_port, uint16_t gpio_pin)
{
    PumpStatus pump_status = PUMP_STATUS_OK;

    if ((ctx == NULL) || (gpio_port == NULL) || (gpio_pin == 0U))
    {
        pump_status = PUMP_STATUS_ERR_PARAM;
    }
    else
    {
        ctx->gpio_port = gpio_port;
        ctx->gpio_pin = gpio_pin;
        ctx->fault_count = 0U;

        /* No heritage of unknown state */
        HAL_GPIO_WritePin(ctx->gpio_port, ctx->gpio_pin, GPIO_PIN_RESET);
        ctx->state = PUMP_STATE_OFF;
    }
    return pump_status;
}

/**
 * @brief Sets the pump ON by driving the control pin to the appropriate state. 
 */
PumpStatus pump_drv_set_on(PumpCtx *ctx)
{
    PumpStatus pump_status = PUMP_STATUS_OK;

    if(ctx == NULL)
    {
        pump_status = PUMP_STATUS_ERR_PARAM;
    }
    else if (ctx->state == PUMP_STATE_UNINIT)
    {
        pump_status = PUMP_STATUS_ERR_INIT;
    }
    else
    {
        /* Drive pump command output high */
        HAL_GPIO_WritePin(ctx->gpio_port, ctx->gpio_pin, GPIO_PIN_SET);        
        ctx->state = PUMP_STATE_ON;
    }
    return pump_status;
}

/**
 * @brief Sets the pump OFF by driving the control pin to the appropriate state.
 */
PumpStatus pump_drv_set_off(PumpCtx *ctx)
{
    PumpStatus pump_status = PUMP_STATUS_OK;

    if(ctx == NULL)
    {
        pump_status = PUMP_STATUS_ERR_PARAM;
    }
    else if (ctx->state == PUMP_STATE_UNINIT)
    {
        pump_status = PUMP_STATUS_ERR_INIT;
    }
    else
    {
        /* Drive pump command output low */
        HAL_GPIO_WritePin(ctx->gpio_port, ctx->gpio_pin, GPIO_PIN_RESET);        
        ctx->state = PUMP_STATE_OFF;
    }
    return pump_status;
}

/**
 * @brief Gets the current command state of the pump.
 */
PumpStatus pump_drv_get_command_state(PumpCtx *ctx, PumpState *pump_state)
{
    PumpStatus pump_status = PUMP_STATUS_OK;

    if ((ctx == NULL) || (pump_state == NULL))
    {
        pump_status = PUMP_STATUS_ERR_PARAM;
    }
    else
    {
        *pump_state = ctx->state;
    }
    return pump_status;
}
