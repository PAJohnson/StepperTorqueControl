/*******************************************************************************
* File Name: Microstep_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Microstep.h"

/* Check for removal by optimization */
#if !defined(Microstep_Sync_ctrl_reg__REMOVED)

static Microstep_BACKUP_STRUCT  Microstep_backup = {0u};

    
/*******************************************************************************
* Function Name: Microstep_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Microstep_SaveConfig(void) 
{
    Microstep_backup.controlState = Microstep_Control;
}


/*******************************************************************************
* Function Name: Microstep_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void Microstep_RestoreConfig(void) 
{
     Microstep_Control = Microstep_backup.controlState;
}


/*******************************************************************************
* Function Name: Microstep_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Microstep_Sleep(void) 
{
    Microstep_SaveConfig();
}


/*******************************************************************************
* Function Name: Microstep_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Microstep_Wakeup(void)  
{
    Microstep_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
