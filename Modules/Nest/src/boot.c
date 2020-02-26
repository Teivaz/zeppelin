/**
 * Boot file for the STM32WB55xx SoC
 * Created by Teivaz
 * 16.02.2020
 */

#include "boot.h"

extern int main(void);
extern void __libc_init_array(void);

// Provided by the linker script
extern int _stack;
extern int _data_start;
extern int _data_end;
extern int _data_start_initialised;
extern int __bss_start;
extern int __bss_end;
extern int _mbMem2_start;
extern int _mbMem2_end;

#define _WEAK_DEFAULT __attribute__ ((weak, alias ("default_handler")))
#define _INLINE __attribute__((always_inline)) inline

typedef void (*fptr)(void);

#ifdef DEFAULT_HANDLER_NONBLOCKING
	static void default_handler(void) {}
#else
	static void default_handler(void) { while (1); }
#endif

_INLINE void zero_initialise(int* region_begin, int const* region_end) {
	int* p = region_begin;
	while (p < region_end) *p++ = 0;
}
_INLINE void copy_data(int const* source_begin, int const* source_end, int* dest) {
	int const* p = source_begin;
	while (p < source_end) *dest++ = *p++;
}

__attribute__ ((weak, naked)) void Reset_Handler(void) {
	copy_data(&_data_start, &_data_end, &_data_start_initialised);
	zero_initialise(&__bss_start, &__bss_end);
	zero_initialise(&_mbMem2_start, &_mbMem2_end);
	__libc_init_array();
	main();
	while(1);
}

_WEAK_DEFAULT void NMI_Handler(void);
_WEAK_DEFAULT void HardFault_Handler(void);
_WEAK_DEFAULT void MemManager_Handler(void);
_WEAK_DEFAULT void BusFault_Handler(void);
_WEAK_DEFAULT void UsageFault_Handler(void);
_WEAK_DEFAULT void SVCall_Handler(void);
_WEAK_DEFAULT void Debug_Handler(void);
_WEAK_DEFAULT void PendSV_Handler(void);
_WEAK_DEFAULT void Systick_Handler(void);
_WEAK_DEFAULT void WWDG_Handler(void);
_WEAK_DEFAULT void PVD_Handler(void);
_WEAK_DEFAULT void TAMP_Handler(void);
_WEAK_DEFAULT void RTC_WKUP_Handler(void);
_WEAK_DEFAULT void Flash_Handler(void);
_WEAK_DEFAULT void RCC_Handler(void);
_WEAK_DEFAULT void EXTI0_Handler(void);
_WEAK_DEFAULT void EXTI1_Handler(void);
_WEAK_DEFAULT void EXTI2_Handler(void);
_WEAK_DEFAULT void EXTI3_Handler(void);
_WEAK_DEFAULT void EXTI4_Handler(void);
_WEAK_DEFAULT void DMA1_CH1_Handler(void);
_WEAK_DEFAULT void DMA1_CH2_Handler(void);
_WEAK_DEFAULT void DMA1_CH3_Handler(void);
_WEAK_DEFAULT void DMA1_CH4_Handler(void);
_WEAK_DEFAULT void DMA1_CH5_Handler(void);
_WEAK_DEFAULT void DMA1_CH6_Handler(void);
_WEAK_DEFAULT void DMA1_CH7_Handler(void);
_WEAK_DEFAULT void ADC1_Handler(void);
_WEAK_DEFAULT void USB_HP_Handler(void);
_WEAK_DEFAULT void USB_LP_Handler(void);
_WEAK_DEFAULT void C2SEV_Handler(void);
_WEAK_DEFAULT void COMP_Handler(void);
_WEAK_DEFAULT void EXTI_9_5_Handler(void);
_WEAK_DEFAULT void TIM1_BRK_Handler(void);
_WEAK_DEFAULT void TIM1_UP_Handler(void);
_WEAK_DEFAULT void TIM1_TRG_COM_Handler(void);
_WEAK_DEFAULT void TIM1_CC_Handler(void);
_WEAK_DEFAULT void TIM2_Handler(void);
_WEAK_DEFAULT void PKA_Handler(void);
_WEAK_DEFAULT void I2C1_EV_Handler(void);
_WEAK_DEFAULT void I2C1_ER_Handler(void);
_WEAK_DEFAULT void I2C3_EV_Handler(void);
_WEAK_DEFAULT void I2C3_ER_Handler(void);
_WEAK_DEFAULT void SPI1_Handler(void);
_WEAK_DEFAULT void SPI2_Handler(void);
_WEAK_DEFAULT void USART1_Handler(void);
_WEAK_DEFAULT void LPUART1_Handler(void);
_WEAK_DEFAULT void SAI1_Handler(void);
_WEAK_DEFAULT void TSC_Handler(void);
_WEAK_DEFAULT void EXTI15_10_Handler(void);
_WEAK_DEFAULT void RTC_ALARM_Handler(void);
_WEAK_DEFAULT void CRS_IT_Handler(void);
_WEAK_DEFAULT void PWR_SOTF_Handler(void);
_WEAK_DEFAULT void IPCC_C1_RX_IT_Handler(void);
_WEAK_DEFAULT void IPCC_C1_TX_IT_Handler(void);
_WEAK_DEFAULT void HSEM_Handler(void);
_WEAK_DEFAULT void LPTIM1_Handler(void);
_WEAK_DEFAULT void LPTIM2_Handler(void);
_WEAK_DEFAULT void LCD_Handler(void);
_WEAK_DEFAULT void QUADSPI_Handler(void);
_WEAK_DEFAULT void AES1_Handler(void);
_WEAK_DEFAULT void AES2_Handler(void);
_WEAK_DEFAULT void RNG_Handler(void);
_WEAK_DEFAULT void FPU_Handler(void);
_WEAK_DEFAULT void DMA2_CH1_Handler(void);
_WEAK_DEFAULT void DMA2_CH2_Handler(void);
_WEAK_DEFAULT void DMA2_CH3_Handler(void);
_WEAK_DEFAULT void DMA2_CH4_Handler(void);
_WEAK_DEFAULT void DMA2_CH5_Handler(void);
_WEAK_DEFAULT void DMA2_CH6_Handler(void);
_WEAK_DEFAULT void DMA2_CH7_Handler(void);
_WEAK_DEFAULT void DMAMUX1_OVR_Handler(void);

