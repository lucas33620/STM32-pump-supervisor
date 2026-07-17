/**
 * @file    fault_manager.h
 * @brief   Include an internal fault management interface used by software modules to declare, update, clear, and query faults.
 * @author  Syloria, Lucas Baquey
 *
 * @copyright MIT License
 *
 * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

/** @section Include */
#include <stdint.h>
#include <stdbool.h>

/** @section Define */
 

/** @section Typedef */
typedef enum
{
    FAULT_ID_NONE = 0x00,

    /* Sensor communication fault: 3 consecutive sensor acquisition failures or timeouts. */
    FAULT_ID_F001_SENSOR_COMMUNICATION = 0x01,

    /* Temperature out of application range: two consecutive values below -20 °C or above +100 °C. */
    FAULT_ID_F002_TEMPERATURE_RANGE = 0x02,

    /* Measured temperature exceeds the configured threshold. */
    FAULT_ID_F003_OVERTEMPERATURE = 0x03,

    /* Pump commanded ON without the expected thermal effect. (not implemented) */
    FAULT_ID_F004_PUMP_NO_EFFECT = 0x04,

    /* Critical task heartbeat missing for more than 400 ms. */
    FAULT_ID_F005_TASK_SUPERVISION = 0x05,

    /* Unsupported or malformed CAN diagnostic command. */
    FAULT_ID_F006_INVALID_DIAG_COMMAND = 0x06,

    FAULT_ID_COUNT
} FaultId;


/** @section Variables */


/** @section Static Functions */


/** @section Global Functions */
/**
 * @brief   Initializes the fault manager module.
 * 
 * @param   
 *
 * @return  
 */
void fault_manager_init(void);

/**
 * @brief   Allows a software module to declare a fault condition.
 *
 * @param   fault_id The unique identifier of the fault to be raised.
 *
 * @return  
 */
void fault_manager_raise_fault(FaultId fault_id);

/**
 * @brief   Allows a software module to clear a previously declared fault condition.
 *
 * @param   fault_id The unique identifier of the fault to be cleared.
 *
 * @return  
 */
void fault_manager_clear_fault(FaultId fault_id);

/**
 * @brief   Allows a software module to check the status of a fault condition.
 *
 * @param   fault_id The unique identifier of the fault to be checked.
 *
 * @return  false if the fault is inactive, true if the fault is active.
 */
bool fault_manager_is_fault_active(FaultId fault_id);

#endif /* FAULT_MANAGER_H */
