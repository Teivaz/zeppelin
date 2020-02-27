/**
 * Boot file for the STM32WB55xx SoC
 * Created by Teivaz
 * 16.02.2020
 */

#ifndef _BOOT_H_INCLUDED_
#define _BOOT_H_INCLUDED_

/**
 * Priority: -3
 * Type of Priority: Fixed
 * 
 * Reset
 **/
void Reset_Handler(void);

/**
 * Priority: -2
 * Type of Priority: Fixed
 * 
 * Non maskable interrupt HSE CSS, Flash ECC, and SRAM2 parity
 **/
void NMI_Handler(void);

/**
 * Priority: -1
 * Type of Priority: Fixed
 * 
 * All classes of fault
 **/
void HardFault_Handler(void);

/**
 * Priority: 0
 * Type of Priority: Settable
 * 
 * Memory manager
 **/
void MemManager_Handler(void);

/**
 * Priority: 1
 * Type of Priority: Settable
 * 
 * Pre-fetch fault, memory access fault
 **/
void BusFault_Handler(void);

/**
 * Priority: 2
 * Type of Priority: Settable
 * 
 * Undefined instruction or illegal state
 **/
void UsageFault_Handler(void);

/**
 * Priority: 3
 * Type of Priority: Settable
 * 
 * System service can via SWI instruction
 **/
void SVCall_Handler(void);

/**
 * Priority: 4
 * Type of Priority: Settable
 * 
 * Debug monitor
 **/
void Debug_Handler(void);

/**
 * Priority: 5
 * Type of Priority: Settable
 * 
 * Pendable request for system service
 **/
void PendSV_Handler(void);

/**
 * Priority: 6
 * Type of Priority: Settable
 * 
 * System tick timer
 **/
void SysTick_Handler(void);

/**
 * Priority: 7
 * Type of Priority: Settable
 * 
 * Window watchdog early wakeup
 **/
void WWDG_Handler(void);

/**
 * Priority: 8
 * Type of Priority: Settable
 * 
 * Interrupts: PVD, PVM[2,0]
 * 
 * PVD through EXTI[16] (C1IMR2[20])
 * PVM[0] through EXTI[31] (C1IMR2[16])
 * PVM[2] through EXTI[33] (C1IMR2[18])
 **/
void PVD_Handler(void);

/**
 * Priority: 9
 * Type of Priority: Settable
 * 
 * Tamper, TimeStamp, LSECSS interrupt through EXTI[18]
 **/
void TAMP_Handler(void);

/**
 * Priority: 10
 * Type of Priority: Settable
 * 
 * RTC wakeup interrupt through EXTI[19]
 **/
void RTC_WKUP_Handler(void);

/**
 * Priority: 11
 * Type of Priority: Settable
 * 
 * Flash memory global interrupt and Flash memory ECC single error interrupt
 **/
void Flash_Handler(void);

/**
 * Priority: 12
 * Type of Priority: Settable
 * 
 * RCC global interrupt
 **/
void RCC_Handler(void);

/**
 * Priority: 13
 * Type of Priority: Settable
 * 
 * EXTI line 0 interrupt through EXTI[0]
 **/
void EXTI0_Handler(void);

/**
 * Priority: 14
 * Type of Priority: Settable
 * 
 * EXTI line 1 interrupt through EXTI[1]
 **/
void EXTI1_Handler(void);

/**
 * Priority: 15
 * Type of Priority: Settable
 * 
 * EXTI line 2 interrupt through EXTI[2]
 **/
void EXTI2_Handler(void);

/**
 * Priority: 16
 * Type of Priority: Settable
 * 
 * EXTI line 3 interrupt through EXTI[3]
 **/
void EXTI3_Handler(void);

/**
 * Priority: 17
 * Type of Priority: Settable
 * 
 * EXTI line 4 interrupt through EXTI[4]
 **/
void EXTI4_Handler(void);

/**
 * Priority: 18
 * Type of Priority: Settable
 * 
 * DMA1 channel 1 interrupt
 **/
void DMA1_CH1_Handler(void);

/**
 * Priority: 19
 * Type of Priority: Settable
 * 
 * DMA1 channel 2 interrupt
 **/
void DMA1_CH2_Handler(void);

/**
 * Priority: 20
 * Type of Priority: Settable
 * 
 * DMA1 channel 3 interrupt
 **/
void DMA1_CH3_Handler(void);

