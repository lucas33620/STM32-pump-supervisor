/**
 * @file    bsp_can.c
 * @brief   CAN bus BSP for STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

/** @section Include */
#include "bsp_can.h"

/** @section Define*/

/** @section Typedef */

/** @section Static Functions */

/** @section Global Functions */
/**
 * @brief   Initializes the CAN peripheral and configures it for communication.	  
 */
HAL_StatusTypeDef bsp_can_start(CAN_HandleTypeDef *hcan)
{
    HAL_StatusTypeDef status = HAL_OK;
    if (HAL_CAN_Start(hcan) != HAL_OK)
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief   Configures the CAN peripheral to operate in loopback mode, which is useful for testing and debugging.	  
 */
HAL_StatusTypeDef bsp_can_init_loopback_filter(CAN_HandleTypeDef *hcan)
{
    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow = 0x0000;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    return HAL_CAN_ConfigFilter(hcan, &filter);   
}

/**
 * @brief  Sends a CAN message with the specified header and data.
 */
HAL_StatusTypeDef bsp_can_send(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *tx_header, uint8_t *data, uint32_t *tx_mailbox)
{
    return HAL_CAN_AddTxMessage(hcan, tx_header, data, tx_mailbox);
}

/**
 * @brief  Receives a CAN message and stores it in the provided buffers.	  
 */
HAL_StatusTypeDef bsp_can_receive(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *rx_header, uint8_t *data)
{
    return HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, rx_header, data);
}

/**
 * @brief  Gets the fill level of the CAN receive FIFO.	  
 */
uint32_t bsp_can_get_rx_fifo_fill_level(CAN_HandleTypeDef *hcan)
{
    return HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0);
}