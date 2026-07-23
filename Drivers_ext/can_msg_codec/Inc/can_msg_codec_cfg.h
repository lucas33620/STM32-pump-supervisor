/**
 * @file    can_msg_codec_cfg.h
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

#ifndef CAN_MSG_CODEC_CFG_H
#define CAN_MSG_CODEC_CFG_H


/**
 *  @brief Only control logic configuration may be changed here.
 */

/** @section Defines */
/** Thermal status message configuration */
/**< CAN message ID for thermal status message */
#define CAN_MSG_THERMAL_STATUS_ID   (0x181U)

/**< CAN message Data Length Code (DLC) for thermal status message */
#define CAN_MSG_THERMAL_STATUS_DLC  (8U)

#endif /* CAN_MSG_CODEC_CONFIG_H */
