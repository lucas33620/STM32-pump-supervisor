/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "can.h"
#include "mcp9808_drv.h"
#include "bsp_can.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for task_sense */
osThreadId_t task_senseHandle;
const osThreadAttr_t task_sense_attributes = {
  .name = "task_sense",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task_control */
osThreadId_t task_controlHandle;
const osThreadAttr_t task_control_attributes = {
  .name = "task_control",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task_diag */
osThreadId_t task_diagHandle;
const osThreadAttr_t task_diag_attributes = {
  .name = "task_diag",
  .stack_size = 384 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for task_supervisor */
osThreadId_t task_supervisorHandle;
const osThreadAttr_t task_supervisor_attributes = {
  .name = "task_supervisor",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskSense(void *argument);
void StartTaskControl(void *argument);
void StartTaskDiag(void *argument);
void StartTaskSupervisor(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of task_sense */
  task_senseHandle = osThreadNew(StartTaskSense, NULL, &task_sense_attributes);

  /* creation of task_control */
  task_controlHandle = osThreadNew(StartTaskControl, NULL, &task_control_attributes);

  /* creation of task_diag */
  task_diagHandle = osThreadNew(StartTaskDiag, NULL, &task_diag_attributes);

  /* creation of task_supervisor */
  task_supervisorHandle = osThreadNew(StartTaskSupervisor, NULL, &task_supervisor_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskSense */
/**
* @brief Function implementing the task_sense thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSense */
void StartTaskSense(void *argument)
{
  int16_t temp_x10;
  int16_t temp_int;
  int16_t temp_frac;
  Mcp9808Status mcp9808_status;
  Mcp9808Ctx mcp9808_ctx;
  char temp_msg[32];

  mcp9808_status = mcp9808_drv_init(&mcp9808_ctx, 0x18U);
  if (mcp9808_status != MCP9808_STATUS_OK)
  {
    HAL_UART_Transmit(&huart3,
                      (uint8_t *)"MCP9808 init FAIL\r\n",
                      strlen("MCP9808 init FAIL\r\n"),
                      HAL_MAX_DELAY);

    for (;;)
    {
      osDelay(1000U);
    }
  }

  for (;;)
  {
    mcp9808_status = mcp9808_drv_get_temperature_x10(&mcp9808_ctx, &temp_x10);

    if (mcp9808_status == MCP9808_STATUS_OK)
    {
      temp_int = temp_x10 / 10;
      temp_frac = temp_x10 % 10;

      if (temp_frac < 0)
      {
        temp_frac = -temp_frac;
      }

      (void)snprintf(temp_msg,
                      sizeof(temp_msg),
                      "MCP9808 temp=%d.%d C\r\n",
                      temp_int,
                      temp_frac);

      HAL_UART_Transmit(&huart3,
                        (uint8_t *)temp_msg,
                        strlen(temp_msg),
                        HAL_MAX_DELAY);
    }
    else
    {
      HAL_UART_Transmit(&huart3,
                        (uint8_t *)"MCP9808 temp read FAIL\r\n",
                        strlen("MCP9808 temp read FAIL\r\n"),
                        HAL_MAX_DELAY);
    }

    osDelay(1000U);
  }
  /* USER CODE END StartTaskSense */
}

/* USER CODE BEGIN Header_StartTaskControl */
/**
* @brief Function implementing the task_control thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskControl */
void StartTaskControl(void *argument)
{
  /* USER CODE BEGIN StartTaskControl */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskControl */
}

/* USER CODE BEGIN Header_StartTaskDiag */
/**
* @brief Function implementing the task_diag thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskDiag */
void StartTaskDiag(void *argument)
{
  uint8_t tx_data[8] = {0};
  uint8_t rx_data[8] = {0};
  uint8_t counter = 0U;
  uint8_t reset_cause = 0x01U;
  uint8_t status_flags = 0x00U;
  uint16_t heartbeat_ticks = 0U;

  CAN_TxHeaderTypeDef tx_header = {0};
  CAN_RxHeaderTypeDef rx_header = {0};
  uint32_t tx_mailbox = 0U;
  char msg[64];

  if (bsp_can_init_loopback_filter(&hcan1) != HAL_OK)
  {
    snprintf(msg, sizeof(msg), "CAN filter init FAIL\r\n");
    HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    /* SLEEP */
    for (;;)
    {
      osDelay(1000);
    }
  }

  snprintf(msg, sizeof(msg), "CAN filter init OK\r\n");
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

  if (bsp_can_start(&hcan1) != HAL_OK)
  {
    snprintf(msg, sizeof(msg), "CAN start FAIL\r\n");
    HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    /* SLEEP */
    for (;;)
    {
      osDelay(1000);
    }
  }

  snprintf(msg, sizeof(msg), "CAN start OK\r\n");
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

  tx_header.StdId = 0x180U;
  tx_header.ExtId = 0x00U;
  tx_header.IDE = CAN_ID_STD;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.DLC = 8U;
  tx_header.TransmitGlobalTime = DISABLE;

  for (;;)
  {
    tx_data[0] = counter;
    tx_data[1] = reset_cause;
    tx_data[2] = status_flags;
    tx_data[3] = 0x00U;
    tx_data[4] = (uint8_t)((heartbeat_ticks >> 8) & 0xFFU);
    tx_data[5] = (uint8_t)(heartbeat_ticks & 0xFFU);
    tx_data[6] = 0x00U;
    tx_data[7] = 0x00U;

    if (bsp_can_send(&hcan1, &tx_header, tx_data, &tx_mailbox) == HAL_OK)
    {
      snprintf(msg, sizeof(msg), "CAN TX heartbeat cnt=%u\r\n", counter);
      HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    }
    else
    {
      snprintf(msg, sizeof(msg), "CAN TX FAIL\r\n");
      HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    }

    osDelay(10);

    if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0U)
    {
      if (bsp_can_receive(&hcan1, &rx_header, rx_data) == HAL_OK)
      {
        if ((rx_header.StdId == tx_header.StdId) &&
            (rx_header.DLC == tx_header.DLC) &&
            (rx_data[0] == counter))
        {
          snprintf(msg, sizeof(msg), "CAN RX heartbeat cnt=%u\r\n", rx_data[0]);
          HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

          snprintf(msg, sizeof(msg),
          "CAN RX id=0x%03lX dlc=%u data=%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
          rx_header.StdId,
          rx_header.DLC,
          rx_data[0], rx_data[1], rx_data[2], rx_data[3],
          rx_data[4], rx_data[5], rx_data[6], rx_data[7]);
          HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
        }
        else
        {
          snprintf(msg, sizeof(msg), "CAN RX data mismatch\r\n");
          HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
        }
      }
      else
      {
        snprintf(msg, sizeof(msg), "CAN RX read FAIL\r\n");
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
      }
    }
    else
    {
      snprintf(msg, sizeof(msg), "CAN RX empty\r\n");
      HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    }

    counter++;
    heartbeat_ticks++;
    osDelay(500);
  }
}

/* USER CODE BEGIN Header_StartTaskSupervisor */
/**
* @brief Function implementing the task_supervisor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSupervisor */
void StartTaskSupervisor(void *argument)
{
  /* USER CODE BEGIN StartTaskSupervisor */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskSupervisor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

