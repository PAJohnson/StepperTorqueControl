/*******************************************************************************
* File Name: STEP.h  
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

#if !defined(CY_PINS_STEP_H) /* Pins STEP_H */
#define CY_PINS_STEP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "STEP_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 STEP__PORT == 15 && ((STEP__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    STEP_Write(uint8 value);
void    STEP_SetDriveMode(uint8 mode);
uint8   STEP_ReadDataReg(void);
uint8   STEP_Read(void);
void    STEP_SetInterruptMode(uint16 position, uint16 mode);
uint8   STEP_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the STEP_SetDriveMode() function.
     *  @{
     */
        #define STEP_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define STEP_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define STEP_DM_RES_UP          PIN_DM_RES_UP
        #define STEP_DM_RES_DWN         PIN_DM_RES_DWN
        #define STEP_DM_OD_LO           PIN_DM_OD_LO
        #define STEP_DM_OD_HI           PIN_DM_OD_HI
        #define STEP_DM_STRONG          PIN_DM_STRONG
        #define STEP_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define STEP_MASK               STEP__MASK
#define STEP_SHIFT              STEP__SHIFT
#define STEP_WIDTH              1u

/* Interrupt constants */
#if defined(STEP__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in STEP_SetInterruptMode() function.
     *  @{
     */
        #define STEP_INTR_NONE      (uint16)(0x0000u)
        #define STEP_INTR_RISING    (uint16)(0x0001u)
        #define STEP_INTR_FALLING   (uint16)(0x0002u)
        #define STEP_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define STEP_INTR_MASK      (0x01u) 
#endif /* (STEP__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define STEP_PS                     (* (reg8 *) STEP__PS)
/* Data Register */
#define STEP_DR                     (* (reg8 *) STEP__DR)
/* Port Number */
#define STEP_PRT_NUM                (* (reg8 *) STEP__PRT) 
/* Connect to Analog Globals */                                                  
#define STEP_AG                     (* (reg8 *) STEP__AG)                       
/* Analog MUX bux enable */
#define STEP_AMUX                   (* (reg8 *) STEP__AMUX) 
/* Bidirectional Enable */                                                        
#define STEP_BIE                    (* (reg8 *) STEP__BIE)
/* Bit-mask for Aliased Register Access */
#define STEP_BIT_MASK               (* (reg8 *) STEP__BIT_MASK)
/* Bypass Enable */
#define STEP_BYP                    (* (reg8 *) STEP__BYP)
/* Port wide control signals */                                                   
#define STEP_CTL                    (* (reg8 *) STEP__CTL)
/* Drive Modes */
#define STEP_DM0                    (* (reg8 *) STEP__DM0) 
#define STEP_DM1                    (* (reg8 *) STEP__DM1)
#define STEP_DM2                    (* (reg8 *) STEP__DM2) 
/* Input Buffer Disable Override */
#define STEP_INP_DIS                (* (reg8 *) STEP__INP_DIS)
/* LCD Common or Segment Drive */
#define STEP_LCD_COM_SEG            (* (reg8 *) STEP__LCD_COM_SEG)
/* Enable Segment LCD */
#define STEP_LCD_EN                 (* (reg8 *) STEP__LCD_EN)
/* Slew Rate Control */
#define STEP_SLW                    (* (reg8 *) STEP__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define STEP_PRTDSI__CAPS_SEL       (* (reg8 *) STEP__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define STEP_PRTDSI__DBL_SYNC_IN    (* (reg8 *) STEP__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define STEP_PRTDSI__OE_SEL0        (* (reg8 *) STEP__PRTDSI__OE_SEL0) 
#define STEP_PRTDSI__OE_SEL1        (* (reg8 *) STEP__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define STEP_PRTDSI__OUT_SEL0       (* (reg8 *) STEP__PRTDSI__OUT_SEL0) 
#define STEP_PRTDSI__OUT_SEL1       (* (reg8 *) STEP__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define STEP_PRTDSI__SYNC_OUT       (* (reg8 *) STEP__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(STEP__SIO_CFG)
    #define STEP_SIO_HYST_EN        (* (reg8 *) STEP__SIO_HYST_EN)
    #define STEP_SIO_REG_HIFREQ     (* (reg8 *) STEP__SIO_REG_HIFREQ)
    #define STEP_SIO_CFG            (* (reg8 *) STEP__SIO_CFG)
    #define STEP_SIO_DIFF           (* (reg8 *) STEP__SIO_DIFF)
#endif /* (STEP__SIO_CFG) */

/* Interrupt Registers */
#if defined(STEP__INTSTAT)
    #define STEP_INTSTAT            (* (reg8 *) STEP__INTSTAT)
    #define STEP_SNAP               (* (reg8 *) STEP__SNAP)
    
	#define STEP_0_INTTYPE_REG 		(* (reg8 *) STEP__0__INTTYPE)
#endif /* (STEP__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_STEP_H */


/* [] END OF FILE */
