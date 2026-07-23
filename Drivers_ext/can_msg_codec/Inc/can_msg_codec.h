/**
 * @file    can_msg_codec.h
 * @brief   Header file for CAN message codec functions according to CAN_message_table.pdf
 * @author  Syloria, Lucas Baquey
 *
 * @copyright MIT License
 *
 * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef CAN_MSG_CODEC_H
#define CAN_MSG_CODEC_H

/** @section Include */
#include <stdint.h>
#include "control_logic.h"
#include "thermal_supervision.h"
#include "can_msg_codec_cfg.h"

/** @section Define */


/** @section Typedef */
typedef enum
{
    CAN_MSG_CODEC_STATUS_OK = 0,
    CAN_MSG_CODEC_STATUS_ERR_PARAM,
    CAN_MSG_CODEC_STATUS_ERR_VALUE
} CanMsgCodecStatus;

typedef enum
{
    CAN_THERMAL_STATE_BELOW_OFF_THRESHOLD = 0,
    CAN_THERMAL_STATE_BETWEEN_THRESHOLDS,
    CAN_THERMAL_STATE_ABOVE_ON_THRESHOLD,
    CAN_THERMAL_STATE_OVERTEMPERATURE,
    CAN_THERMAL_STATE_INVALID_MEASUREMENT,
    CAN_THERMAL_STATE_COUNT
} CanThermalState;

typedef struct
{
    int16_t temperature_x10;
    PumpCommand pump_command;
    CanThermalState thermal_state;
    int16_t activation_threshold_x10;
    int16_t deactivation_threshold_x10;
} CanThermalStatus;

/** @section Variables */


/** @section Static Functions */


/** @section Global Functions */


/**
 * @brief   Builds a thermal status CAN message (ID : 0x181).
 *
 * @param   can_msg Pointer to the CAN message buffer.
 * @param   thermal_state Current thermal supervision state.
 * @param   last_valid_temp_x10 Last valid temperature value multiplied by 10.
 * @param   pump_command Pump command to include in the message.
 *
 * @return  None.
 */
CanMsgCodecStatus can_msg_codec_build_thermal_status(uint8_t can_msg[CAN_MSG_THERMAL_STATUS_DLC], CanThermalState thermal_state, int16_t last_valid_temp_x10, PumpCommand pump_command);

#endif /* CAN_MSG_CODEC_H */
