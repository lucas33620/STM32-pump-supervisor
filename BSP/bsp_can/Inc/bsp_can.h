/**
 * @file    bsp_can.h
 * @brief   CAN bus BSP for STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef BSP_CAN_H
#define BSP_CAN_H

/** @section Include */
#include <stdbool.h>
#include <stdint.h>
#include "can.h"

/** @section Define*/

/** @section Typedef */

/** @section Global Functions */
/**
 * @brief   Initializes the CAN peripheral and configures it for communication.	  
 *
 * @param   None  
 *
 * @return  HAL_StatusTypeDef: HAL_OK if initialization is successful, otherwise an error code. 
 */
HAL_StatusTypeDef bsp_can_start(CAN_HandleTypeDef *hcan);

/**
 * @brief   Configures the CAN peripheral to operate in loopback mode, which is useful for testing and debugging.	  
 *
 * @param   None  
 *
 * @return  HAL_StatusTypeDef: HAL_OK if configuration is successful, otherwise an error code.
 */
HAL_StatusTypeDef bsp_can_init_loopback_filter(CAN_HandleTypeDef *hcan);

/**
 * @brief  Sends a CAN message with the specified header and data.	  
 *
 * @param  tx_header: Pointer to a CAN_TxHeaderTypeDef structure that contains the header information for the CAN message.  
 *
 * @return HAL_StatusTypeDef: HAL_OK if the message is sent successfully, otherwise an error code. 
 */
HAL_StatusTypeDef bsp_can_send(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *tx_header, uint8_t *data, uint32_t *tx_mailbox);

/**
 * @brief  Receives a CAN message and stores it in the provided buffers.	  
 *
 * @param  rx_header: Pointer to a CAN_RxHeaderTypeDef structure that will contain the header information for the received CAN message.  
 * @param  data: Pointer to a buffer where the received CAN message data will be stored.  
 *
 * @return HAL_StatusTypeDef: HAL_OK if a message is received successfully, otherwise an error code. 
 */
HAL_StatusTypeDef bsp_can_receive(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *rx_header, uint8_t *data);

/**
 * @brief  Gets the fill level of the CAN receive FIFO.	  
 *
 * @param  None
 *
 * @return uint32_t: The number of messages currently in the CAN receive FIFO. 
 */
uint32_t bsp_can_get_rx_fifo_fill_level(CAN_HandleTypeDef *hcan);

#endif /* BSP_CAN_H */