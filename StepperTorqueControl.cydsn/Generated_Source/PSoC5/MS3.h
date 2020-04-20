/*******************************************************************************
* File Name: MS3.h  
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

#if !defined(CY_PINS_MS3_H) /* Pins MS3_H */
#define CY_PINS_MS3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MS3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MS3__PORT == 15 && ((MS3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MS3_Write(uint8 value);
void    MS3_SetDriveMode(uint8 mode);
uint8   MS3_ReadDataReg(void);
uint8   MS3_Read(void);
void    MS3_SetInterruptMode(uint16 position, uint16 mode);
uint8   MS3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MS3_SetDriveMode() function.
     *  @{
     */
        #define MS3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MS3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MS3_DM_RES_UP          PIN_DM_RES_UP
        #define MS3_DM_RES_DWN         PIN_DM_RES_DWN
        #define MS3_DM_OD_LO           PIN_DM_OD_LO
        #define MS3_DM_OD_HI           PIN_DM_OD_HI
        #define MS3_DM_STRONG          PIN_DM_STRONG
        #define MS3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MS3_MASK               MS3__MASK
#define MS3_SHIFT              MS3__SHIFT
#define MS3_WIDTH              1u

/* Interrupt constants */
#if defined(MS3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MS3_SetInterruptMode() function.
     *  @{
     */
        #define MS3_INTR_NONE      (uint16)(0x0000u)
        #define MS3_INTR_RISING    (uint16)(0x0001u)
        #define MS3_INTR_FALLING   (uint16)(0x0002u)
        #define MS3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MS3_INTR_MASK      (0x01u) 
#endif /* (MS3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MS3_PS                     (* (reg8 *) MS3__PS)
/* Data Register */
#define MS3_DR                     (* (reg8 *) MS3__DR)
/* Port Number */
#define MS3_PRT_NUM                (* (reg8 *) MS3__PRT) 
/* Connect to Analog Globals */                                                  
#define MS3_AG                     (* (reg8 *) MS3__AG)                       
/* Analog MUX bux enable */
#define MS3_AMUX                   (* (reg8 *) MS3__AMUX) 
/* Bidirectional Enable */                                                        
#define MS3_BIE                    (* (reg8 *) MS3__BIE)
/* Bit-mask for Aliased Register Access */
#define MS3_BIT_MASK               (* (reg8 *) MS3__BIT_MASK)
/* Bypass Enable */
#define MS3_BYP                    (* (reg8 *) MS3__BYP)
/* Port wide control signals */                                                   
#define MS3_CTL                    (* (reg8 *) MS3__CTL)
/* Drive Modes */
#define MS3_DM0                    (* (reg8 *) MS3__DM0) 
#define MS3_DM1                    (* (reg8 *) MS3__DM1)
#define MS3_DM2                    (* (reg8 *) MS3__DM2) 
/* Input Buffer Disable Override */
#define MS3_INP_DIS                (* (reg8 *) MS3__INP_DIS)
/* LCD Common or Segment Drive */
#define MS3_LCD_COM_SEG            (* (reg8 *) MS3__LCD_COM_SEG)
/* Enable Segment LCD */
#define MS3_LCD_EN                 (* (reg8 *) MS3__LCD_EN)
/* Slew Rate Control */
#define MS3_SLW                    (* (reg8 *) MS3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MS3_PRTDSI__CAPS_SEL       (* (reg8 *) MS3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MS3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MS3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MS3_PRTDSI__OE_SEL0        (* (reg8 *) MS3__PRTDSI__OE_SEL0) 
#define MS3_PRTDSI__OE_SEL1        (* (reg8 *) MS3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MS3_PRTDSI__OUT_SEL0       (* (reg8 *) MS3__PRTDSI__OUT_SEL0) 
#define MS3_PRTDSI__OUT_SEL1       (* (reg8 *) MS3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MS3_PRTDSI__SYNC_OUT       (* (reg8 *) MS3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MS3__SIO_CFG)
    #define MS3_SIO_HYST_EN        (* (reg8 *) MS3__SIO_HYST_EN)
    #define MS3_SIO_REG_HIFREQ     (* (reg8 *) MS3__SIO_REG_HIFREQ)
    #define MS3_SIO_CFG            (* (reg8 *) MS3__SIO_CFG)
    #define MS3_SIO_DIFF           (* (reg8 *) MS3__SIO_DIFF)
#endif /* (MS3__SIO_CFG) */

/* Interrupt Registers */
#if defined(MS3__INTSTAT)
    #define MS3_INTSTAT            (* (reg8 *) MS3__INTSTAT)
    #define MS3_SNAP               (* (reg8 *) MS3__SNAP)
    
	#define MS3_0_INTTYPE_REG 		(* (reg8 *) MS3__0__INTTYPE)
#endif /* (MS3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MS3_H */


/* [] END OF FILE */
