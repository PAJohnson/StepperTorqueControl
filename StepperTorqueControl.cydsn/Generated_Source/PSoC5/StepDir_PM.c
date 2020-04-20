/*******************************************************************************
* File Name: StepDir_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "StepDir.h"

static StepDir_backupStruct StepDir_backup;


/*******************************************************************************
* Function Name: StepDir_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepDir_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void StepDir_SaveConfig(void) 
{
    #if (!StepDir_UsingFixedFunction)

        StepDir_backup.CounterUdb = StepDir_ReadCounter();

        #if(!StepDir_ControlRegRemoved)
            StepDir_backup.CounterControlRegister = StepDir_ReadControlRegister();
        #endif /* (!StepDir_ControlRegRemoved) */

    #endif /* (!StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepDir_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void StepDir_RestoreConfig(void) 
{      
    #if (!StepDir_UsingFixedFunction)

       StepDir_WriteCounter(StepDir_backup.CounterUdb);

        #if(!StepDir_ControlRegRemoved)
            StepDir_WriteControlRegister(StepDir_backup.CounterControlRegister);
        #endif /* (!StepDir_ControlRegRemoved) */

    #endif /* (!StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepDir_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void StepDir_Sleep(void) 
{
    #if(!StepDir_ControlRegRemoved)
        /* Save Counter's enable state */
        if(StepDir_CTRL_ENABLE == (StepDir_CONTROL & StepDir_CTRL_ENABLE))
        {
            /* Counter is enabled */
            StepDir_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            StepDir_backup.CounterEnableState = 0u;
        }
    #else
        StepDir_backup.CounterEnableState = 1u;
        if(StepDir_backup.CounterEnableState != 0u)
        {
            StepDir_backup.CounterEnableState = 0u;
        }
    #endif /* (!StepDir_ControlRegRemoved) */
    
    StepDir_Stop();
    StepDir_SaveConfig();
}


/*******************************************************************************
* Function Name: StepDir_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  StepDir_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void StepDir_Wakeup(void) 
{
    StepDir_RestoreConfig();
    #if(!StepDir_ControlRegRemoved)
        if(StepDir_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            StepDir_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!StepDir_ControlRegRemoved) */
    
}


/* [] END OF FILE */
