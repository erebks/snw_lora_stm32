/*!
  * \file      sx1276-board.c
  *
  * \brief     Target board CMWX1ZZABZ-0xx module driver implementation
  *
  * \copyright Revised BSD License, see section \ref LICENSE.
  *
  * \code
  *                ______                              _
  *               / _____)             _              | |
  *              ( (____  _____ ____ _| |_ _____  ____| |__
  *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
  *               _____) ) ____| | | || |_| ____( (___| | | |
  *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
  *              (C)2013-2017 Semtech
  *
  * \endcode
  *
  * \author    Miguel Luis ( Semtech )
  *
  * \author    Gregory Cristian ( Semtech )
  */
/**
  *******************************************************************************
  * @file    cmwx1zzabz-0xx.c
  * @author  MCD Application Team
  * @brief   driver LoRa module murata cmwx1zzabz-078
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "cmwx1zzabz_0xx.h"


#define IRQ_HIGH_PRIORITY  0

/* Private variables ---------------------------------------------------------*/
static const uint32_t RADIO_DIO_EXTI_LINE[RADIO_DIOn] =
{
  RADIO_DIO_0_EXTI_LINE,
  RADIO_DIO_1_EXTI_LINE,
  RADIO_DIO_2_EXTI_LINE,
  RADIO_DIO_3_EXTI_LINE,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_EXTI_LINE,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_EXTI_LINE,
#endif
};

static const uint32_t RADIO_DIO_IT_PRIO [RADIO_DIOn] =
{
  RADIO_DIO_0_IT_PRIO,
  RADIO_DIO_1_IT_PRIO,
  RADIO_DIO_2_IT_PRIO,
  RADIO_DIO_3_IT_PRIO,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_IT_PRIO,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_IT_PRIO,
#endif
};

static const IRQn_Type RADIO_DIO_IRQn [RADIO_DIOn]   =
{
  RADIO_DIO_0_IRQn,
  RADIO_DIO_1_IRQn,
  RADIO_DIO_2_IRQn,
  RADIO_DIO_3_IRQn,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_IRQn,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_IRQn,
#endif
};

static GPIO_TypeDef *RADIO_DIO_PORT[RADIO_DIOn] =
{
  RADIO_DIO_0_PORT,
  RADIO_DIO_1_PORT,
  RADIO_DIO_2_PORT,
  RADIO_DIO_3_PORT,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_PORT,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_PORT,
#endif
};

static const uint16_t RADIO_DIO_PIN[RADIO_DIOn] =
{
  RADIO_DIO_0_PIN,
  RADIO_DIO_1_PIN,
  RADIO_DIO_2_PIN,
  RADIO_DIO_3_PIN,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_PIN,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_PIN,
#endif
};

static const uint32_t RADIO_DIO_MODE[RADIO_DIOn] =
{
  GPIO_MODE_IT_RISING,
  GPIO_MODE_IT_RISING_FALLING,
  GPIO_MODE_IT_RISING,
  GPIO_MODE_IT_RISING,
#if( RADIO_DIOn > 4 )
  GPIO_MODE_IT_RISING,
#endif
#if( RADIO_DIOn > 5 )
  GPIO_MODE_IT_RISING,
#endif
};

/* static */ EXTI_HandleTypeDef hRADIO_DIO_exti[RADIO_DIOn];

/* Private function prototypes -----------------------------------------------*/

static void CMWX1ZZABZ0XX_RADIO_SPI_IoInit(SPI_HandleTypeDef *spiHandle);
static void CMWX1ZZABZ0XX_RADIO_SPI_IoDeInit(void);

/* Exported functions ---------------------------------------------------------*/
uint32_t CMWX1ZZABZ0XX_RADIO_GetWakeUpTime(void)
{
  return  BOARD_WAKEUP_TIME;
}