__attribute__ ((section(".vector")))
struct {
	int* StackPtr;						// 0x0000
	fptr Reset;								// 0x0004
	fptr NMI;									// 0x0008
	fptr HardFault;						// 0x000C
	fptr MemManager;					// 0x0010
	fptr BusFault;						// 0x0014
	fptr UsageFault;					// 0x0018
	fptr Reserved_1C[4];			// 0x001C - 0x0028
	fptr SVCall;							// 0x002C
	fptr Debug;								// 0x0030
	fptr Reserved_34;					// 0x0034
	fptr PendSV;							// 0x0038
	fptr Systick;							// 0x003C
	fptr WWDG;								// 0x0040
	fptr PVD;									// 0x0044
	fptr TAMP;								// 0x0048
	fptr RTC_WKUP;						// 0x004C
	fptr Flash;								// 0x0050
	fptr RCC;									// 0x0054
	fptr EXTI0;								// 0x0058
	fptr EXTI1;								// 0x005C
	fptr EXTI2;								// 0x0060
	fptr EXTI3;								// 0x0064
	fptr EXTI4;								// 0x0068
	fptr DMA1_CH1;						// 0x006C
	fptr DMA1_CH2;						// 0x0070
	fptr DMA1_CH3;						// 0x0074
	fptr DMA1_CH4;						// 0x0078
	fptr DMA1_CH5;						// 0x007C
	fptr DMA1_CH6;						// 0x0080
	fptr DMA1_CH7;						// 0x0084
	fptr ADC1;								// 0x0088
	fptr USB_HP;							// 0x008C
	fptr USB_LP;							// 0x0090
	fptr C2SEV;								// 0x0094
	fptr COMP;								// 0x0098
	fptr EXTI_9_5;						// 0x009C
	fptr TIM1_BRK;						// 0x00A0
	fptr TIM1_UP;							// 0x00A4
	fptr TIM1_TRG_COM;				// 0x00A8
	fptr TIM1_CC;							// 0x00AC
	fptr TIM2;								// 0x00B0
	fptr PKA;									// 0x00B4
	fptr I2C1_EV;							// 0x00B8
	fptr I2C1_ER;							// 0x00BC
	fptr I2C3_EV;							// 0x00C0
	fptr I2C3_ER;							// 0x00C4
	fptr SPI1;								// 0x00C8
	fptr SPI2;								// 0x00CC
	fptr USART1;							// 0x00D0
	fptr LPUART1;							// 0x00D4
	fptr SAI1;								// 0x00D8
	fptr TSC;									// 0x00DC
	fptr EXTI15_10;						// 0x00E0
	fptr RTC_ALARM;						// 0x00E4
	fptr CRS_IT;							// 0x00E8
	fptr PWR_SOTF;						// 0x00EC
	fptr IPCC_C1_RX_IT;				// 0x00F0
	fptr IPCC_C1_TX_IT;				// 0x00F4
	fptr HSEM;								// 0x00F8
	fptr LPTIM1;							// 0x00FC
	fptr LPTIM2;							// 0x0100
	fptr LCD;									// 0x0104
	fptr QUADSPI;							// 0x0108
	fptr AES1;								// 0x010C
	fptr AES2;								// 0x0110
	fptr RNG;									// 0x0114
	fptr FPU;									// 0x0118
	fptr DMA2_CH1;						// 0x011C
	fptr DMA2_CH2;						// 0x0120
	fptr DMA2_CH3;						// 0x0124
	fptr DMA2_CH4;						// 0x0128
	fptr DMA2_CH5;						// 0x012C
	fptr DMA2_CH6;						// 0x0130
	fptr DMA2_CH7;						// 0x0134
	fptr DMAMUX1_OVR;					// 0x0138
} _vector_table = {
	.StackPtr = &_stack,
	.Reset = Reset_Handler,
	.NMI = NMI_Handler,
	.HardFault = HardFault_Handler,
	.MemManager = MemManager_Handler,
	.BusFault = BusFault_Handler,
	.UsageFault = UsageFault_Handler,
	.SVCall = SVCall_Handler,
	.Debug = Debug_Handler,
	.PendSV = PendSV_Handler,
	.Systick = Systick_Handler,
	.WWDG = WWDG_Handler,
	.PVD = PVD_Handler,
	.TAMP = TAMP_Handler,
	.RTC_WKUP = RTC_WKUP_Handler,
	.Flash = Flash_Handler,
	.RCC = RCC_Handler,
	.EXTI0 = EXTI0_Handler,
	.EXTI1 = EXTI1_Handler,
	.EXTI2 = EXTI2_Handler,
	.EXTI3 = EXTI3_Handler,
	.EXTI4 = EXTI4_Handler,
	.DMA1_CH1 = DMA1_CH1_Handler,
	.DMA1_CH2 = DMA1_CH2_Handler,
	.DMA1_CH3 = DMA1_CH3_Handler,
	.DMA1_CH4 = DMA1_CH4_Handler,
	.DMA1_CH5 = DMA1_CH5_Handler,
	.DMA1_CH6 = DMA1_CH6_Handler,
	.DMA1_CH7 = DMA1_CH7_Handler,
	.ADC1 = ADC1_Handler,
	.USB_HP = USB_HP_Handler,
	.USB_LP = USB_LP_Handler,
	.C2SEV = C2SEV_Handler,
	.COMP = COMP_Handler,
	.EXTI_9_5 = EXTI_9_5_Handler,
	.TIM1_BRK = TIM1_BRK_Handler,
	.TIM1_UP = TIM1_UP_Handler,
	.TIM1_TRG_COM = TIM1_TRG_COM_Handler,
	.TIM1_CC = TIM1_CC_Handler,
	.TIM2 = TIM2_Handler,
	.PKA = PKA_Handler,
	.I2C1_EV = I2C1_EV_Handler,
	.I2C1_ER = I2C1_ER_Handler,
	.I2C3_EV = I2C3_EV_Handler,
	.I2C3_ER = I2C3_ER_Handler,
	.SPI1 = SPI1_Handler,
	.SPI2 = SPI2_Handler,
	.USART1 = USART1_Handler,
	.LPUART1 = LPUART1_Handler,
	.SAI1 = SAI1_Handler,
	.TSC = TSC_Handler,
	.EXTI15_10 = EXTI15_10_Handler,
	.RTC_ALARM = RTC_ALARM_Handler,
	.CRS_IT = CRS_IT_Handler,
	.PWR_SOTF = PWR_SOTF_Handler,
	.IPCC_C1_RX_IT = IPCC_C1_RX_IT_Handler,
	.IPCC_C1_TX_IT = IPCC_C1_TX_IT_Handler,
	.HSEM = HSEM_Handler,
	.LPTIM1 = LPTIM1_Handler,
	.LPTIM2 = LPTIM2_Handler,
	.LCD = LCD_Handler,
	.QUADSPI = QUADSPI_Handler,
	.AES1 = AES1_Handler,
	.AES2 = AES2_Handler,
	.RNG = RNG_Handler,
	.FPU = FPU_Handler,
	.DMA2_CH1 = DMA2_CH1_Handler,
	.DMA2_CH2 = DMA2_CH2_Handler,
	.DMA2_CH3 = DMA2_CH3_Handler,
	.DMA2_CH4 = DMA2_CH4_Handler,
	.DMA2_CH5 = DMA2_CH5_Handler,
	.DMA2_CH6 = DMA2_CH6_Handler,
	.DMA2_CH7 = DMA2_CH7_Handler,
	.DMAMUX1_OVR = DMAMUX1_OVR_Handler,
};
