/*******************************************************************************
* File Name: Count_Reset_PM.c
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

#include "Count_Reset.h"

/* Check for removal by optimization */
#if !defined(Count_Reset_Sync_ctrl_reg__REMOVED)

static Count_Reset_BACKUP_STRUCT  Count_Reset_backup = {0u};

    
/*******************************************************************************
* Function Name: Count_Reset_SaveConfig
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
void Count_Reset_SaveConfig(void) 
{
    Count_Reset_backup.controlState = Count_Reset_Control;
}


/*******************************************************************************
* Function Name: Count_Reset_RestoreConfig
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
void Count_Reset_RestoreConfig(void) 
{
     Count_Reset_Control = Count_Reset_backup.controlState;
}


/*******************************************************************************
* Function Name: Count_Reset_Sleep
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
void Count_Reset_Sleep(void) 
{
    Count_Reset_SaveConfig();
}


/*******************************************************************************
* Function Name: Count_Reset_Wakeup
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
void Count_Reset_Wakeup(void)  
{
    Count_Reset_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
