/**
 * @file    operating_modes.h
 * @brief   Operating modes management module.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef OPERATING_MODES_H
#define OPERATING_MODES_H

/** @section Include */
#include <stdint.h>

/** @section Define*/

/** @section Typedef */
typedef enum{
    OPERATING_MODE_INIT = 0,
    OPERATING_MODE_NORMAL,
    OPERATING_MODE_DEGRADED,
    OPERATING_MODE_FAULT_LATCHED
} OperatingMode;

/** @section Static Functions */

/** @section Global Functions */

/**
 * @brief Initialize the operating modes module.
 *
 * @param None
 *
 * @return None
 */
void operating_modes_init(void);

/**
 * @brief Gets the current operating mode. 	  
 *
 * @param None  
 *
 * @return The current operating mode. 
 */
OperatingMode operating_modes_get_current(void);

/**
 * @brief Converts an operating mode to its string representation.	  
 *
 * @param mode The operating mode to convert.  
 *
 * @return A string representation of the operating mode. 
 */
const char *operating_modes_to_string(OperatingMode mode);

/**
 * @brief Make transition between INIT -> NORMAL
 */
uint8_t operating_modes_on_initialization_successful(void);

/**
 * @brief Make transition between INIT -> FAULT_LATCHED
 */
uint8_t operating_modes_on_critical_initialization_fault(void);

/**
 * @brief Make transition between NORMAL -> DEGRADED 
 */
uint8_t operating_modes_on_recoverable_fault_confirmed(void);

/**
 * @brief Make transition between DEGRADED -> NORMAL 
 */
uint8_t operating_modes_on_critical_fault_confirmed(void);

/**
 * @brief Make transition between DEGRADED -> NORMAL 
 */
uint8_t operating_modes_on_recovery_conditions_met(void);

/**
 * @brief Make transition between FAULT_LATCHED -> INIT
 */
uint8_t operating_modes_on_user_reset_clear_authorized(void);

#endif /* OPERATING_MODES_H */
