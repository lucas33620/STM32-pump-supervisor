/**
 * @file    fault_manager.c
 * @brief   Description du module.
 * @author  Syloria, Lucas Baquey
 *
 * @copyright MIT License
 *
 * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

/** @section Include */
#include "fault_manager.h"

/** @section Define */

/** @section Typedef */


/** @section Variables */
static bool fault_active[FAULT_ID_COUNT];
static bool fault_manager_is_valid_id(FaultId fault_id);

/** @section Static Functions */
/**
 * @brief   Checks if a given fault ID is valid.
 * 
 * @param   fault_id The fault ID to check.
 * 
 * @return  true if the fault ID is valid, false otherwise.
 */
static bool fault_manager_is_valid_id(FaultId fault_id)
{
    return ((fault_id > FAULT_ID_NONE) &&
            (fault_id < FAULT_ID_COUNT));
}

/** @section Global Functions */
/**
 * @brief   Initializes the fault manager module.
 */
void fault_manager_init(void)
{
    for (int i =0; i < FAULT_ID_COUNT; i++)
    {
        fault_active[i] = false;
    }
}

/**
 * @brief   Allows a software module to declare a fault condition.
 */
void fault_manager_raise_fault(FaultId fault_id)
{
    if (fault_manager_is_valid_id(fault_id))
    {
        fault_active[fault_id] = true;
    }
}

/**
 * @brief   Allows a software module to clear a previously declared fault condition.
 */
void fault_manager_clear_fault(FaultId fault_id)
{
    if (fault_manager_is_valid_id(fault_id))
    {
        if (fault_active[fault_id] == true)
        {
            fault_active[fault_id] = false;
        }
    }
}

/**
 * @brief   Allows a software module to check the status of a fault condition.
 */
bool fault_manager_is_fault_active(FaultId fault_id)
{
    bool is_active = false;
    
    if (fault_manager_is_valid_id(fault_id))
    {
        if (fault_active[fault_id] == true)
        {
            is_active = true;
        }
    }
    
    return is_active;
}
