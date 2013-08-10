/*---------------------------------------------------------------------------- 
 *      Name:    STM32_Reg.h 
 *      Purpose: STM32Register values and Bit definitions 
 *      Version: V1.00 
 *---------------------------------------------------------------------------- 
 *      This file is part of the uVision/ARM development tools. 
 *      This software may only be used under the terms of a valid, current, 
 *      end user licence from KEIL for a compatible version of KEIL software 
 *      development tools. Nothing else gives you the right to use it. 
 * 
 *      Copyright (c) 2005-2007 Keil Software. 
 *----------------------------------------------------------------------------*/ 
 
/* Define to prevent recursive inclusion ------------------------------------ */ 
#ifndef __STM32_REG_H 
#define __STM32_REg_H 
 
/*---------------------------------------------------------------------------- 
   SysTick 
 *----------------------------------------------------------------------------*/ 
/* register SYSTICK_CSR ------------------------------------------------------*/ 
#define SYSTICK_CSR_ENABLE    ((unsigned long)0x00000001) 
#define SYSTICK_CSR_COUNTFLAG ((unsigned long)0x00010000) 
  
/*---------------------------------------------------------------------------- 
   TIMX 
 *----------------------------------------------------------------------------*/ 
/* register TIMX_CR1 ---------------------------------------------------------*/ 
#define TIMX_CR1_CEN         ((unsigned short)0x0001) 
 
/* register TIMX_SR ----------------------------------------------------------*/ 
#define TIMX_SR_UIF          ((unsigned short)0x0001) 
 
#endif 
