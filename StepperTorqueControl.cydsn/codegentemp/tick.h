/*******************************************************************************
* File Name: tick.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_tick_H)
#define CY_ISR_tick_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void tick_Start(void);
void tick_StartEx(cyisraddress address);
void tick_Stop(void);

CY_ISR_PROTO(tick_Interrupt);

void tick_SetVector(cyisraddress address);
cyisraddress tick_GetVector(void);

void tick_SetPriority(uint8 priority);
uint8 tick_GetPriority(void);

void tick_Enable(void);
uint8 tick_GetState(void);
void tick_Disable(void);

void tick_SetPending(void);
void tick_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the tick ISR. */
#define tick_INTC_VECTOR            ((reg32 *) tick__INTC_VECT)

/* Address of the tick ISR priority. */
#define tick_INTC_PRIOR             ((reg8 *) tick__INTC_PRIOR_REG)

/* Priority of the tick interrupt. */
#define tick_INTC_PRIOR_NUMBER      tick__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable tick interrupt. */
#define tick_INTC_SET_EN            ((reg32 *) tick__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the tick interrupt. */
#define tick_INTC_CLR_EN            ((reg32 *) tick__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the tick interrupt state to pending. */
#define tick_INTC_SET_PD            ((reg32 *) tick__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the tick interrupt. */
#define tick_INTC_CLR_PD            ((reg32 *) tick__INTC_CLR_PD_REG)


#endif /* CY_ISR_tick_H */


/* [] END OF FILE */
