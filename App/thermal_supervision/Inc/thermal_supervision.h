/**
 * @file    thermal_supervision.h
 * @brief   Thermal supervision functions for MCP9808 temperature sensor on STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 * 
 * @note Thermal_supervision centralizes the thermal state of the current subsystem and currently manages a single application temperature source.
 */

#ifndef THERMAL_SUPERVISION_H
#define THERMAL_SUPERVISION_H

/** @section Include */
#include <stdint.h>
#include <stdio.h>

#include "thermal_supervision_cfg.h"

/** @section Define*/

/** @section Typedef */
typedef enum
{
    THERMAL_SUPERVISION_STATE_OK = 0,
    THERMAL_SUPERVISION_STATE_OVERTEMP,
    THERMAL_SUPERVISION_STATE_INVALID_MEASUREMENT,
} ThermalSupervisionState;

/** @section Variables */

/** @section Static Functions */

/** @section Global Functions */
/**
 * @brief Update thermal supervision with a new temperature measurement.
 * @param temperature_x10 Temperature in degree Celsius x10.
 * @return Current thermal supervision state after update.
 */
ThermalSupervisionState thermal_supervision_update_valid_temperature(int16_t temperature_x10);

/**
 * @brief Set thermal supervision state to invalid measurement.
 */
void thermal_supervision_set_invalid_measurement();

/**
 * @brief Get current last valid temperature measurement.
 * @param temperature_x10 Pointer to destination.
 * @return Current thermal supervision state.
 */
ThermalSupervisionState thermal_supervision_get_last_valid_temperature(int16_t *temperature_x10);

/**
 * @brief Get current thermal supervision state.
 * @return Current thermal supervision state.
 */
ThermalSupervisionState thermal_supervision_get_state(void);

#endif /* THERMAL_SUPERVISION_H */
