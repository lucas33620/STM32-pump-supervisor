/**
 * @file    thermal_supervision.c
 * @brief   Thermal supervision functions for MCP9808 temperature sensor on STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

/** @section Include */
#include "thermal_supervision.h"
#include "mcp9808_drv.h"

/** @section Define*/

/** @section Typedef */

/** @section Variables */
static int last_valid_temperature_x10 = 0;
static ThermalSupervisionState current_state = THERMAL_SUPERVISION_STATE_INVALID_MEASUREMENT;

/** @section Static Functions */

/** @section Global Functions */
/**
 * @brief Update thermal supervision with a new temperature measurement.
 */
ThermalSupervisionState thermal_supervision_update_valid_temperature(int16_t temperature_x10)
{
    if (temperature_x10 < THERMAL_SUPERVISION_TEMP_MIN_THRESHOLD_X10 ||
        temperature_x10 > THERMAL_SUPERVISION_TEMP_MAX_THRESHOLD_X10)
    {
        current_state = THERMAL_SUPERVISION_STATE_INVALID_MEASUREMENT;
    }
    else
    {
        last_valid_temperature_x10 = temperature_x10;

        if (current_state == THERMAL_SUPERVISION_STATE_OVERTEMP)
        {
            if (temperature_x10 < THERMAL_SUPERVISION_OVERTEMP_RECOVERY_THRESHOLD_X10)
            {
                current_state = THERMAL_SUPERVISION_STATE_OK;
            }
        }
        else
        {
            if (temperature_x10 >= THERMAL_SUPERVISION_OVERTEMP_FAULT_THRESHOLD_X10)
            {
                current_state = THERMAL_SUPERVISION_STATE_OVERTEMP;
            }
            else
            {
                current_state = THERMAL_SUPERVISION_STATE_OK;
            }
        }
    }

    return current_state;
}

/**
 * @brief Set thermal supervision state to invalid measurement.
 */
void thermal_supervision_set_invalid_measurement()
{
    current_state = THERMAL_SUPERVISION_STATE_INVALID_MEASUREMENT;
}


/**
 * @brief Get current last valid temperature measurement.
 */
ThermalSupervisionState thermal_supervision_get_last_valid_temperature(int16_t *temperature_x10)
{
    if (temperature_x10 != NULL)
    {
        *temperature_x10 = last_valid_temperature_x10;
    }
    return current_state;
}

/**
 * @brief Get current thermal supervision state.
 */
ThermalSupervisionState thermal_supervision_get_state(void)
{
    return current_state;
}
