/*******************************************************************************
* File Name: Interrupt_t.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Interrupt_t_H) /* Pins Interrupt_t_H */
#define CY_PINS_Interrupt_t_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Interrupt_t_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Interrupt_t__PORT == 15 && ((Interrupt_t__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Interrupt_t_Write(uint8 value);
void    Interrupt_t_SetDriveMode(uint8 mode);
uint8   Interrupt_t_ReadDataReg(void);
uint8   Interrupt_t_Read(void);
void    Interrupt_t_SetInterruptMode(uint16 position, uint16 mode);
uint8   Interrupt_t_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Interrupt_t_SetDriveMode() function.
     *  @{
     */
        #define Interrupt_t_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Interrupt_t_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Interrupt_t_DM_RES_UP          PIN_DM_RES_UP
        #define Interrupt_t_DM_RES_DWN         PIN_DM_RES_DWN
        #define Interrupt_t_DM_OD_LO           PIN_DM_OD_LO
        #define Interrupt_t_DM_OD_HI           PIN_DM_OD_HI
        #define Interrupt_t_DM_STRONG          PIN_DM_STRONG
        #define Interrupt_t_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Interrupt_t_MASK               Interrupt_t__MASK
#define Interrupt_t_SHIFT              Interrupt_t__SHIFT
#define Interrupt_t_WIDTH              1u

/* Interrupt constants */
#if defined(Interrupt_t__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Interrupt_t_SetInterruptMode() function.
     *  @{
     */
        #define Interrupt_t_INTR_NONE      (uint16)(0x0000u)
        #define Interrupt_t_INTR_RISING    (uint16)(0x0001u)
        #define Interrupt_t_INTR_FALLING   (uint16)(0x0002u)
        #define Interrupt_t_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Interrupt_t_INTR_MASK      (0x01u) 
#endif /* (Interrupt_t__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Interrupt_t_PS                     (* (reg8 *) Interrupt_t__PS)
/* Data Register */
#define Interrupt_t_DR                     (* (reg8 *) Interrupt_t__DR)
/* Port Number */
#define Interrupt_t_PRT_NUM                (* (reg8 *) Interrupt_t__PRT) 
/* Connect to Analog Globals */                                                  
#define Interrupt_t_AG                     (* (reg8 *) Interrupt_t__AG)                       
/* Analog MUX bux enable */
#define Interrupt_t_AMUX                   (* (reg8 *) Interrupt_t__AMUX) 
/* Bidirectional Enable */                                                        
#define Interrupt_t_BIE                    (* (reg8 *) Interrupt_t__BIE)
/* Bit-mask for Aliased Register Access */
#define Interrupt_t_BIT_MASK               (* (reg8 *) Interrupt_t__BIT_MASK)
/* Bypass Enable */
#define Interrupt_t_BYP                    (* (reg8 *) Interrupt_t__BYP)
/* Port wide control signals */                                                   
#define Interrupt_t_CTL                    (* (reg8 *) Interrupt_t__CTL)
/* Drive Modes */
#define Interrupt_t_DM0                    (* (reg8 *) Interrupt_t__DM0) 
#define Interrupt_t_DM1                    (* (reg8 *) Interrupt_t__DM1)
#define Interrupt_t_DM2                    (* (reg8 *) Interrupt_t__DM2) 
/* Input Buffer Disable Override */
#define Interrupt_t_INP_DIS                (* (reg8 *) Interrupt_t__INP_DIS)
/* LCD Common or Segment Drive */
#define Interrupt_t_LCD_COM_SEG            (* (reg8 *) Interrupt_t__LCD_COM_SEG)
/* Enable Segment LCD */
#define Interrupt_t_LCD_EN                 (* (reg8 *) Interrupt_t__LCD_EN)
/* Slew Rate Control */
#define Interrupt_t_SLW                    (* (reg8 *) Interrupt_t__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Interrupt_t_PRTDSI__CAPS_SEL       (* (reg8 *) Interrupt_t__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Interrupt_t_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Interrupt_t__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Interrupt_t_PRTDSI__OE_SEL0        (* (reg8 *) Interrupt_t__PRTDSI__OE_SEL0) 
#define Interrupt_t_PRTDSI__OE_SEL1        (* (reg8 *) Interrupt_t__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Interrupt_t_PRTDSI__OUT_SEL0       (* (reg8 *) Interrupt_t__PRTDSI__OUT_SEL0) 
#define Interrupt_t_PRTDSI__OUT_SEL1       (* (reg8 *) Interrupt_t__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Interrupt_t_PRTDSI__SYNC_OUT       (* (reg8 *) Interrupt_t__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Interrupt_t__SIO_CFG)
    #define Interrupt_t_SIO_HYST_EN        (* (reg8 *) Interrupt_t__SIO_HYST_EN)
    #define Interrupt_t_SIO_REG_HIFREQ     (* (reg8 *) Interrupt_t__SIO_REG_HIFREQ)
    #define Interrupt_t_SIO_CFG            (* (reg8 *) Interrupt_t__SIO_CFG)
    #define Interrupt_t_SIO_DIFF           (* (reg8 *) Interrupt_t__SIO_DIFF)
#endif /* (Interrupt_t__SIO_CFG) */

/* Interrupt Registers */
#if defined(Interrupt_t__INTSTAT)
    #define Interrupt_t_INTSTAT            (* (reg8 *) Interrupt_t__INTSTAT)
    #define Interrupt_t_SNAP               (* (reg8 *) Interrupt_t__SNAP)
    
	#define Interrupt_t_0_INTTYPE_REG 		(* (reg8 *) Interrupt_t__0__INTTYPE)
#endif /* (Interrupt_t__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Interrupt_t_H */


/* [] END OF FILE */
