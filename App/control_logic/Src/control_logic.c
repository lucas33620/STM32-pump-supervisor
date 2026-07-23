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

/** @section Define*/

/** @section Typedef */

/** @section Variables */
static PumpControlResult current_control_result =
{
    .command = PUMP_COMMAND_OFF,
    .region = PUMP_CONTROL_REGION_INVALID_MEASUREMENT
};

/** @section Static Functions */

/** @section Global Functions */

/**
 * @brief      Updates the pump control logic based on the current temperature with hysteresis x10.
 */
PumpControlResult pump_control_logic_update(
    ThermalSupervisionState supervision_state,
    int16_t temperature_x10)
{
    switch (supervision_state)
    {
        case THERMAL_SUPERVISION_STATE_INVALID_MEASUREMENT:
            current_control_result.command = PUMP_COMMAND_OFF;
            current_control_result.region =
                PUMP_CONTROL_REGION_INVALID_MEASUREMENT;
            break;

        case THERMAL_SUPERVISION_STATE_OVERTEMP:
            current_control_result.command = PUMP_COMMAND_ON;
            current_control_result.region =
                PUMP_CONTROL_REGION_OVERTEMPERATURE;
            break;

        case THERMAL_SUPERVISION_STATE_OK:
            if (temperature_x10 <=
                CONTROL_LOGIC_TEMP_OFF_THRESHOLD_X10)
            {
                current_control_result.region =
                    PUMP_CONTROL_REGION_BELOW_OFF_THRESHOLD;
            }
            else if (temperature_x10 <
                     CONTROL_LOGIC_TEMP_ON_THRESHOLD_X10)
            {
                current_control_result.region =
                    PUMP_CONTROL_REGION_BETWEEN_THRESHOLDS;
            }
            else
            {
                current_control_result.region =
                    PUMP_CONTROL_REGION_ABOVE_ON_THRESHOLD;
            }

            if ((current_control_result.command == PUMP_COMMAND_OFF) &&
                (temperature_x10 >=
                 CONTROL_LOGIC_TEMP_ON_THRESHOLD_X10))
            {
                current_control_result.command = PUMP_COMMAND_ON;
            }
            else if ((current_control_result.command == PUMP_COMMAND_ON) &&
                     (temperature_x10 <=
                      CONTROL_LOGIC_TEMP_OFF_THRESHOLD_X10))
            {
                current_control_result.command = PUMP_COMMAND_OFF;
            }
            else
            {
                /* Keep current command inside hysteresis band. */
            }
            break;

        default:
            current_control_result.command = PUMP_COMMAND_OFF;
            current_control_result.region =
                PUMP_CONTROL_REGION_INVALID_MEASUREMENT;
            break;
    }

    return current_control_result;
}

/**
 * @brief      Gets the current pump command.
 */
PumpControlResult pump_control_logic_get_current_result(void)
{
    return current_control_result;
}

