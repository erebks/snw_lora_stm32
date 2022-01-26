/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32l0xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sys_debug.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
/* when fast wake up is enabled, the mcu wakes up in ~20us  * and
 * does not wait for the VREFINT to be settled. THis is ok for
 * most of the case except when adc must be used in this case before
 *starting the adc, you must make sure VREFINT is settled*/
#define ENABLE_FAST_WAKEUP
/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();

  /* Disable the Power Voltage Detector */
  HAL_PWR_DisablePVD();

  /* Enables the Ultra Low Power mode */
  HAL_PWREx_EnableUltraLowPower();

  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

  /*In debug mode, e.g. when DBGMCU is activated, Arm core has always clocks
   * And will not wait that the FLACH is ready to be read. It can miss in this
   * case the first instruction. To overcome this issue, the flash remain clcoked during sleep mode
   */
  DBG(__HAL_FLASH_SLEEP_POWERDOWN_DISABLE(););

#ifdef ENABLE_FAST_WAKEUP
  /*Enable fast wakeUp*/
  HAL_PWREx_EnableFastWakeUp();
#else
  HAL_PWREx_DisableFastWakeUp();
#endif

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
