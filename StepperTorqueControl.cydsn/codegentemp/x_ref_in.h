/*******************************************************************************
* File Name: x_ref_in.h  
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

#if !defined(CY_PINS_x_ref_in_H) /* Pins x_ref_in_H */
#define CY_PINS_x_ref_in_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "x_ref_in_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 x_ref_in__PORT == 15 && ((x_ref_in__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    x_ref_in_Write(uint8 value);
void    x_ref_in_SetDriveMode(uint8 mode);
uint8   x_ref_in_ReadDataReg(void);
uint8   x_ref_in_Read(void);
void    x_ref_in_SetInterruptMode(uint16 position, uint16 mode);
uint8   x_ref_in_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the x_ref_in_SetDriveMode() function.
     *  @{
     */
        #define x_ref_in_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define x_ref_in_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define x_ref_in_DM_RES_UP          PIN_DM_RES_UP
        #define x_ref_in_DM_RES_DWN         PIN_DM_RES_DWN
        #define x_ref_in_DM_OD_LO           PIN_DM_OD_LO
        #define x_ref_in_DM_OD_HI           PIN_DM_OD_HI
        #define x_ref_in_DM_STRONG          PIN_DM_STRONG
        #define x_ref_in_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define x_ref_in_MASK               x_ref_in__MASK
#define x_ref_in_SHIFT              x_ref_in__SHIFT
#define x_ref_in_WIDTH              1u

/* Interrupt constants */
#if defined(x_ref_in__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in x_ref_in_SetInterruptMode() function.
     *  @{
     */
        #define x_ref_in_INTR_NONE      (uint16)(0x0000u)
        #define x_ref_in_INTR_RISING    (uint16)(0x0001u)
        #define x_ref_in_INTR_FALLING   (uint16)(0x0002u)
        #define x_ref_in_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define x_ref_in_INTR_MASK      (0x01u) 
#endif /* (x_ref_in__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define x_ref_in_PS                     (* (reg8 *) x_ref_in__PS)
/* Data Register */
#define x_ref_in_DR                     (* (reg8 *) x_ref_in__DR)
/* Port Number */
#define x_ref_in_PRT_NUM                (* (reg8 *) x_ref_in__PRT) 
/* Connect to Analog Globals */                                                  
#define x_ref_in_AG                     (* (reg8 *) x_ref_in__AG)                       
/* Analog MUX bux enable */
#define x_ref_in_AMUX                   (* (reg8 *) x_ref_in__AMUX) 
/* Bidirectional Enable */                                                        
#define x_ref_in_BIE                    (* (reg8 *) x_ref_in__BIE)
/* Bit-mask for Aliased Register Access */
#define x_ref_in_BIT_MASK               (* (reg8 *) x_ref_in__BIT_MASK)
/* Bypass Enable */
#define x_ref_in_BYP                    (* (reg8 *) x_ref_in__BYP)
/* Port wide control signals */                                                   
#define x_ref_in_CTL                    (* (reg8 *) x_ref_in__CTL)
/* Drive Modes */
#define x_ref_in_DM0                    (* (reg8 *) x_ref_in__DM0) 
#define x_ref_in_DM1                    (* (reg8 *) x_ref_in__DM1)
#define x_ref_in_DM2                    (* (reg8 *) x_ref_in__DM2) 
/* Input Buffer Disable Override */
#define x_ref_in_INP_DIS                (* (reg8 *) x_ref_in__INP_DIS)
/* LCD Common or Segment Drive */
#define x_ref_in_LCD_COM_SEG            (* (reg8 *) x_ref_in__LCD_COM_SEG)
/* Enable Segment LCD */
#define x_ref_in_LCD_EN                 (* (reg8 *) x_ref_in__LCD_EN)
/* Slew Rate Control */
#define x_ref_in_SLW                    (* (reg8 *) x_ref_in__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define x_ref_in_PRTDSI__CAPS_SEL       (* (reg8 *) x_ref_in__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define x_ref_in_PRTDSI__DBL_SYNC_IN    (* (reg8 *) x_ref_in__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define x_ref_in_PRTDSI__OE_SEL0        (* (reg8 *) x_ref_in__PRTDSI__OE_SEL0) 
#define x_ref_in_PRTDSI__OE_SEL1        (* (reg8 *) x_ref_in__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define x_ref_in_PRTDSI__OUT_SEL0       (* (reg8 *) x_ref_in__PRTDSI__OUT_SEL0) 
#define x_ref_in_PRTDSI__OUT_SEL1       (* (reg8 *) x_ref_in__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define x_ref_in_PRTDSI__SYNC_OUT       (* (reg8 *) x_ref_in__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(x_ref_in__SIO_CFG)
    #define x_ref_in_SIO_HYST_EN        (* (reg8 *) x_ref_in__SIO_HYST_EN)
    #define x_ref_in_SIO_REG_HIFREQ     (* (reg8 *) x_ref_in__SIO_REG_HIFREQ)
    #define x_ref_in_SIO_CFG            (* (reg8 *) x_ref_in__SIO_CFG)
    #define x_ref_in_SIO_DIFF           (* (reg8 *) x_ref_in__SIO_DIFF)
#endif /* (x_ref_in__SIO_CFG) */

/* Interrupt Registers */
#if defined(x_ref_in__INTSTAT)
    #define x_ref_in_INTSTAT            (* (reg8 *) x_ref_in__INTSTAT)
    #define x_ref_in_SNAP               (* (reg8 *) x_ref_in__SNAP)
    
	#define x_ref_in_0_INTTYPE_REG 		(* (reg8 *) x_ref_in__0__INTTYPE)
#endif /* (x_ref_in__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_x_ref_in_H */


/* [] END OF FILE */
