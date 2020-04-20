/*******************************************************************************
* File Name: StepDir.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 StepDir_initVar = 0u;


/*******************************************************************************
* Function Name: StepDir_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void StepDir_Init(void) 
{
        #if (!StepDir_UsingFixedFunction && !StepDir_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!StepDir_UsingFixedFunction && !StepDir_ControlRegRemoved) */
        
        #if(!StepDir_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 StepDir_interruptState;
        #endif /* (!StepDir_UsingFixedFunction) */
        
        #if (StepDir_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            StepDir_CONTROL &= StepDir_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                StepDir_CONTROL2 &= ((uint8)(~StepDir_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                StepDir_CONTROL3 &= ((uint8)(~StepDir_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (StepDir_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                StepDir_CONTROL |= StepDir_ONESHOT;
            #endif /* (StepDir_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            StepDir_CONTROL2 |= StepDir_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            StepDir_RT1 &= ((uint8)(~StepDir_RT1_MASK));
            StepDir_RT1 |= StepDir_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            StepDir_RT1 &= ((uint8)(~StepDir_SYNCDSI_MASK));
            StepDir_RT1 |= StepDir_SYNCDSI_EN;

        #else
            #if(!StepDir_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = StepDir_CONTROL & ((uint8)(~StepDir_CTRL_CMPMODE_MASK));
            StepDir_CONTROL = ctrl | StepDir_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = StepDir_CONTROL & ((uint8)(~StepDir_CTRL_CAPMODE_MASK));
            
            #if( 0 != StepDir_CAPTURE_MODE_CONF)
                StepDir_CONTROL = ctrl | StepDir_DEFAULT_CAPTURE_MODE;
            #else
                StepDir_CONTROL = ctrl;
            #endif /* 0 != StepDir_CAPTURE_MODE */ 
            
            #endif /* (!StepDir_ControlRegRemoved) */
        #endif /* (StepDir_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!StepDir_UsingFixedFunction)
            StepDir_ClearFIFO();
        #endif /* (!StepDir_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        StepDir_WritePeriod(StepDir_INIT_PERIOD_VALUE);
        #if (!(StepDir_UsingFixedFunction && (CY_PSOC5A)))
            StepDir_WriteCounter(StepDir_INIT_COUNTER_VALUE);
        #endif /* (!(StepDir_UsingFixedFunction && (CY_PSOC5A))) */
        StepDir_SetInterruptMode(StepDir_INIT_INTERRUPTS_MASK);
        
        #if (!StepDir_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)StepDir_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            StepDir_WriteCompare(StepDir_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            StepDir_interruptState = CyEnterCriticalSection();
            
            StepDir_STATUS_AUX_CTRL |= StepDir_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(StepDir_interruptState);
            
        #endif /* (!StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void StepDir_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (StepDir_UsingFixedFunction)
        StepDir_GLOBAL_ENABLE |= StepDir_BLOCK_EN_MASK;
        StepDir_GLOBAL_STBY_ENABLE |= StepDir_BLOCK_STBY_EN_MASK;
    #endif /* (StepDir_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!StepDir_ControlRegRemoved || StepDir_UsingFixedFunction)
        StepDir_CONTROL |= StepDir_CTRL_ENABLE;                
    #endif /* (!StepDir_ControlRegRemoved || StepDir_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: StepDir_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  StepDir_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void StepDir_Start(void) 
{
    if(StepDir_initVar == 0u)
    {
        StepDir_Init();
        
        StepDir_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    StepDir_Enable();        
}


/*******************************************************************************
* Function Name: StepDir_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void StepDir_Stop(void) 
{
    /* Disable Counter */
    #if(!StepDir_ControlRegRemoved || StepDir_UsingFixedFunction)
        StepDir_CONTROL &= ((uint8)(~StepDir_CTRL_ENABLE));        
    #endif /* (!StepDir_ControlRegRemoved || StepDir_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (StepDir_UsingFixedFunction)
        StepDir_GLOBAL_ENABLE &= ((uint8)(~StepDir_BLOCK_EN_MASK));
        StepDir_GLOBAL_STBY_ENABLE &= ((uint8)(~StepDir_BLOCK_STBY_EN_MASK));
    #endif /* (StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void StepDir_SetInterruptMode(uint8 interruptsMask) 
{
    StepDir_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: StepDir_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   StepDir_ReadStatusRegister(void) 
{
    return StepDir_STATUS;
}


#if(!StepDir_ControlRegRemoved)
/*******************************************************************************
* Function Name: StepDir_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   StepDir_ReadControlRegister(void) 
{
    return StepDir_CONTROL;
}


/*******************************************************************************
* Function Name: StepDir_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    StepDir_WriteControlRegister(uint8 control) 
{
    StepDir_CONTROL = control;
}

#endif  /* (!StepDir_ControlRegRemoved) */


#if (!(StepDir_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: StepDir_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void StepDir_WriteCounter(uint32 counter) \
                                   
{
    #if(StepDir_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (StepDir_GLOBAL_ENABLE & StepDir_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        StepDir_GLOBAL_ENABLE |= StepDir_BLOCK_EN_MASK;
        CY_SET_REG16(StepDir_COUNTER_LSB_PTR, (uint16)counter);
        StepDir_GLOBAL_ENABLE &= ((uint8)(~StepDir_BLOCK_EN_MASK));
    #else
        CY_SET_REG32(StepDir_COUNTER_LSB_PTR, counter);
    #endif /* (StepDir_UsingFixedFunction) */
}
#endif /* (!(StepDir_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: StepDir_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) The present value of the counter.
*
*******************************************************************************/
uint32 StepDir_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(StepDir_UsingFixedFunction)
		(void)CY_GET_REG16(StepDir_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(StepDir_COUNTER_LSB_PTR_8BIT);
	#endif/* (StepDir_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(StepDir_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(StepDir_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG32(StepDir_STATICCOUNT_LSB_PTR));
    #endif /* (StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint32) Present Capture value.
*
*******************************************************************************/
uint32 StepDir_ReadCapture(void) 
{
    #if(StepDir_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(StepDir_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG32(StepDir_STATICCOUNT_LSB_PTR));
    #endif /* (StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint32) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void StepDir_WritePeriod(uint32 period) 
{
    #if(StepDir_UsingFixedFunction)
        CY_SET_REG16(StepDir_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG32(StepDir_PERIOD_LSB_PTR, period);
    #endif /* (StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) Present period value.
*
*******************************************************************************/
uint32 StepDir_ReadPeriod(void) 
{
    #if(StepDir_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(StepDir_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG32(StepDir_PERIOD_LSB_PTR));
    #endif /* (StepDir_UsingFixedFunction) */
}


#if (!StepDir_UsingFixedFunction)
/*******************************************************************************
* Function Name: StepDir_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void StepDir_WriteCompare(uint32 compare) \
                                   
{
    #if(StepDir_UsingFixedFunction)
        CY_SET_REG16(StepDir_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG32(StepDir_COMPARE_LSB_PTR, compare);
    #endif /* (StepDir_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: StepDir_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint32) Present compare value.
*
*******************************************************************************/
uint32 StepDir_ReadCompare(void) 
{
    return (CY_GET_REG32(StepDir_COMPARE_LSB_PTR));
}


#if (StepDir_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: StepDir_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void StepDir_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    StepDir_CONTROL &= ((uint8)(~StepDir_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    StepDir_CONTROL |= compareMode;
}
#endif  /* (StepDir_COMPARE_MODE_SOFTWARE) */


#if (StepDir_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: StepDir_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void StepDir_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    StepDir_CONTROL &= ((uint8)(~StepDir_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    StepDir_CONTROL |= ((uint8)((uint8)captureMode << StepDir_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (StepDir_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: StepDir_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void StepDir_ClearFIFO(void) 
{

    while(0u != (StepDir_ReadStatusRegister() & StepDir_STATUS_FIFONEMP))
    {
        (void)StepDir_ReadCapture();
    }

}
#endif  /* (!StepDir_UsingFixedFunction) */


/* [] END OF FILE */

