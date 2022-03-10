/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    lora_app.c
  * @author  MCD Application Team
  * @brief   Application of the LRWAN Middleware
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "platform.h"
#include "Region.h" /* Needed for LORAWAN_DEFAULT_DATA_RATE */
#include "sys_app.h"
#include "lora_app.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "utilities_def.h"
#include "lora_app_version.h"
#include "lorawan_version.h"
#include "subghz_phy_version.h"
#include "lora_info.h"
#include "LmHandler.h"
#include "lora_command.h"
#include "lora_at.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

static uint32_t snwTimestamp = 0;

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SNW_PACKET_PERIOD_MS ( 5 * 60 * 1000 )
#define SNW_KEY ( (uint32_t) 0xA5A5 )
#define SNW_PHASE_DELTA_MS ( 50 )
#define SNW_DELAY_MIN_MS ( 0 )
#define SNW_DELAY_MAX_MS ( 10 * SNW_PHASE_DELTA_MS )

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  join event callback function
  * @param  joinParams status of join
  */
static void OnJoinRequest(LmHandlerJoinParams_t *joinParams);

/**
  * @brief  tx event callback function
  * @param  params status of last Tx
  */
static void OnTxData(LmHandlerTxParams_t *params);

/**
  * @brief callback when LoRa application has received a frame
  * @param appData data received in the last Rx
  * @param params status of last Rx
  */
static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params);

/*!
 * Will be called each time a Radio IRQ is handled by the MAC layer
 *
 */
static void OnMacProcessNotify(void);

/**
  * @brief  call back when LoRaWan Stack needs update
  */
static void CmdProcessNotify(void);

/* USER CODE BEGIN PFP */

/**
  * @brief  LED Tx timer callback function
  * @param  context ptr of LED context
  */
static void OnTxTimerLedEvent(void *context);

/**
  * @brief  LED Rx timer callback function
  * @param  context ptr of LED context
  */
static void OnRxTimerLedEvent(void *context);

/**
  * @brief  LED Join timer callback function
  * @param  context ptr of LED context
  */
static void OnJoinTimerLedEvent(void *context);

static uint32_t calcWatermark(uint32_t oldData, uint32_t newData, uint32_t key);

static uint32_t calcDelayMS(uint32_t oldDelay, uint8_t phase, uint32_t delta, uint32_t delayMin, uint32_t delayMax);

static void OnSNWTimerEvent(void *context);

static void OnSNWSendTimerEvent(void *context);

/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
/**
  * @brief LoRaWAN handler Callbacks
  */
static LmHandlerCallbacks_t LmHandlerCallbacks =
{
  .GetBatteryLevel =           GetBatteryLevel,
  .GetTemperature =            GetTemperatureLevel,
  .GetUniqueId =               GetUniqueId,
  .GetDevAddr =                GetDevAddr,
  .OnMacProcess =              OnMacProcessNotify,
  .OnJoinRequest =             OnJoinRequest,
  .OnTxData =                  OnTxData,
  .OnRxData =                  OnRxData
};

/**
  * @brief LoRaWAN handler parameters
  */
static LmHandlerParams_t LmHandlerParams =
{
  .ActiveRegion =             ACTIVE_REGION,
  .DefaultClass =             LORAWAN_DEFAULT_CLASS,
  .AdrEnable =                LORAWAN_ADR_STATE,
  .TxDatarate =               LORAWAN_DEFAULT_DATA_RATE,
  .PingPeriodicity =          LORAWAN_DEFAULT_PING_SLOT_PERIODICITY
};

/* USER CODE BEGIN PV */
/**
  * @brief Timer to handle the application Tx Led to toggle
  */
static UTIL_TIMER_Object_t TxLedTimer;

/**
  * @brief Timer to handle the application Rx Led to toggle
  */
static UTIL_TIMER_Object_t RxLedTimer;

/**
  * @brief Timer to handle the application Join Led to toggle
  */
static UTIL_TIMER_Object_t JoinLedTimer;

static UTIL_TIMER_Object_t SNWSendTimer;

static UTIL_TIMER_Object_t SNWTimer;

/* USER CODE END PV */

/* Exported functions ---------------------------------------------------------*/
/* USER CODE BEGIN EF */

/* USER CODE END EF */

