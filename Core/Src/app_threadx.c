/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
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
/* USER CODE BEGIN PV */
TX_THREAD              EventFlagThreadOne;
TX_THREAD              EventFlagThreadTwo;
TX_EVENT_FLAGS_GROUP   Event_flags_0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void    EventFlagThreadOne_Entry(ULONG thread_input);
void    EventFlagThreadTwo_Entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer;

  /* Allocate the stack for EventFlagThreadOne.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create EventFlagThreadOne.  */
  if (tx_thread_create(&EventFlagThreadOne, "EventFlag Thread One",
                   EventFlagThreadOne_Entry, 0, pointer, APP_STACK_SIZE,
             EVENT_FLAG_THREAD_ONE_PRIO, EVENT_FLAG_THREAD_ONE_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for EventFlagThreadTwo.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create EventFlagThreadTwo.  */
  if (tx_thread_create(&EventFlagThreadTwo, "EventFlag Thread Two",
                   EventFlagThreadTwo_Entry, 0, pointer, APP_STACK_SIZE,
             EVENT_FLAG_THREAD_TWO_PRIO, EVENT_FLAG_THREAD_TWO_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Create the event flags group used by threads EventFlagThreadOne and EventFlagThreadTwo. */
  if (tx_event_flags_create(&Event_flags_0, "event flags 0"))
  {
    ret = TX_THREAD_ERROR;
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing the EventFlagThreadOne thread.
  * @param  thread_input: Not used
  * @retval None
  */
void EventFlagThreadOne_Entry(ULONG thread_input)
{
  UINT status;
  (void) thread_input;
  /* Infinite loop */
  while(1)
  {
    /* Sleep for 1000 ticks */
    tx_thread_sleep(1000);

    /* Set event flag 0 to wake up thread Two */
    status = tx_event_flags_set(&Event_flags_0, 0x1, TX_OR);

    /* Check status */
    if (status != TX_SUCCESS)
      break;
  }
}

/**
  * @brief  Function implementing the EventFlagThreadTwo thread.
  * @param  thread_input: Not used
  * @retval None
  */
void EventFlagThreadTwo_Entry(ULONG thread_input)
{
  UINT status;
  ULONG actual_flags;
  (void) thread_input;
  /* Infinite loop */
  while(1)
  {
      /* Wait for event flag 0 */
    status = tx_event_flags_get(&Event_flags_0, 0x1, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

    /* Check status. */
    if ((status != TX_SUCCESS) || (actual_flags != 0x1)){
      break;
    }else{
      HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
    }
  }
}
/* USER CODE END 1 */
