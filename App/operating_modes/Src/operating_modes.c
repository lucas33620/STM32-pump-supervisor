/**
 * @file    operating_modes.c
 * @brief   Operating modes management module.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

 /** @section Include */
#include "operating_modes.h"

/** @section Define*/

/** @section Typedef */

/** @section Variables */
static OperatingMode operating_modes_current = OPERATING_MODE_INIT;

/** @section Static Functions */
static uint8_t operating_modes_apply_transition(OperatingMode new_mode);

/**
 * @brief Apply a transition to a new operating mode.
 */
static uint8_t operating_modes_apply_transition(OperatingMode new_mode)
{
    if (operating_modes_current == new_mode)
    {
        return 0U;
    }

    operating_modes_current = new_mode;
    return 1U;
}

/** @section Global Functions */

/**
 * @brief Initialize the operating modes module.
 */
void operating_modes_init(void)
{
    operating_modes_current = OPERATING_MODE_INIT;
}

/**
 * @brief Get the current operating mode.
 */
OperatingMode operating_modes_get_current(void)
{
    return operating_modes_current;
}

/**
 * @brief Convert an operating mode to its string representation.
 */
const char *operating_modes_to_string(OperatingMode mode)
{
    char *str = "UNKNOWN";
    switch (mode)
    {
        case OPERATING_MODE_INIT:
            str = "INIT";
            break;

        case OPERATING_MODE_NORMAL:
            str = "NORMAL";
            break;

        case OPERATING_MODE_DEGRADED:
        	str = "DEGRADED";
        	break;

        case OPERATING_MODE_FAULT_LATCHED:
            str = "FAULT";
            break;

        default:
            str = "UNKNOWN";
    }
    return str;
}
/**
 * @brief Make rules transition between INIT -> NORMAL
 */
uint8_t operating_modes_on_initialization_successful(void)
{
    uint8_t status = 0U;
    if (operating_modes_current == OPERATING_MODE_INIT)
    {
        status = operating_modes_apply_transition(OPERATING_MODE_NORMAL);
    }
    return status;
}

/**
 * @brief Make rules transition between INIT -> FAULT_LATCHED
 */
uint8_t operating_modes_on_critical_initialization_fault(void)
{
    uint8_t status = 0U;
    if (operating_modes_current == OPERATING_MODE_INIT)
    {
        status = operating_modes_apply_transition(OPERATING_MODE_FAULT_LATCHED);
    }
    return status;
}

/**
 * @brief Make rules transition between NORMAL -> DEGRADED
 */
uint8_t operating_modes_on_recoverable_fault_confirmed(void)
{
    uint8_t status = 0U;
    if (operating_modes_current == OPERATING_MODE_NORMAL)
    {
        status = operating_modes_apply_transition(OPERATING_MODE_DEGRADED);
    }
    return status;
}

/**
 * @brief Make rules transition between NORMAL/DEGRADED -> FAULT_LATCHED
 */
uint8_t operating_modes_on_critical_fault_confirmed(void)
{
    uint8_t status = 0U;
    if (operating_modes_current == OPERATING_MODE_NORMAL || operating_modes_current == OPERATING_MODE_DEGRADED)
    {
        status = operating_modes_apply_transition(OPERATING_MODE_FAULT_LATCHED);
    }
    return status;
}

/**
 * @brief Make rules transition between DEGRADED -> NORMAL
 */
uint8_t operating_modes_on_recovery_conditions_met(void)
{
    uint8_t status = 0U;
    if (operating_modes_current == OPERATING_MODE_DEGRADED)
    {
        status = operating_modes_apply_transition(OPERATING_MODE_NORMAL);
    }
    return status;
}

/**
 * @brief Make rules transition between FAULT_LATCHED -> INIT
 */
uint8_t operating_modes_on_user_reset_clear_authorized(void)
{
    uint8_t status = 0U;
    if (operating_modes_current == OPERATING_MODE_FAULT_LATCHED)
    {
        status = operating_modes_apply_transition(OPERATING_MODE_INIT);
    }
    return status;
}
