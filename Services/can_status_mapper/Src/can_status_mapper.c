/**
 * @file    can_status_mapper.c
 * @brief   Implementation file for CAN status mapper functions to map divers states to CAN states.
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
#include "can_status_mapper.h"

/** @section Define */


/** @section Typedef */


/** @section Variables */


/** @section Static Functions */


/** @section Global Functions */

/**
 * @brief   Maps the thermal supervision state to a CAN thermal state and builds a CAN thermal status message.
 */
CanStatusMapperStatus can_status_mapper_map_thermal_state(
    PumpControlRegion control_region,
    CanThermalState *can_thermal_state)
{
    if (can_thermal_state == NULL)
    {
        return CAN_STATUS_MAPPER_STATUS_ERR_PARAM;
    }

    switch (control_region)
    {
        case PUMP_CONTROL_REGION_BELOW_OFF_THRESHOLD:
            *can_thermal_state =
                CAN_THERMAL_STATE_BELOW_OFF_THRESHOLD;
            break;

        case PUMP_CONTROL_REGION_BETWEEN_THRESHOLDS:
            *can_thermal_state =
                CAN_THERMAL_STATE_BETWEEN_THRESHOLDS;
            break;

        case PUMP_CONTROL_REGION_ABOVE_ON_THRESHOLD:
            *can_thermal_state =
                CAN_THERMAL_STATE_ABOVE_ON_THRESHOLD;
            break;

        case PUMP_CONTROL_REGION_OVERTEMPERATURE:
            *can_thermal_state =
                CAN_THERMAL_STATE_OVERTEMPERATURE;
            break;

        case PUMP_CONTROL_REGION_INVALID_MEASUREMENT:
            *can_thermal_state =
                CAN_THERMAL_STATE_INVALID_MEASUREMENT;
            break;

        default:
            return CAN_STATUS_MAPPER_STATUS_ERR_VALUE;
    }

    return CAN_STATUS_MAPPER_STATUS_OK;
}
