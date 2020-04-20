/*******************************************************************************
* File Name: sw1_read.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "sw1_read.h"

#if !defined(sw1_read_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: sw1_read_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 sw1_read_Read(void) 
{ 
    return sw1_read_Status;
}


/*******************************************************************************
* Function Name: sw1_read_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void sw1_read_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    sw1_read_Status_Aux_Ctrl |= sw1_read_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: sw1_read_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void sw1_read_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    sw1_read_Status_Aux_Ctrl &= (uint8)(~sw1_read_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: sw1_read_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void sw1_read_WriteMask(uint8 mask) 
{
    #if(sw1_read_INPUTS < 8u)
    	mask &= ((uint8)(1u << sw1_read_INPUTS) - 1u);
	#endif /* End sw1_read_INPUTS < 8u */
    sw1_read_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: sw1_read_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 sw1_read_ReadMask(void) 
{
    return sw1_read_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