void CMWX1ZZABZ0XX_RADIO_SetXO(uint8_t state)
{

  if (state == SET)
  {
    HAL_GPIO_WritePin(RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, GPIO_PIN_SET);

    HAL_Delay(BOARD_WAKEUP_TIME);   //start up time of TCXO
  }
  else
  {
    HAL_GPIO_WritePin(RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, GPIO_PIN_RESET);
  }
}
void CMWX1ZZABZ0XX_RADIO_IoInit(void)
{
  GPIO_InitTypeDef initStruct = {0};
  SPI_HandleTypeDef dummy_hspi;

  /* Enable DIO GPIO clock */
  RADIO_DIO_0_GPIO_CLK_ENABLE();
  RADIO_DIO_1_GPIO_CLK_ENABLE();
  RADIO_DIO_2_GPIO_CLK_ENABLE();
  RADIO_DIO_3_GPIO_CLK_ENABLE();
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_GPIO_CLK_ENABLE();
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_GPIO_CLK_ENABLE();
#endif

  /* DIO IO Init */
  initStruct.Pull = GPIO_PULLDOWN;
  initStruct.Speed = GPIO_SPEED_HIGH;

  for (uint32_t i = 0; i < RADIO_DIOn ; i++)
  {
    initStruct.Mode = RADIO_DIO_MODE[i];
    initStruct.Pin = RADIO_DIO_PIN[i];
    HAL_GPIO_Init(RADIO_DIO_PORT[i], &initStruct);
  }


  /* SPI IO Init */
  /* Normally done by the HAL_MSP callback but not for this applic */
  CMWX1ZZABZ0XX_RADIO_SPI_IoInit(&dummy_hspi);

  /* NSS initialization */
  initStruct.Pin = RADIO_NSS_PIN;
  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_PULLUP;

  /* Enable NSS */
  RADIO_NSS_CLK_ENABLE();
  HAL_GPIO_Init(RADIO_NSS_PORT, &initStruct);
  HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_SET);

  /* Antenna IO Init */
  RADIO_TCXO_VCC_CLK_ENABLE();
  RADIO_ANT_SWITCH_CLK_ENABLE_RX();
  RADIO_ANT_SWITCH_CLK_ENABLE_TX_BOOST();
  RADIO_ANT_SWITCH_CLK_ENABLE_TX_RFO();


  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_HIGH;

  initStruct.Pin = RADIO_TCXO_VCC_PIN;
  HAL_GPIO_Init(RADIO_TCXO_VCC_PORT, &initStruct);

  initStruct.Pin = RADIO_ANT_SWITCH_PIN_RX;
  HAL_GPIO_Init(RADIO_ANT_SWITCH_PORT_RX, &initStruct);

  initStruct.Pin = RADIO_ANT_SWITCH_PIN_TX_BOOST;
  HAL_GPIO_Init(RADIO_ANT_SWITCH_PORT_TX_BOOST, &initStruct);

  initStruct.Pin = RADIO_ANT_SWITCH_PIN_TX_RFO;
  HAL_GPIO_Init(RADIO_ANT_SWITCH_PORT_TX_RFO, &initStruct);
}

void CMWX1ZZABZ0XX_RADIO_IoDeInit(void)
{
  GPIO_InitTypeDef initStruct = {0};

  /* DIO IO DeInit */

  initStruct.Pull = GPIO_PULLDOWN;

  for (uint32_t i = 0; i < RADIO_DIOn ; i++)
  {
    initStruct.Mode = RADIO_DIO_MODE[i];
    initStruct.Pin = RADIO_DIO_PIN[i];
    HAL_GPIO_Init(RADIO_DIO_PORT[i], &initStruct);
  }

  /* SPI IO DeInit */
  CMWX1ZZABZ0XX_RADIO_SPI_IoDeInit();

  /* NSS IO DeInit is not done */

  /* Antenna IO DeInit is not done */

  /* Reset IO DeInit is not done */
}

void CMWX1ZZABZ0XX_RADIO_IoIrqInit(DioIrqHandler **irqHandlers)
{
  CRITICAL_SECTION_BEGIN();
  for (uint32_t i = 0; i < RADIO_DIOn ; i++)
  {
    HAL_EXTI_GetHandle(&hRADIO_DIO_exti[i], RADIO_DIO_EXTI_LINE[i]);
    HAL_EXTI_RegisterCallback(&hRADIO_DIO_exti[i], HAL_EXTI_COMMON_CB_ID, irqHandlers[i]);
    HAL_NVIC_SetPriority(RADIO_DIO_IRQn[i], RADIO_DIO_IT_PRIO[i], 0x00);
    HAL_NVIC_EnableIRQ(RADIO_DIO_IRQn[i]);
  }
  CRITICAL_SECTION_END();
}

/*!
 * @brief it points the demanded line in the EXTI_HandleTypeDef TABLE before calling HAL_EXTI_IRQHandler()
 * @param [IN]: DIO extiLine demanded
 * @retval none
 * @note "stm32lyxx_it.c" code generated by MX do not call this function but directly the HAL_EXTI_IRQHandler()
 */
void CMWX1ZZABZ0XX_RADIO_IRQHandler(RADIO_DIO_TypeDef DIO)
{
  HAL_EXTI_IRQHandler(&hRADIO_DIO_exti[DIO]);
}

TxConfig_TypeDef CMWX1ZZABZ0XX_RADIO_GetPaSelect(uint32_t channel)
{
  return CONF_RFO_LP_HP;
}



void CMWX1ZZABZ0XX_RADIO_SetAntSw(RfSw_TypeDef state)
{

  switch (state)
  {
    case RFSW_RX:
    {
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, GPIO_PIN_SET);
      break;
    }
    case RFSW_RFO_LP:
    {
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, GPIO_PIN_SET);
      break;
    }
    case RFSW_RFO_HP:
    {
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, GPIO_PIN_SET);
      break;
    }
    case RFSW_RFO_LF:
    {
      break;
    }
    case RFSW_OFF:
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, GPIO_PIN_RESET);
      break;
    default:
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, GPIO_PIN_RESET);

      break;
  }
}

