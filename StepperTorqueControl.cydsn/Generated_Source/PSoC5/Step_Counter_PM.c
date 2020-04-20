/*******************************************************************************
* File Name: Step_Counter_PM.c  
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

#include "Step_Counter.h"

static Step_Counter_backupStruct Step_Counter_backup;


/*******************************************************************************
* Function Name: Step_Counter_SaveConfig
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
*  Step_Counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Step_Counter_SaveConfig(void) 
{
    #if (!Step_Counter_UsingFixedFunction)

        Step_Counter_backup.CounterUdb = Step_Counter_ReadCounter();

        #if(!Step_Counter_ControlRegRemoved)
            Step_Counter_backup.CounterControlRegister = Step_Counter_ReadControlRegister();
        #endif /* (!Step_Counter_ControlRegRemoved) */

    #endif /* (!Step_Counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Step_Counter_RestoreConfig
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
*  Step_Counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Step_Counter_RestoreConfig(void) 
{      
    #if (!Step_Counter_UsingFixedFunction)

       Step_Counter_WriteCounter(Step_Counter_backup.CounterUdb);

        #if(!Step_Counter_ControlRegRemoved)
            Step_Counter_WriteControlRegister(Step_Counter_backup.CounterControlRegister);
        #endif /* (!Step_Counter_ControlRegRemoved) */

    #endif /* (!Step_Counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Step_Counter_Sleep
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
*  Step_Counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Step_Counter_Sleep(void) 
{
    #if(!Step_Counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Step_Counter_CTRL_ENABLE == (Step_Counter_CONTROL & Step_Counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Step_Counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Step_Counter_backup.CounterEnableState = 0u;
        }
    #else
        Step_Counter_backup.CounterEnableState = 1u;
        if(Step_Counter_backup.CounterEnableState != 0u)
        {
            Step_Counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!Step_Counter_ControlRegRemoved) */
    
    Step_Counter_Stop();
    Step_Counter_SaveConfig();
}


/*******************************************************************************
* Function Name: Step_Counter_Wakeup
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
*  Step_Counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Step_Counter_Wakeup(void) 
{
    Step_Counter_RestoreConfig();
    #if(!Step_Counter_ControlRegRemoved)
        if(Step_Counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Step_Counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Step_Counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
