/*******************************************************************************
* File Name: MS2.h  
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

#if !defined(CY_PINS_MS2_H) /* Pins MS2_H */
#define CY_PINS_MS2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MS2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MS2__PORT == 15 && ((MS2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MS2_Write(uint8 value);
void    MS2_SetDriveMode(uint8 mode);
uint8   MS2_ReadDataReg(void);
uint8   MS2_Read(void);
void    MS2_SetInterruptMode(uint16 position, uint16 mode);
uint8   MS2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MS2_SetDriveMode() function.
     *  @{
     */
        #define MS2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MS2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MS2_DM_RES_UP          PIN_DM_RES_UP
        #define MS2_DM_RES_DWN         PIN_DM_RES_DWN
        #define MS2_DM_OD_LO           PIN_DM_OD_LO
        #define MS2_DM_OD_HI           PIN_DM_OD_HI
        #define MS2_DM_STRONG          PIN_DM_STRONG
        #define MS2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MS2_MASK               MS2__MASK
#define MS2_SHIFT              MS2__SHIFT
#define MS2_WIDTH              1u

/* Interrupt constants */
#if defined(MS2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MS2_SetInterruptMode() function.
     *  @{
     */
        #define MS2_INTR_NONE      (uint16)(0x0000u)
        #define MS2_INTR_RISING    (uint16)(0x0001u)
        #define MS2_INTR_FALLING   (uint16)(0x0002u)
        #define MS2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MS2_INTR_MASK      (0x01u) 
#endif /* (MS2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MS2_PS                     (* (reg8 *) MS2__PS)
/* Data Register */
#define MS2_DR                     (* (reg8 *) MS2__DR)
/* Port Number */
#define MS2_PRT_NUM                (* (reg8 *) MS2__PRT) 
/* Connect to Analog Globals */                                                  
#define MS2_AG                     (* (reg8 *) MS2__AG)                       
/* Analog MUX bux enable */
#define MS2_AMUX                   (* (reg8 *) MS2__AMUX) 
/* Bidirectional Enable */                                                        
#define MS2_BIE                    (* (reg8 *) MS2__BIE)
/* Bit-mask for Aliased Register Access */
#define MS2_BIT_MASK               (* (reg8 *) MS2__BIT_MASK)
/* Bypass Enable */
#define MS2_BYP                    (* (reg8 *) MS2__BYP)
/* Port wide control signals */                                                   
#define MS2_CTL                    (* (reg8 *) MS2__CTL)
/* Drive Modes */
#define MS2_DM0                    (* (reg8 *) MS2__DM0) 
#define MS2_DM1                    (* (reg8 *) MS2__DM1)
#define MS2_DM2                    (* (reg8 *) MS2__DM2) 
/* Input Buffer Disable Override */
#define MS2_INP_DIS                (* (reg8 *) MS2__INP_DIS)
/* LCD Common or Segment Drive */
#define MS2_LCD_COM_SEG            (* (reg8 *) MS2__LCD_COM_SEG)
/* Enable Segment LCD */
#define MS2_LCD_EN                 (* (reg8 *) MS2__LCD_EN)
/* Slew Rate Control */
#define MS2_SLW                    (* (reg8 *) MS2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MS2_PRTDSI__CAPS_SEL       (* (reg8 *) MS2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MS2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MS2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MS2_PRTDSI__OE_SEL0        (* (reg8 *) MS2__PRTDSI__OE_SEL0) 
#define MS2_PRTDSI__OE_SEL1        (* (reg8 *) MS2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MS2_PRTDSI__OUT_SEL0       (* (reg8 *) MS2__PRTDSI__OUT_SEL0) 
#define MS2_PRTDSI__OUT_SEL1       (* (reg8 *) MS2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MS2_PRTDSI__SYNC_OUT       (* (reg8 *) MS2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MS2__SIO_CFG)
    #define MS2_SIO_HYST_EN        (* (reg8 *) MS2__SIO_HYST_EN)
    #define MS2_SIO_REG_HIFREQ     (* (reg8 *) MS2__SIO_REG_HIFREQ)
    #define MS2_SIO_CFG            (* (reg8 *) MS2__SIO_CFG)
    #define MS2_SIO_DIFF           (* (reg8 *) MS2__SIO_DIFF)
#endif /* (MS2__SIO_CFG) */

/* Interrupt Registers */
#if defined(MS2__INTSTAT)
    #define MS2_INTSTAT            (* (reg8 *) MS2__INTSTAT)
    #define MS2_SNAP               (* (reg8 *) MS2__SNAP)
    
	#define MS2_0_INTTYPE_REG 		(* (reg8 *) MS2__0__INTTYPE)
#endif /* (MS2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MS2_H */


/* [] END OF FILE */