bool CMWX1ZZABZ0XX_RADIO_CheckRfFrequency(uint32_t frequency)
{
  // Implement check. Currently all frequencies are supported
  return true;
}

void CMWX1ZZABZ0XX_RADIO_Reset(void)
{
  GPIO_InitTypeDef initStruct = { 0 };

  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_HIGH;
  initStruct.Pin = RADIO_RESET_PIN;

  // Set RESET pin to 0
  HAL_GPIO_Init(RADIO_RESET_PORT, &initStruct);
  HAL_GPIO_WritePin(RADIO_RESET_PORT, RADIO_RESET_PIN, GPIO_PIN_RESET);

  // Wait 1 ms
  HAL_Delay(1);

  // Configure RESET as input
  initStruct.Mode = GPIO_NOPULL;
  HAL_GPIO_Init(RADIO_RESET_PORT, &initStruct);

  // Wait 6 ms
  HAL_Delay(6);
}

/* Bus mapping to SPI */

void CMWX1ZZABZ0XX_RADIO_Bus_Init(void)
{
  RADIO_SPI_Init();
}

void CMWX1ZZABZ0XX_RADIO_Bus_deInit(void)
{
  RADIO_SPI_DeInit();
}

/**
  * @brief  Send Receive data via SPI
  * @param  Data to send
  * @retval Received data
  */
uint16_t CMWX1ZZABZ0XX_RADIO_SendRecv(uint16_t txData)
{
  uint16_t rxData ;

  /*RADIO_SPI_SendRecv(( uint8_t * ) &txData, ( uint8_t* ) &rxData, 1);*/
  RADIO_SPI_SendRecv((uint8_t *) &txData, (uint8_t *) &rxData, 1);

  return rxData;
}

void CMWX1ZZABZ0XX_RADIO_ChipSelect(int32_t state)
{
  if (state == 0)
  {
    HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_SET);
  }
}

uint32_t CMWX1ZZABZ0XX_RADIO_GetDio1PinState(void)
{
  return HAL_GPIO_ReadPin(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN);
}
/* Private functions --------------------------------------------------------*/

/**
  * @brief  Initializes SPI MSP.
  * @param  hspi  SPI handler
  * @retval None
*/
static void CMWX1ZZABZ0XX_RADIO_SPI_IoInit(SPI_HandleTypeDef *spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
  /* Enable Peripheral clock */
  RADIO_SPI_SCK_GPIO_CLK_ENABLE();
  RADIO_SPI_MOSI_GPIO_CLK_ENABLE();
  RADIO_SPI_MISO_GPIO_CLK_ENABLE();

  /**SPI1 GPIO Configuration
  PA5     ------> SPI1_SCK
  PA6     ------> SPI1_MISO
  PA7     ------> SPI1_MOSI
    */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Alternate = RADIO_SPI_MOSI_GPIO_AF;
  GPIO_InitStruct.Pin = RADIO_SPI_MOSI_GPIO_PIN;
  HAL_GPIO_Init(RADIO_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Alternate = RADIO_SPI_MISO_GPIO_AF;
  GPIO_InitStruct.Pin = RADIO_SPI_MISO_GPIO_PIN;
  HAL_GPIO_Init(RADIO_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Alternate = RADIO_SPI_SCK_GPIO_AF;
  GPIO_InitStruct.Pin = RADIO_SPI_SCK_GPIO_PIN;
  HAL_GPIO_Init(RADIO_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes SPI MSP.
  * @param  hspi  SPI handler
  * @retval None
  */
static void CMWX1ZZABZ0XX_RADIO_SPI_IoDeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Peripheral clock disable */
  /* no need to call SPI1_CLK_DISABLE() because going in LowPower it gets disabled automatically */

  /* DeInitialize Peripheral GPIOs */
  /* Instead of using HAL_GPIO_DeInit() which set ANALOG mode
     it's preferred to set in OUTPUT_PP mode, with the pins set to 0 */

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Pin = RADIO_SPI_MOSI_GPIO_PIN;
  HAL_GPIO_Init(RADIO_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = RADIO_SPI_MISO_GPIO_PIN;
  HAL_GPIO_Init(RADIO_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = RADIO_SPI_SCK_GPIO_PIN;
  HAL_GPIO_Init(RADIO_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);


  HAL_GPIO_WritePin(RADIO_SPI_MOSI_GPIO_PORT, RADIO_SPI_MOSI_GPIO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RADIO_SPI_MISO_GPIO_PORT, RADIO_SPI_MISO_GPIO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RADIO_SPI_SCK_GPIO_PORT, RADIO_SPI_SCK_GPIO_PIN, GPIO_PIN_RESET);

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
