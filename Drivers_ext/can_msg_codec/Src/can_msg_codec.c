/**
 * @file    can_msg_codec.c
 * @brief   Source file for CAN message codec functions according to CAN_message_table.pdf
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
#include "can_msg_codec.h"

/** @section Define */


/** @section Typedef */


/** @section Variables */


/** @section Static Functions */
static void can_msg_codec_write_int16_le(uint8_t *buffer, uint8_t offset, int16_t value)
{
    uint16_t uvalue = (uint16_t)value; // Cast to unsigned to avoid sign extension issues
    buffer[offset] = (uint8_t)(uvalue & 0xFF);         // LSB
    buffer[offset + 1] = (uint8_t)((uvalue >> 8) & 0xFF); // MSB
}

/** @section Global Functions */


/**
 * @brief   Builds a thermal status CAN message (ID : 0x181).
 * 
 * @note    Byte 0: Temperature (LSB)
 * @note    Byte 1: Temperature (MSB)
 * @note    Byte 2: Pump command (0x00 for OFF, 0x01 for ON)
 * @note    Byte 3: Thermal supervision state (0x00 for OK, 0x01 for WARNING, 0x02 for CRITICAL)
 * @note    Byte 4: activation Thresold (LSB)
 * @note    Byte 5: activation Thresold (MSB)
 * @note    Byte 6: deactivation Thresold (LSB)
 * @note    Byte 7: deactivation Thresold (MSB)
 */
CanMsgCodecStatus can_msg_codec_build_thermal_status(uint8_t can_msg[CAN_MSG_THERMAL_STATUS_DLC],  CanThermalState thermal_state,  
                                                        int16_t last_valid_temp_x10, PumpCommand pump_command)
{
    CanMsgCodecStatus status = CAN_MSG_CODEC_STATUS_OK;

    if (can_msg == NULL)
    {
        status = CAN_MSG_CODEC_STATUS_ERR_PARAM;
    }
    else{

        /*
        * Clear the complete payload first so that a partially built
        * or stale frame cannot be transmitted after an error.
        */
        for (uint32_t index = 0U;
            index < CAN_MSG_THERMAL_STATUS_DLC;
            index++)
        {
            can_msg[index] = 0U;
        }

        if (thermal_state >= CAN_THERMAL_STATE_COUNT)
        {
            status = CAN_MSG_CODEC_STATUS_ERR_VALUE;
        }

        else if ((pump_command != PUMP_COMMAND_OFF) && (pump_command != PUMP_COMMAND_ON))
        {
            status = CAN_MSG_CODEC_STATUS_ERR_VALUE;
        }
        else 
        {
            // Payload construction according to CAN_message_table.pdf
            can_msg_codec_write_int16_le( can_msg, 0U, last_valid_temp_x10); // byte 0-1: Last valid temperature value multiplied by 10
            can_msg[2] = (uint8_t)pump_command; // byte 2: Pump command (0x00 for OFF, 0x01 for ON)
            can_msg[3] = (uint8_t)thermal_state; // byte 3: Thermal supervision state
            can_msg_codec_write_int16_le( can_msg, 4U, CONTROL_LOGIC_TEMP_ON_THRESHOLD_X10); // byte 4-5: Activation threshold 
            can_msg_codec_write_int16_le(can_msg, 6U, CONTROL_LOGIC_TEMP_OFF_THRESHOLD_X10); // byte 6-7: Deactivation threshold
        }
    }
    return status;
}