/**
 * Priority: 21
 * Type of Priority: Settable
 * 
 * DMA1 channel 4 interrupt
 **/
void DMA1_CH4_Handler(void);

/**
 * Priority: 22
 * Type of Priority: Settable
 * 
 * DMA1 channel 5 interrupt
 **/
void DMA1_CH5_Handler(void);

/**
 * Priority: 23
 * Type of Priority: Settable
 * 
 * DMA1 channel 6 interrupt
 **/
void DMA1_CH6_Handler(void);

/**
 * Priority: 24
 * Type of Priority: Settable
 * 
 * DMA1 channel 7 interrupt
 **/
void DMA1_CH7_Handler(void);

/**
 * Priority: 25
 * Type of Priority: Settable
 * 
 * ADC1 global interrupt
 **/
void ADC1_Handler(void);

/**
 * Priority: 26
 * Type of Priority: Settable
 * 
 * USB high priority interrupt
 **/
void USB_HP_Handler(void);

/**
 * Priority: 27
 * Type of Priority: Settable
 * 
 * USB low priority interrupt (including USB wakeup)
 **/
void USB_LP_Handler(void);

/**
 * Priority: 28
 * Type of Priority: Settable
 * 
 * Interrupts: C2SEV, PWR_C2H
 * 
 * CPU2 SEV through EXTI
 * PWR CPU2 HOLD wakeup interrupt
 **/
void C2SEV_Handler(void);

/**
 * Priority: 29
 * Type of Priority: Settable
 * 
 * COMP2 and COMP1 interrupt through EXTI[21:20]
 **/
void COMP_Handler(void);

/**
 * Priority: 30
 * Type of Priority: Settable
 * 
 * EXTI line [9:5] interrupt through EXTI[9:5] (C1IMR1[25:21])
 **/
void EXTI_9_5_Handler(void);

/**
 * Priority: 31
 * Type of Priority: Settable
 * 
 * Timer 1 break interrupt
 **/
void TIM1_BRK_Handler(void);

/**
 * Priority: 32
 * Type of Priority: Settable
 * 
 * Interrupts: TIM1_UP, TIM16
 * 
 * Timer 1 Update (C1IMR1[13])
 * Timer 16 global interrupt (C1IMR1[14])
 **/
void TIM1_UP_Handler(void);

/**
 * Priority: 33
 * Type of Priority: Settable
 * 
 * Interrupts: TIM1_TRG_COM, TIM17
 * 
 * Timer 1 trigger and communication (C1IMR1[13])
 * Timer 17 global interrupt (C1IMR1[15])
 **/
void TIM1_TRG_COM_Handler(void);

/**
 * Priority: 34
 * Type of Priority: Settable
 * 
 * Timer 1 capture compare interrupt
 **/
void TIM1_CC_Handler(void);

/**
 * Priority: 35
 * Type of Priority: Settable
 * 
 * Timer 2 global interrupt
 **/
void TIM2_Handler(void);

/**
 * Priority: 36
 * Type of Priority: Settable
 * 
 * Private key accelerator interrupt
 **/
void PKA_Handler(void);

/**
 * Priority: 37
 * Type of Priority: Settable
 * 
 * I2C1 event interrupt
 **/
void I2C1_EV_Handler(void);

/**
 * Priority: 38
 * Type of Priority: Settable
 * 
 * I2C1 error interrupt
 **/
void I2C1_ER_Handler(void);

/**
 * Priority: 39
 * Type of Priority: Settable
 * 
 * I2C3 event interrupt
 **/
void I2C3_EV_Handler(void);

/**
 * Priority: 40
 * Type of Priority: Settable
 * 
 * I2C3 error interrupt
 **/
void I2C3_ER_Handler(void);

/**
 * Priority: 41
 * Type of Priority: Settable
 * 
 * SPI 1 global interrupt
 **/
void SPI1_Handler(void);

/**
 * Priority: 42
 * Type of Priority: Settable
 * 
 * SPI 2 global interrupt
 **/
void SPI2_Handler(void);

/**
 * Priority: 43
 * Type of Priority: Settable
 * 
 * USART1 global interrupt
 **/
void USART1_Handler(void);

/**
 * Priority: 44
 * Type of Priority: Settable
 * 
 * LPUART1 global interrupt
 **/
void LPUART1_Handler(void);

/**
 * Priority: 45
 * Type of Priority: Settable
 * 
 * SAI1 A and B global interrupt
 **/
