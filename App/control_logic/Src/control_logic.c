/**
 * @file    control_logic.c
 * @brief   Control logic for pumping system on STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

/** @section Include */
#include "control_logic.h" // Include the header for control logic functions
#include "pump_drv.h" // Include the header for pump control functions

/** @section Define*/

/** @section Typedef */

/** @section Variables */
static PumpCommand current_pump_state = PUMP_COMMAND_OFF; // Initialize pump state to OFF

/** @section Static Functions */

/** @section Global Functions */

/**
 * @brief      Updates the pump control logic based on the current temperature with hysteresis x10.
 */
PumpCommand pump_control_logic_update(int16_t temperature_x10)
{

    if (current_pump_state == PUMP_COMMAND_OFF && temperature_x10 >= CONTROL_LOGIC_TEMP_ON_THRESHOLD_X10)
    {
        current_pump_state = PUMP_COMMAND_ON;
    }
    else if (current_pump_state == PUMP_COMMAND_ON && temperature_x10 <= CONTROL_LOGIC_TEMP_OFF_THRESHOLD_X10)
    {
        current_pump_state = PUMP_COMMAND_OFF;
    }

    return current_pump_state;
}
