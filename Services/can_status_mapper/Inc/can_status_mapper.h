/**
 * @file    can_status_mapper.h
 * @brief   Header file for CAN status mapper functions to map divers states to CAN states.
 * @author  Syloria, Lucas Baquey
 *
 * @copyright MIT License
 *
 * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef CAN_STATUS_MAPPER_H
#define CAN_STATUS_MAPPER_H

/** @section Include */
#include "can_msg_codec.h"
#include "thermal_supervision.h"

/** @section Define */


/** @section Typedef */
typedef enum
{
    CAN_STATUS_MAPPER_STATUS_OK = 0,
    CAN_STATUS_MAPPER_STATUS_ERR_PARAM,
    CAN_STATUS_MAPPER_STATUS_ERR_VALUE
} CanStatusMapperStatus;

/** @section Variables */


/** @section Static Functions */


/** @section Global Functions */

/**
 * @brief   Maps the thermal supervision state to a CAN thermal state and builds a CAN thermal status message.
 *
 * @param   PumpControlRegion control_region
 * @param   CanThermalState *can_thermal_state
 *
 * @return  CAN_MSG_CODEC_STATUS_OK if the mapping and building were successful, otherwise an error status.
 */
CanStatusMapperStatus can_status_mapper_map_thermal_state(
    PumpControlRegion control_region,
    CanThermalState *can_thermal_state);

#endif 
