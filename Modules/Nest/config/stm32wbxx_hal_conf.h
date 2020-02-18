#ifndef _STM32WBXX_HAL_CONF_H_INCLUDED_
#define _STM32WBXX_HAL_CONF_H_INCLUDED_

#define HAL_MODULE_ENABLED
//#define HAL_ADC_MODULE_ENABLED
//#define HAL_CRYP_MODULE_ENABLED
//#define HAL_COMP_MODULE_ENABLED
//#define HAL_CRC_MODULE_ENABLED
//#define HAL_HSEM_MODULE_ENABLED
//#define HAL_I2C_MODULE_ENABLED
//#define HAL_IPCC_MODULE_ENABLED
//#define HAL_IRDA_MODULE_ENABLED
//#define HAL_IWDG_MODULE_ENABLED
//#define HAL_LCD_MODULE_ENABLED
//#define HAL_LPTIM_MODULE_ENABLED
//#define HAL_PCD_MODULE_ENABLED
//#define HAL_PKA_MODULE_ENABLED
//#define HAL_QSPI_MODULE_ENABLED
//#define HAL_RNG_MODULE_ENABLED
//#define HAL_RTC_MODULE_ENABLED
//#define HAL_SAI_MODULE_ENABLED
//#define HAL_SMBUS_MODULE_ENABLED
//#define HAL_SMARTCARD_MODULE_ENABLED
//#define HAL_SPI_MODULE_ENABLED
//#define HAL_TIM_MODULE_ENABLED
//#define HAL_TSC_MODULE_ENABLED
//#define HAL_UART_MODULE_ENABLED
//#define HAL_USART_MODULE_ENABLED
//#define HAL_WWDG_MODULE_ENABLED
//#define HAL_EXTI_MODULE_ENABLED
//#define HAL_CORTEX_MODULE_ENABLED
//#define HAL_DMA_MODULE_ENABLED
//#define HAL_FLASH_MODULE_ENABLED
//#define HAL_GPIO_MODULE_ENABLED
//#define HAL_PWR_MODULE_ENABLED
//#define HAL_RCC_MODULE_ENABLED

#define USE_HAL_ADC_REGISTER_CALLBACKS       0u
#define USE_HAL_COMP_REGISTER_CALLBACKS      0u
#define USE_HAL_CRYP_REGISTER_CALLBACKS      0u
#define USE_HAL_I2C_REGISTER_CALLBACKS       0u
#define USE_HAL_IRDA_REGISTER_CALLBACKS      0u
#define USE_HAL_LPTIM_REGISTER_CALLBACKS     0u
#define USE_HAL_PCD_REGISTER_CALLBACKS       0u
#define USE_HAL_PKA_REGISTER_CALLBACKS       0u
#define USE_HAL_QSPI_REGISTER_CALLBACKS      0u
#define USE_HAL_RNG_REGISTER_CALLBACKS       0u
#define USE_HAL_RTC_REGISTER_CALLBACKS       0u
#define USE_HAL_SAI_REGISTER_CALLBACKS       0u
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS 0u
#define USE_HAL_SMBUS_REGISTER_CALLBACKS     0u
#define USE_HAL_SPI_REGISTER_CALLBACKS       0u
#define USE_HAL_TIM_REGISTER_CALLBACKS       0u
#define USE_HAL_TSC_REGISTER_CALLBACKS       0u
#define USE_HAL_UART_REGISTER_CALLBACKS      0u
#define USE_HAL_USART_REGISTER_CALLBACKS     0u
#define USE_HAL_WWDG_REGISTER_CALLBACKS      0u

#ifndef HSE_VALUE
#define HSE_VALUE 32000000U // Value of the External oscillator, Hz
#endif

#ifndef HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT ((uint32_t)100) // Time out for HSE start up, ms
#endif

#ifndef MSI_VALUE
#define MSI_VALUE ((uint32_t)4000000) // Value of the Internal oscillator, Hz
#endif

#ifndef HSI_VALUE
#define HSI_VALUE 16000000U // Value of the Internal oscillator, Hz
#endif

#ifndef LSI1_VALUE
#define LSI1_VALUE ((uint32_t)32000) // LSI1 Typical Value, Hz
#endif

#ifndef LSI2_VALUE
#define LSI2_VALUE ((uint32_t)32000) // LSI2 Typical Value, Hz
#endif

