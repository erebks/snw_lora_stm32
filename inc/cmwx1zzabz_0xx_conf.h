/**
  ******************************************************************************
  * @file              : cmwx1zzabz_0xx_conf.h
  * @brief             : This file provides code for the configuration
  *                      of the shield instances (pin mapping).
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

#ifndef __CMWX1ZZABZ_0XX_CONF_H__
#define __CMWX1ZZABZ_0XX_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"
#include "radio_conf.h"

/* Defines ---------------------------------------------------------------*/
#define RADIO_DIO_0_IT_PRIO                        0
#define RADIO_DIO_1_IT_PRIO                        0
#define RADIO_DIO_2_IT_PRIO                        0
#define RADIO_DIO_3_IT_PRIO                        0

/* Daughter board Pin mapping --------------------------------------------*/

/*  SPI functions redefinition */
#define RADIO_SPI_Init                   BSP_SPI1_Init
#define RADIO_SPI_DeInit                 BSP_SPI1_DeInit
#define RADIO_SPI_SendRecv               BSP_SPI1_SendRecv

#define RADIO_SPI_SCK_GPIO_PIN           BUS_SPI1_SCK_GPIO_PIN
#define RADIO_SPI_MISO_GPIO_PIN          BUS_SPI1_MISO_GPIO_PIN
#define RADIO_SPI_MOSI_GPIO_PIN          BUS_SPI1_MOSI_GPIO_PIN
#define RADIO_SPI_SCK_GPIO_PORT          BUS_SPI1_SCK_GPIO_PORT
#define RADIO_SPI_MISO_GPIO_PORT         BUS_SPI1_MISO_GPIO_PORT
#define RADIO_SPI_MOSI_GPIO_PORT         BUS_SPI1_MOSI_GPIO_PORT
#define RADIO_SPI_SCK_GPIO_AF            BUS_SPI1_SCK_GPIO_AF
#define RADIO_SPI_MOSI_GPIO_AF           BUS_SPI1_MOSI_GPIO_AF
#define RADIO_SPI_MISO_GPIO_AF           BUS_SPI1_MISO_GPIO_AF
#define RADIO_SPI_SCK_GPIO_CLK_ENABLE()  BUS_SPI1_SCK_GPIO_CLK_ENABLE()
#define RADIO_SPI_MOSI_GPIO_CLK_ENABLE() BUS_SPI1_MOSI_GPIO_CLK_ENABLE()
#define RADIO_SPI_MISO_GPIO_CLK_ENABLE() BUS_SPI1_MISO_GPIO_CLK_ENABLE()

/* SPIx Bus Pin mapping */
#define RADIO_NSS_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define RADIO_NSS_PORT              GPIOA
#define RADIO_NSS_PIN               GPIO_PIN_15

/* LORA I/O pin mapping */
#define RADIO_RESET_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_RESET_PORT            GPIOC
#define RADIO_RESET_PIN             GPIO_PIN_0

#define RADIO_DIOn                               4U

#define RADIO_DIO_0_PORT                  GPIOB
#define RADIO_DIO_0_PIN                   GPIO_PIN_4
#define RADIO_DIO_0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_DIO_0_EXTI_LINE             EXTI_LINE_4
#define RADIO_DIO_0_IRQn                  EXTI4_15_IRQn
#define H_EXTI_4        hRADIO_DIO_exti[0]

#define RADIO_DIO_1_PORT                  GPIOB
#define RADIO_DIO_1_PIN                   GPIO_PIN_1
#define RADIO_DIO_1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_DIO_1_EXTI_LINE             EXTI_LINE_1
#define RADIO_DIO_1_IRQn                  EXTI0_1_IRQn
#define H_EXTI_1        hRADIO_DIO_exti[1]

#define RADIO_DIO_2_PORT        GPIOB
#define RADIO_DIO_2_PIN         GPIO_PIN_0
#define RADIO_DIO_2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_DIO_2_EXTI_LINE             EXTI_LINE_0
#define RADIO_DIO_2_IRQn                  EXTI0_1_IRQn
#define H_EXTI_0        hRADIO_DIO_exti[2]

#define RADIO_DIO_3_PORT        GPIOC
#define RADIO_DIO_3_PIN         GPIO_PIN_13
#define RADIO_DIO_3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_DIO_3_EXTI_LINE             EXTI_LINE_13
#define RADIO_DIO_3_IRQn                  EXTI4_15_IRQn
#define H_EXTI_13        hRADIO_DIO_exti[3]

#define RADIO_TCXO_VCC_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define RADIO_TCXO_VCC_PORT       GPIOA
#define RADIO_TCXO_VCC_PIN        GPIO_PIN_12

#define RADIO_ANT_SWITCH_CLK_ENABLE_RX()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define RADIO_ANT_SWITCH_PORT_RX       GPIOA
#define RADIO_ANT_SWITCH_PIN_RX        GPIO_PIN_1

#define RADIO_ANT_SWITCH_CLK_ENABLE_TX_BOOST()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_ANT_SWITCH_PORT_TX_BOOST       GPIOC
#define RADIO_ANT_SWITCH_PIN_TX_BOOST        GPIO_PIN_1

#define RADIO_ANT_SWITCH_CLK_ENABLE_TX_RFO()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define RADIO_ANT_SWITCH_PORT_TX_RFO       GPIOC
#define RADIO_ANT_SWITCH_PIN_TX_RFO        GPIO_PIN_2

#ifdef __cplusplus
}
#endif
#endif /* __CMWX1ZZABZ_0XX_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
