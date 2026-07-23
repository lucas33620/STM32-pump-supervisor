/**
 * @file    control_logic.h
 * @brief   Control logic for pumping system on STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef CONTROL_LOGIC_H
#define CONTROL_LOGIC_H

/** @section Include */
#include <stdint.h>
#include "thermal_supervision.h"
#include "control_logic_cfg.h" // Include the configuration header for control logic

/** @section Define*/

/** @section Typedef */
typedef enum {
    PUMP_COMMAND_OFF = 0,
    PUMP_COMMAND_ON
} PumpCommand;

/** @section Variables */

/** @section Static Functions */

/** @section Global Functions */

/**
 * @brief	  Updates the pump control logic based on the current temperature with hysteresis x10.
 *
 * @param supervision_state  The current thermal supervision state.
 * @param temperature_c  The current temperature in degrees Celsius x 10.
 *
 * @return  The updated pump command.
 */
PumpCommand pump_control_logic_update(ThermalSupervisionState supervision_state, int16_t temperature_c);

/**
 * @brief	  Gets the current pump command.
 *
 * @return  The current pump command.
 */
PumpCommand pump_control_logic_get_current_command(void);

#endif /* CONTROL_LOGIC_H */