void LoRaWAN_Init(void)
{
  CMD_Init(CmdProcessNotify);

  /* USER CODE BEGIN LoRaWAN_Init_1 */

  LED_Init(LED_BLUE);
  LED_Init(LED_RED1);
  LED_Init(LED_RED2);

  /* Get LoRa APP version*/
  APP_LOG(TS_OFF, VLEVEL_M, "APP_VERSION:        V%X.%X.%X\r\n",
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_SUB2_SHIFT));

  /* Get MW LoraWAN info */
  APP_LOG(TS_OFF, VLEVEL_M, "MW_LORAWAN_VERSION: V%X.%X.%X\r\n",
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_SUB2_SHIFT));

  /* Get MW SubGhz_Phy info */
  APP_LOG(TS_OFF, VLEVEL_M, "MW_RADIO_VERSION:   V%X.%X.%X\r\n",
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_SUB2_SHIFT));

  UTIL_TIMER_Create(&TxLedTimer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnTxTimerLedEvent, NULL);
  UTIL_TIMER_Create(&RxLedTimer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnRxTimerLedEvent, NULL);
  UTIL_TIMER_Create(&JoinLedTimer, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnJoinTimerLedEvent, NULL);
  UTIL_TIMER_Create(&SNWTimer, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnSNWTimerEvent, NULL);
  UTIL_TIMER_Create(&SNWSendTimer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnSNWSendTimerEvent, NULL);

  UTIL_TIMER_SetPeriod(&TxLedTimer, 500);
  UTIL_TIMER_SetPeriod(&RxLedTimer, 500);
  UTIL_TIMER_SetPeriod(&JoinLedTimer, 500);
  UTIL_TIMER_SetPeriod(&SNWTimer, SNW_PACKET_PERIOD_MS);

  /* USER CODE END LoRaWAN_Init_1 */

  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LmHandlerProcess), UTIL_SEQ_RFU, LmHandlerProcess);
  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_Vcom), UTIL_SEQ_RFU, CMD_Process);

  /* Init Info table used by LmHandler*/
  LoraInfo_Init();

  /* Init the Lora Stack*/
  LmHandlerInit(&LmHandlerCallbacks);

  LmHandlerConfigure(&LmHandlerParams);

  /* USER CODE BEGIN LoRaWAN_Init_Last */
  UTIL_TIMER_Start(&JoinLedTimer);

  APP_PPRINTF("ATtention command interface\r\n");
  APP_PPRINTF("AT? to list all available functions\r\n");

  // Join network
  LmHandlerJoin(ACTIVATION_TYPE_OTAA);

  UTIL_TIMER_Start(&SNWTimer);
  /* USER CODE END LoRaWAN_Init_Last */
}

/* USER CODE BEGIN PB_Callbacks */

/* USER CODE END PB_Callbacks */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params)
{
  /* USER CODE BEGIN OnRxData_1 */
  if ((appData != NULL) || (params != NULL))
  {
    LED_On(LED_BLUE);

    UTIL_TIMER_Start(&RxLedTimer);

    AT_event_receive(appData, params);
  }
  /* USER CODE END OnRxData_1 */
}

/* USER CODE BEGIN PrFD_LedEvents */
static void OnTxTimerLedEvent(void *context)
{
  LED_Off(LED_RED2);
}

static void OnRxTimerLedEvent(void *context)
{
  LED_Off(LED_BLUE) ;
}

static void OnJoinTimerLedEvent(void *context)
{
  LED_Toggle(LED_RED1) ;
}

/* USER CODE END PrFD_LedEvents */

static void OnTxData(LmHandlerTxParams_t *params)
{
  /* USER CODE BEGIN OnTxData_1 */
  if ((params != NULL))
  {
    /* Process Tx event only if its a mcps response to prevent some internal events (mlme) */
    if (params->IsMcpsConfirm != 0)
    {
      LED_On(LED_RED2) ;
      UTIL_TIMER_Start(&TxLedTimer);
    }
    AT_event_confirm(params);
  }
  /* USER CODE END OnTxData_1 */
}

