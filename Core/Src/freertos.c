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
#include "pump_drv.h"
#include "operating_modes.h"
#include "thermal_supervision.h"
#include "control_logic.h"
#include "fault_manager.h"
#include "can_msg_codec.h"
#include "can_status_mapper.h"


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

static const char *mcp9808_status_to_string(Mcp9808Status status)
{
    const char *status_str = "UNKNOWN";

    switch (status)
    {
        case MCP9808_STATUS_OK:
            status_str = "OK";
            break;

        case MCP9808_STATUS_ERR_INIT:
            status_str = "ERR_INIT";
            break;

        case MCP9808_STATUS_ERR_PARAM:
            status_str = "ERR_PARAM";
            break;

        case MCP9808_STATUS_ERR_I2C:
            status_str = "ERR_I2C";
            break;

        case MCP9808_STATUS_ERR_TIMEOUT:
            status_str = "ERR_TIMEOUT";
            break;

        case MCP9808_STATUS_ERR_INVALID_TEMP:
            status_str = "ERR_INVALID_TEMP";
            break;

        case MCP9808_STATUS_ERR_FAULT:
            status_str = "ERR_FAULT";
            break;

        default:
            status_str = "UNKNOWN";
            break;
    }

    return status_str;
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
    /* USER CODE BEGIN StartTaskSense */
    int16_t temp_x10;
    char temp_msg[128];

    Mcp9808Status mcp9808_status;
    Mcp9808Status recover_status;
    Mcp9808Status state_status;
    Mcp9808State mcp9808_state;
    Mcp9808Ctx mcp9808_ctx;
    ThermalSupervisionState supervision_state = THERMAL_SUPERVISION_STATE_INVALID_MEASUREMENT;

    mcp9808_status = mcp9808_drv_init(&mcp9808_ctx, 0x18U);
    if (mcp9808_status != MCP9808_STATUS_OK)
    {
        (void)snprintf(temp_msg,
                       sizeof(temp_msg),
                       "MCP9808 init FAIL status=%s\r\n",
                       mcp9808_status_to_string(mcp9808_status));

        HAL_UART_Transmit(&huart3,
                          (uint8_t *)temp_msg,
                          strlen(temp_msg),
                          HAL_MAX_DELAY);

        for (;;)
        {
            thermal_supervision_set_invalid_measurement();
            mcp9808_status = mcp9808_drv_init(&mcp9808_ctx, 0x18U);
             if (mcp9808_status == MCP9808_STATUS_OK)
             {
                HAL_UART_Transmit(&huart3,
                                    (uint8_t *)"MCP9808 init recovery OK\r\n",
                                    strlen("MCP9808 init recovery OK\r\n"),
                                    HAL_MAX_DELAY);
                break;
             }
            osDelay(1000U);
        }
    }

    for (;;)
    {
        mcp9808_status = mcp9808_drv_get_temperature_x10(&mcp9808_ctx, &temp_x10);

        if (mcp9808_status == MCP9808_STATUS_OK)
        {
            supervision_state = thermal_supervision_update_valid_temperature(temp_x10);

            /* Clear fault if it's active */
            if (fault_manager_is_fault_active(FAULT_ID_F001_SENSOR_COMMUNICATION))
            {
                fault_manager_clear_fault(FAULT_ID_F001_SENSOR_COMMUNICATION);

                HAL_UART_Transmit(
                    &huart3,
                    (uint8_t *)"FAULT F001 cleared\r\n",
                    strlen("FAULT F001 cleared\r\n"),
                    HAL_MAX_DELAY);
            }

            (void)snprintf(temp_msg,
                           sizeof(temp_msg),
                           "TEMP=%d.%dC state=%d\r\n",
                           temp_x10 / 10,
                           (temp_x10 < 0) ? (-temp_x10 % 10) : (temp_x10 % 10),
                           (int)supervision_state);

            HAL_UART_Transmit(&huart3,
                              (uint8_t *)temp_msg,
                              strlen(temp_msg),
                              HAL_MAX_DELAY);
        }
        else
        {
            thermal_supervision_set_invalid_measurement();

            (void)snprintf(temp_msg,
                           sizeof(temp_msg),
                           "TEMP read FAIL status=%s\r\n",
                           mcp9808_status_to_string(mcp9808_status));

            HAL_UART_Transmit(&huart3,
                              (uint8_t *)temp_msg,
                              strlen(temp_msg),
                              HAL_MAX_DELAY);
        }

        state_status = mcp9808_drv_get_state(&mcp9808_ctx, &mcp9808_state);
        if (state_status == MCP9808_STATUS_OK && mcp9808_state == MCP9808_STATE_FAULT)
        {
            /* 3 consecutive sensor acquisition failures */
            if (!fault_manager_is_fault_active(FAULT_ID_F001_SENSOR_COMMUNICATION))
            {
                fault_manager_raise_fault(FAULT_ID_F001_SENSOR_COMMUNICATION);

                HAL_UART_Transmit(
                    &huart3,
                    (uint8_t *)"FAULT F001 raised\r\n",
                    strlen("FAULT F001 raised\r\n"),
                    HAL_MAX_DELAY);
            }

            HAL_UART_Transmit(
                &huart3,
                (uint8_t *)"MCP9808 FAULT -> recover request\r\n",
                strlen("MCP9808 FAULT -> recover request\r\n"),
                HAL_MAX_DELAY);

            recover_status = mcp9808_drv_recover(&mcp9808_ctx);

            (void)snprintf(temp_msg,
                            sizeof(temp_msg),
                            "MCP9808 recover status=%s\r\n",
                            mcp9808_status_to_string(recover_status));

            HAL_UART_Transmit(&huart3,
                              (uint8_t *)temp_msg,
                              strlen(temp_msg),
                              HAL_MAX_DELAY);
        }


        osDelay(1000U);
    }
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
  int16_t last_valid_temp_x10;
  char ctrl_msg[128];

  ThermalSupervisionState supervision_state;
  PumpCtx pump_ctx;
  PumpStatus pump_status;
  PumpControlResult control_result =
  {
      .command = PUMP_COMMAND_OFF,
      .region = PUMP_CONTROL_REGION_INVALID_MEASUREMENT
  };
  PumpCommand previous_pump_command = PUMP_COMMAND_OFF;

  pump_status = pump_drv_init(&pump_ctx, PUMP_CMD_GPIO_Port, PUMP_CMD_Pin);

  if (pump_status != PUMP_STATUS_OK)
  {
    HAL_UART_Transmit(&huart3,
                      (uint8_t *)"Pump init FAIL\r\n",
                      strlen("Pump init FAIL\r\n"),
                      HAL_MAX_DELAY);

    for (;;)
    {
      osDelay(1000U);
    }
  }

  pump_status = pump_drv_set_off(&pump_ctx);
  if (pump_status != PUMP_STATUS_OK)
  {
    HAL_UART_Transmit(&huart3,
                      (uint8_t *)"Pump force OFF init FAIL\r\n",
                      strlen("Pump force OFF init FAIL\r\n"),
                      HAL_MAX_DELAY);
  }

  for (;;)
  {
    supervision_state = thermal_supervision_get_last_valid_temperature(&last_valid_temp_x10);

    /* Only control_logic API is authorized to modify the pump command */
    control_result  = pump_control_logic_update(supervision_state, last_valid_temp_x10);

    if (control_result.command == PUMP_COMMAND_ON)
    {
        pump_status = pump_drv_set_on(&pump_ctx);
    }
    else
    {
        pump_status = pump_drv_set_off(&pump_ctx);
    }

    if (pump_status != PUMP_STATUS_OK)
    {
      (void)snprintf(ctrl_msg,
                     sizeof(ctrl_msg),
                     "Pump command apply FAIL status=%d\r\n",
                     (int)pump_status);

      HAL_UART_Transmit(&huart3,
                        (uint8_t *)ctrl_msg,
                        strlen(ctrl_msg),
                        HAL_MAX_DELAY);
    }
    else if (control_result.command != previous_pump_command)
    {
      (void)snprintf(ctrl_msg,
                     sizeof(ctrl_msg),
                     "Pump command -> %s\r\n",
                     (control_result.command == PUMP_COMMAND_ON) ? "ON" : "OFF");

      HAL_UART_Transmit(&huart3,
                        (uint8_t *)ctrl_msg,
                        strlen(ctrl_msg),
                        HAL_MAX_DELAY);

      previous_pump_command = control_result.command;
    }

    osDelay(1000U);
  }
  /* USER CODE END StartTaskControl */
}

