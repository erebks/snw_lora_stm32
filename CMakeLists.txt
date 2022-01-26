# **************************************************************************** #

# CMakeLists.txt

# **************************************************************************** #

cmake_minimum_required(VERSION 3.22)

# **************************************************************************** #

project(minimal VERSION 1.0.0)

# **************************************************************************** #
# Compiler Debug macros

add_definitions(-DTRACE)
add_definitions(-DDEBUG)
add_definitions(-DSTM32L072xx)
add_definitions(-DCMWX1ZZABZ0XX)

# **************************************************************************** #

set(ENV{ExeName} lorawan_at_slave)

# **************************************************************************** #

set(HEX_FILE ${PROJECT_BINARY_DIR}/$ENV{ExeName}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/$ENV{ExeName}.bin)

# **************************************************************************** #

add_executable($ENV{ExeName})
set_target_properties($ENV{ExeName} PROPERTIES SUFFIX ".elf" LINKER_LANGUAGE C)

# -----------------------------------------------------------------------------
# Add sources and includes

target_include_directories($ENV{ExeName}
  PRIVATE
  inc/
  LoRaWAN/Conf/
  LoRaWAN/Crypto/
  LoRaWAN/LmHandler/
  LoRaWAN/LmHandler/Packages/
  LoRaWAN/Mac/
  LoRaWAN/Mac/Region/
  LoRaWAN/Utilities/

  SubGHz_Phy/
  SubGHz_Phy/sx1276/

  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Inc/
  STM32CubeL0/Drivers/CMSIS/Include/
  STM32CubeL0/Drivers/CMSIS/Core/Include/
  STM32CubeL0/Drivers/CMSIS/Core_A/Include/
  STM32CubeL0/Drivers/CMSIS/Device/ST/STM32L0xx/Include/
  )

target_sources($ENV{ExeName}
  PRIVATE
  src/adc.c
  src/adc_if.c
  src/app_lorawan.c
  src/b_l072z_lrwan1_bus.c
  src/b-l072z-lrwan1.c
  src/cmwx1zzabz_0xx.c
  src/dma.c
  src/lora_app.c
  src/lora_at.c
  src/lora_command.c
  src/lora_info.c
  src/main.c
  src/rtc.c
  src/rtc_if.c
  src/stm32l0xx_hal_msp.c
  src/stm32l0xx_it.c
  src/stm32_adv_trace.c
  src/stm32_lpm_if.c
  src/stm32_mem.c
  src/stm32_seq.c
  src/stm32_tiny_sscanf.c
  src/stm32_tiny_vsnprintf.c
  src/stm32_systime.c
  src/stm32_timer.c
  src/stm32_lpm.c
  src/sys_app.c
  src/sys_debug.c
  src/system_stm32l0xx.c
  src/test_rf.c
  src/usart.c
  src/usart_if.c
  src/startup_stm32l072xx.s

  LoRaWAN/Crypto/cmac.c
  LoRaWAN/Crypto/lorawan_aes.c
  LoRaWAN/Crypto/soft-se.c

  LoRaWAN/LmHandler/LmHandler.c
  LoRaWAN/LmHandler/NvmDataMgmt.c

#  LoRaWAN/LmHandler/Packages/FragDecoder.c
  LoRaWAN/LmHandler/Packages/LmhpClockSync.c
  LoRaWAN/LmHandler/Packages/LmhpCompliance.c
  LoRaWAN/LmHandler/Packages/LmhpFirmwareManagement.c
#  LoRaWAN/LmHandler/Packages/LmhpFragmentation.c
#  LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.c
  LoRaWAN/LmHandler/Packages/LmhpRemoteMcastSetup.c

  LoRaWAN/Mac/LoRaMacAdr.c
  LoRaWAN/Mac/LoRaMac.c
  LoRaWAN/Mac/LoRaMacClassB.c
  LoRaWAN/Mac/LoRaMacCommands.c
  LoRaWAN/Mac/LoRaMacConfirmQueue.c
  LoRaWAN/Mac/LoRaMacCrypto.c
  LoRaWAN/Mac/LoRaMacParser.c
  LoRaWAN/Mac/LoRaMacSerializer.c

  LoRaWAN/Mac/Region/RegionAS923.c
  LoRaWAN/Mac/Region/RegionAU915.c
  LoRaWAN/Mac/Region/RegionBaseUS.c
  LoRaWAN/Mac/Region/Region.c
  LoRaWAN/Mac/Region/RegionCN470.c
  LoRaWAN/Mac/Region/RegionCN779.c
  LoRaWAN/Mac/Region/RegionCommon.c
  LoRaWAN/Mac/Region/RegionEU433.c
  LoRaWAN/Mac/Region/RegionEU868.c
  LoRaWAN/Mac/Region/RegionIN865.c
  LoRaWAN/Mac/Region/RegionKR920.c
  LoRaWAN/Mac/Region/RegionRU864.c
  LoRaWAN/Mac/Region/RegionUS915.c

  LoRaWAN/Utilities/utilities.c

  SubGHz_Phy/sx1276/sx1276.c

  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_comp.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_comp_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cortex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_crc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_crc_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cryp.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cryp_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dac.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dac_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dma.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_exti.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_firewall.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ramfunc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_gpio.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2s.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_irda.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_iwdg.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_lcd.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_lptim.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pcd.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pcd_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rng.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rtc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rtc_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_smartcard.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_smartcard_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_smbus.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_smbus_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_spi.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tsc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_uart.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_uart_ex.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_usart.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_wwdg.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_adc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_comp.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_crc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_crs.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_dac.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_dma.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_exti.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_gpio.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_i2c.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_lptim.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_lpuart.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_pwr.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_rcc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_rng.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_rtc.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_spi.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_tim.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_usart.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_usb.c
  STM32CubeL0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_utils.c

#  STM32CubeL0/Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.c
  )

# **************************************************************************** #

add_custom_command(
    TARGET $ENV{ExeName}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex   $<TARGET_FILE:$ENV{ExeName}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:$ENV{ExeName}> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}"
    )

# **************************************************************************** #

enable_language(C ASM)