#ifndef LSE_VALUE
#define LSE_VALUE 32768U // Value of the External oscillator, Hz
#endif

#ifndef HSI48_VALUE
#define HSI48_VALUE ((uint32_t)48000000) // Value of the Internal oscillator, Hz
#endif

#ifndef LSE_STARTUP_TIMEOUT
#define LSE_STARTUP_TIMEOUT 5000U // Time out for LSE start up, ms
#endif

#ifndef EXTERNAL_SAI1_CLOCK_VALUE
#define EXTERNAL_SAI1_CLOCK_VALUE ((uint32_t)2097000) // Value of the SAI1 External clock source, Hz
#endif

#define  VDD_VALUE 3300U // Value of VDD, mv
#define  TICK_INT_PRIORITY         0U // tick interrupt priority
#define  USE_RTOS                  0U
#define  PREFETCH_ENABLE           1U
#define  INSTRUCTION_CACHE_ENABLE  1U
#define  DATA_CACHE_ENABLE         1U

//#define USE_FULL_ASSERT 1U

#define USE_SPI_CRC 0U

#include "stm32wbxx_hal_def.h"

#ifdef HAL_DMA_MODULE_ENABLED
#include "stm32wbxx_hal_dma.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "stm32wbxx_hal_adc.h"
#endif

#ifdef HAL_COMP_MODULE_ENABLED
#include "stm32wbxx_hal_comp.h"
#endif

#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32wbxx_hal_cortex.h"
#endif

#ifdef HAL_CRC_MODULE_ENABLED
#include "stm32wbxx_hal_crc.h"
#endif

#ifdef HAL_CRYP_MODULE_ENABLED
#include "stm32wbxx_hal_cryp.h"
#endif

#ifdef HAL_EXTI_MODULE_ENABLED
#include "stm32wbxx_hal_exti.h"
#endif
   
#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32wbxx_hal_flash.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32wbxx_hal_gpio.h"
#endif

#ifdef HAL_HSEM_MODULE_ENABLED
#include "stm32wbxx_hal_hsem.h"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32wbxx_hal_i2c.h"
#endif

#ifdef HAL_IPCC_MODULE_ENABLED
#include "stm32wbxx_hal_ipcc.h"
#endif

#ifdef HAL_IRDA_MODULE_ENABLED
#include "stm32wbxx_hal_irda.h"
#endif

#ifdef HAL_IWDG_MODULE_ENABLED
#include "stm32wbxx_hal_iwdg.h"
#endif

#ifdef HAL_LCD_MODULE_ENABLED
#include "stm32wbxx_hal_lcd.h"
#endif

#ifdef HAL_LPTIM_MODULE_ENABLED
#include "stm32wbxx_hal_lptim.h"
#endif

#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32wbxx_hal_pcd.h"
#endif

#ifdef HAL_PKA_MODULE_ENABLED
#include "stm32wbxx_hal_pka.h"
#endif

#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32wbxx_hal_pwr.h"
#endif

#ifdef HAL_QSPI_MODULE_ENABLED
#include "stm32wbxx_hal_qspi.h"
#endif

#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32wbxx_hal_rcc.h"
#endif

#ifdef HAL_RNG_MODULE_ENABLED
#include "stm32wbxx_hal_rng.h"
#endif
    
#ifdef HAL_RTC_MODULE_ENABLED
#include "stm32wbxx_hal_rtc.h"
#endif

#ifdef HAL_SAI_MODULE_ENABLED
#include "stm32wbxx_hal_sai.h"
#endif

#ifdef HAL_SMARTCARD_MODULE_ENABLED
#include "stm32wbxx_hal_smartcard.h"
#endif

#ifdef HAL_SMBUS_MODULE_ENABLED
#include "stm32wbxx_hal_smbus.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32wbxx_hal_spi.h"
#endif

#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32wbxx_hal_tim.h"
#endif

#ifdef HAL_TSC_MODULE_ENABLED
#include "stm32wbxx_hal_tsc.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
#include "stm32wbxx_hal_uart.h"
#endif

#ifdef HAL_USART_MODULE_ENABLED
#include "stm32wbxx_hal_usart.h"
#endif

#ifdef HAL_WWDG_MODULE_ENABLED
#include "stm32wbxx_hal_wwdg.h"
#endif

#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif

#endif // _STM32WBXX_HAL_CONF_H_INCLUDED_