static void OnJoinRequest(LmHandlerJoinParams_t *joinParams)
{
  /* USER CODE BEGIN OnJoinRequest_1 */
  if (joinParams != NULL)
  {
    if (joinParams->Status == LORAMAC_HANDLER_SUCCESS)
    {
      UTIL_TIMER_Stop(&JoinLedTimer);

      LED_Off(LED_RED1) ;

    }
    AT_event_join(joinParams);
  }
  /* USER CODE END OnJoinRequest_1 */
}

static void CmdProcessNotify(void)
{
  /* USER CODE BEGIN CmdProcessNotify_1 */

  /* USER CODE END CmdProcessNotify_1 */
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_Vcom), 0);
  /* USER CODE BEGIN CmdProcessNotify_2 */

  /* USER CODE END CmdProcessNotify_2 */
}

static void OnMacProcessNotify(void)
{
  /* USER CODE BEGIN OnMacProcessNotify_1 */

  /* USER CODE END OnMacProcessNotify_1 */
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LmHandlerProcess), CFG_SEQ_Prio_0);

  /* USER CODE BEGIN OnMacProcessNotify_2 */

  /* USER CODE END OnMacProcessNotify_2 */
}

static uint32_t calcWatermark(uint32_t oldData, uint32_t newData, uint32_t key)
{
    // "hash" function (XOR)
    // First 13 bit are not chaning with timestamp as data
    uint32_t reg = ( oldData >> 13 ) ^ ( newData >> 13 ) ^ key;

    return reg;
}

static uint32_t calcDelayMS(uint32_t oldDelay, uint8_t phase, uint32_t delta, uint32_t delayMin, uint32_t delayMax)
{
    // Based on previous timestamp calc the next delta after the 10min
    // timer
    uint32_t delay = 0;

    delay = oldDelay + (phase * delta);

    if ( delay > delayMax)
    {
	delay = delayMin;
    }

    return delay;
}

static void OnSNWSendTimerEvent(void *context)
{
    LmHandlerErrorStatus_t lmhStatus = LORAMAC_HANDLER_ERROR;
    uint8_t payload[4] = {0x1, 0x2, 0x3, 0x4};
    LmHandlerAppData_t AppData = { 0, 0, payload };
    UTIL_TIMER_Time_t nextTxIn = 0;
    LmHandlerMsgTypes_t isTxConfirmed = LORAMAC_HANDLER_UNCONFIRMED_MSG;

    payload[0] = (uint8_t) snwTimestamp;
    payload[1] = (uint8_t) (snwTimestamp >> 8);
    payload[2] = (uint8_t) (snwTimestamp >> 16);
    payload[3] = (uint8_t) (snwTimestamp >> 24);

    AppData.BufferSize = 4;
    AppData.Port = 1;
    LmHandlerSend(&AppData, isTxConfirmed, &nextTxIn, 0);

    APP_PPRINTF("TS: %d sent! @%d\r\n", snwTimestamp, SysTimeToMs(SysTimeGet()));
}

static void OnSNWTimerEvent(void *context)
{
    static uint32_t oldTimestamp = 0;
    static uint32_t oldDelay = 0;
    static uint8_t oldPhase = 0;
    uint32_t watermark = 0;
    uint32_t delay = 0;
    uint8_t phase = 0;

    snwTimestamp = SysTimeToMs(SysTimeGet());
    APP_PPRINTF("\r\n################\r\n");
    APP_PPRINTF("SNW TIMER TS: %d\r\n", snwTimestamp);

    // Calculate watermark, phase and delay
    watermark = calcWatermark(oldTimestamp, snwTimestamp, SNW_KEY);
    phase = ((oldPhase ^ (watermark & 0x1)) & 0x1);

    delay = calcDelayMS(oldDelay, phase, SNW_PHASE_DELTA_MS, SNW_DELAY_MIN_MS, SNW_DELAY_MAX_MS);

    APP_PPRINTF("Watermark: 0x%x (%x), Phase: %x, Delay: %d\r\n", watermark, watermark & 0x1, phase, delay);

    if (delay == 0)
    {
	// Send right away
	OnSNWSendTimerEvent( (void *) NULL );
    }
    else
    {
	// Set period and start timer
	UTIL_TIMER_SetPeriod(&SNWSendTimer, delay);
	UTIL_TIMER_Start(&SNWSendTimer);
    }

    oldTimestamp = snwTimestamp;
    oldDelay = delay;
    oldPhase = phase;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