void SAI1_Handler(void);

/**
 * Priority: 46
 * Type of Priority: Settable
 * 
 * TSC global interrupt
 **/
void TSC_Handler(void);

/**
 * Priority: 47
 * Type of Priority: Settable
 * 
 * EXTI line [15:10] interrupt through EXTI[15:10] (C1IMR1[31:26])
 **/
void EXTI15_10_Handler(void);

/**
 * Priority: 48
 * Type of Priority: Settable
 * 
 * RTC Alarms (A and B) interrupt through EXTI[17]
 **/
void RTC_ALARM_Handler(void);

/**
 * Priority: 49
 * Type of Priority: Settable
 * 
 * CRS interrupt
 **/
void CRS_IT_Handler(void);

/**
 * Priority: 50
 * Type of Priority: Settable
 * 
 * Interrupts: PWR_SOTF, PWR_BLEACT, PWR_802ACT, PWR_RFPHASE
 * 
 * PWR switching on the fly interrupt
 * PWR end of BLE activity interrupt
 * PWR end of 802.15.4 activity interrupt
 * PWR end of critical radio phase interrupt
 **/
void PWR_SOTF_Handler(void);

/**
 * Priority: 51
 * Type of Priority: Settable
 * 
 * IPCC CPU1 RX occupied interrupt
 **/
void IPCC_C1_RX_IT_Handler(void);

/**
 * Priority: 52
 * Type of Priority: Settable
 * 
 * IPCC CPU1 TX free interrupt
 **/
void IPCC_C1_TX_IT_Handler(void);

/**
 * Priority: 53
 * Type of Priority: Settable
 * 
 * Semaphore interrupt 0 to CPU1
 **/
void HSEM_Handler(void);

/**
 * Priority: 54
 * Type of Priority: Settable
 * 
 * LPtimer 1 global interrupt
 **/
void LPTIM1_Handler(void);

/**
 * Priority: 55
 * Type of Priority: Settable
 * 
 * LPtimer 2 global interrupt
 **/
void LPTIM2_Handler(void);

/**
 * Priority: 56
 * Type of Priority: Settable
 * 
 * LCD global interrupt
 **/
void LCD_Handler(void);

/**
 * Priority: 57
 * Type of Priority: Settable
 * 
 * QUADSPI global interrupt
 **/
void QUADSPI_Handler(void);

/**
 * Priority: 58
 * Type of Priority: Settable
 * 
 * AES1 global interrupt
 **/
void AES1_Handler(void);

/**
 * Priority: 59
 * Type of Priority: Settable
 * 
 * AES2 global interrupt
 **/
void AES2_Handler(void);

/**
 * Priority: 60
 * Type of Priority: Settable
 * 
 * RNG	True random number generator interrupt
 **/
void RNG_Handler(void);

/**
 * Priority: 61
 * Type of Priority: Settable
 * 
 * Floating point unit interrupt
 **/
void FPU_Handler(void);

/**
 * Priority: 62
 * Type of Priority: Settable
 * 
 * DMA2 channel 1 interrupt
 **/
void DMA2_CH1_Handler(void);

/**
 * Priority: 63
 * Type of Priority: Settable
 * 
 * DMA2 channel 2 interrupt
 **/
void DMA2_CH2_Handler(void);

/**
 * Priority: 64
 * Type of Priority: Settable
 * 
 * DMA2 channel 3 interrupt
 **/
void DMA2_CH3_Handler(void);

/**
 * Priority: 65
 * Type of Priority: Settable
 * 
 * DMA2 channel 4 interrupt
 **/
void DMA2_CH4_Handler(void);

/**
 * Priority: 66
 * Type of Priority: Settable
 * 
 * DMA2 channel 5 interrupt
 **/
void DMA2_CH5_Handler(void);

/**
 * Priority: 67
 * Type of Priority: Settable
 * 
 * DMA2 channel 6 interrupt
 **/
void DMA2_CH6_Handler(void);

/**
 * Priority: 68
 * Type of Priority: Settable
 * 
 * DMA2 channel 7 interrupt
 **/
void DMA2_CH7_Handler(void);

/**
 * Priority: 69
 * Type of Priority: Settable
 * 
 * DMAMUX1 overrun interrupt
 **/
void DMAMUX1_OVR_Handler(void);

#endif // _BOOT_H_INCLUDED_