/* USER CODE BEGIN Header_StartTaskDiag */
static HAL_StatusTypeDef task_diag_configure_can_filter(void)
{
    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0U;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0U;
    filter.FilterIdLow = 0U;
    filter.FilterMaskIdHigh = 0U;
    filter.FilterMaskIdLow = 0U;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14U;

    return HAL_CAN_ConfigFilter(&hcan1, &filter);
}

void StartTaskDiag(void *argument)
{
    /* USER CODE BEGIN StartTaskDiag */

    CanMsgCodecStatus codec_status;
    CanStatusMapperStatus mapper_status;
    HAL_StatusTypeDef can_status;
    HAL_StatusTypeDef hal_status;

    PumpControlResult control_result;
    CanThermalState can_thermal_state;

    int16_t temperature_x10 = 0;

    uint8_t thermal_payload[CAN_MSG_THERMAL_STATUS_DLC] = {0U};
    uint8_t rx_payload[CAN_MSG_THERMAL_STATUS_DLC] = {0U};

    char diag_msg[128];

    CAN_TxHeaderTypeDef tx_header = {0};
    CAN_RxHeaderTypeDef rx_header = {0};

    uint32_t tx_mailbox = 0U;

    tx_header.StdId = CAN_MSG_THERMAL_STATUS_ID;
    tx_header.ExtId = 0U;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = CAN_MSG_THERMAL_STATUS_DLC;
    tx_header.TransmitGlobalTime = DISABLE;

    hal_status = task_diag_configure_can_filter();

    if (hal_status != HAL_OK)
    {
        (void)snprintf(
            diag_msg,
            sizeof(diag_msg),
            "CAN filter config FAIL status=%d\r\n",
            (int)hal_status);

        HAL_UART_Transmit(
            &huart3,
            (uint8_t *)diag_msg,
            strlen(diag_msg),
            HAL_MAX_DELAY);

        for (;;)
        {
            osDelay(1000U);
        }
    }

    HAL_UART_Transmit(
        &huart3,
        (uint8_t *)"CAN filter config OK\r\n",
        strlen("CAN filter config OK\r\n"),
        HAL_MAX_DELAY);

    hal_status = HAL_CAN_Start(&hcan1);

    if (hal_status != HAL_OK)
    {
        (void)snprintf(
            diag_msg,
            sizeof(diag_msg),
            "CAN start FAIL status=%d\r\n",
            (int)hal_status);

        HAL_UART_Transmit(
            &huart3,
            (uint8_t *)diag_msg,
            strlen(diag_msg),
            HAL_MAX_DELAY);

        for (;;)
        {
            osDelay(1000U);
        }
    }

    HAL_UART_Transmit(
        &huart3,
        (uint8_t *)"CAN start OK\r\n",
        strlen("CAN start OK\r\n"),
        HAL_MAX_DELAY);

    for (;;)
    {
        (void)thermal_supervision_get_last_valid_temperature(
            &temperature_x10);

        control_result =
            pump_control_logic_get_current_result();

        mapper_status =
            can_status_mapper_map_thermal_state(
                control_result.region,
                &can_thermal_state);

        if (mapper_status == CAN_STATUS_MAPPER_STATUS_OK)
        {
            codec_status =
                can_msg_codec_build_thermal_status(
                    thermal_payload,
                    can_thermal_state,
                    temperature_x10,
                    control_result.command);
        }
        else
        {
            codec_status = CAN_MSG_CODEC_STATUS_ERR_VALUE;
        }

        if (codec_status == CAN_MSG_CODEC_STATUS_OK)
        {
            can_status =
                bsp_can_send(
                    &hcan1,
                    &tx_header,
                    thermal_payload,
                    &tx_mailbox);

            if (can_status == HAL_OK)
            {
                HAL_UART_Transmit(
                    &huart3,
                    (uint8_t *)"CAN thermal status queued\r\n",
                    strlen("CAN thermal status queued\r\n"),
                    HAL_MAX_DELAY);

                osDelay(10U);

                if (HAL_CAN_GetRxFifoFillLevel(
                        &hcan1,
                        CAN_RX_FIFO0) > 0U)
                {
                    if (HAL_CAN_GetRxMessage(
                            &hcan1,
                            CAN_RX_FIFO0,
                            &rx_header,
                            rx_payload) == HAL_OK)
                    {
                        (void)snprintf(
                            diag_msg,
                            sizeof(diag_msg),
                            "RX id=0x%03lX dlc=%lu data="
                            "%02X %02X %02X %02X "
                            "%02X %02X %02X %02X\r\n",
                            rx_header.StdId,
                            rx_header.DLC,
                            rx_payload[0],
                            rx_payload[1],
                            rx_payload[2],
                            rx_payload[3],
                            rx_payload[4],
                            rx_payload[5],
                            rx_payload[6],
                            rx_payload[7]);

                        HAL_UART_Transmit(
                            &huart3,
                            (uint8_t *)diag_msg,
                            strlen(diag_msg),
                            HAL_MAX_DELAY);
                    }
                    else
                    {
                        HAL_UART_Transmit(
                            &huart3,
                            (uint8_t *)"CAN RX read FAIL\r\n",
                            strlen("CAN RX read FAIL\r\n"),
                            HAL_MAX_DELAY);
                    }
                }
                else
                {
                    HAL_UART_Transmit(
                        &huart3,
                        (uint8_t *)"CAN RX FIFO empty\r\n",
                        strlen("CAN RX FIFO empty\r\n"),
                        HAL_MAX_DELAY);
                }
            }
            else
            {
                (void)snprintf(
                    diag_msg,
                    sizeof(diag_msg),
                    "CAN thermal status TX FAIL status=%d\r\n",
                    (int)can_status);

                HAL_UART_Transmit(
                    &huart3,
                    (uint8_t *)diag_msg,
                    strlen(diag_msg),
                    HAL_MAX_DELAY);
            }
        }
        else
        {
            (void)snprintf(
                diag_msg,
                sizeof(diag_msg),
                "CAN thermal status build FAIL status=%d\r\n",
                (int)codec_status);

            HAL_UART_Transmit(
                &huart3,
                (uint8_t *)diag_msg,
                strlen(diag_msg),
                HAL_MAX_DELAY);
        }

        osDelay(500U);
    }

    /* USER CODE END StartTaskDiag */
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
  

  /* Infinite loop */
  for(;;)
  {
    

    osDelay(1000);
  }
  /* USER CODE END StartTaskSupervisor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

