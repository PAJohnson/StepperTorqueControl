/*******************************************************************************
* File Name: tick.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <tick.h>
#include "cyapicallbacks.h"

#if !defined(tick__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START tick_intc` */
    #include "SPIM.h"
    #include "ShiftReg_1.h"
    #include "Direction.h"
    #include "Interrupt_t.h"
    #define TORQUE_CTRL
    extern volatile uint8 controller_tick;
    extern volatile int32 angle;
    extern volatile uint16 read_angle_command;
    extern volatile int32 lead_angle;
    extern volatile uint32 step_array[17];
    extern volatile int32 angle_electric;
    extern volatile int32 angle_diff;
    extern volatile int32 torque_command;
    extern volatile int32 axis_position;
    extern volatile int32 offsetMap[512];
    volatile int32 angle_old = 0;
    volatile int32 axis_position_old = 0;
    extern volatile int32 axis_position_filtered;
    int start_count;
    int count;
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: tick_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_Start(void)
{
    /* For all we know the interrupt is active. */
    tick_Disable();

    /* Set the ISR to point to the tick Interrupt. */
    tick_SetVector(&tick_Interrupt);

    /* Set the priority. */
    tick_SetPriority((uint8)tick_INTC_PRIOR_NUMBER);

    /* Enable it. */
    tick_Enable();
}


/*******************************************************************************
* Function Name: tick_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void tick_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    tick_Disable();

    /* Set the ISR to point to the tick Interrupt. */
    tick_SetVector(address);

    /* Set the priority. */
    tick_SetPriority((uint8)tick_INTC_PRIOR_NUMBER);

    /* Enable it. */
    tick_Enable();
}


/*******************************************************************************
* Function Name: tick_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_Stop(void)
{
    /* Disable this interrupt. */
    tick_Disable();

    /* Set the ISR to point to the passive one. */
    tick_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: tick_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for tick.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(tick_Interrupt)
{
    #ifdef tick_INTERRUPT_INTERRUPT_CALLBACK
        tick_Interrupt_InterruptCallback();
    #endif /* tick_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START tick_Interrupt` */
        
        //sample the angle sensor
        //send read command after checking for data
        //interrupt takes less time this way (hopefully)
        if(SPIM_GetRxBufferSize() != 0){
            angle = SPIM_ReadRxData() & 0x3FFF; //((SPIM_ReadRxData() & 0x3FFF) * 3600)/16384;
            angle = angle + offsetMap[angle/32];
        }
        SPIM_WriteTxData(read_angle_command);
        
        axis_position_old = axis_position;
        
        //calculate axis position from shaft angle position
        if(angle - angle_old > 10000){
            //angle decreasing, crossed over 0
            axis_position = axis_position + (angle - (angle_old + 16384));
        }
        else if(angle - angle_old < -10000){
            //angle increasing, crossed over 0
            axis_position = axis_position + ((angle - angle_old) + 16384) % 16384;
        }
        else{
            axis_position = axis_position + angle - angle_old;
        }
        angle_old = angle;
        
        //a touch of filtering
        axis_position_filtered = (axis_position_filtered*5 + axis_position*3)/8;
        
        if(start_count < 100){
            start_count++;   
        }
        #ifdef TORQUE_CTRL
        if(start_count == 100){
        count = (count + 1)%10;
        if(count == 0){
            controller_tick = 1;
        }
        
        
        angle_diff = angle_electric - (axis_position_filtered * 3200) / 16384;
        
        lead_angle = torque_command - angle_diff;
            if(lead_angle > 15){
                lead_angle = 15;
            }
            else if(lead_angle < -15){
                lead_angle = -15;
            }
            if(lead_angle > 0){
                Direction_Write(1);
                CyDelayUs(1);
                ShiftReg_1_Stop();
                ShiftReg_1_WriteRegValue(step_array[lead_angle]);
                ShiftReg_1_Enable();
                angle_electric += lead_angle;
            }
            else if(lead_angle < 0){
                Direction_Write(0);
                CyDelayUs(1);
                lead_angle *= -1;
                ShiftReg_1_Stop();
                ShiftReg_1_WriteRegValue(step_array[lead_angle]);
                ShiftReg_1_Enable();
                angle_electric -= lead_angle;
            }
            else{
                
            }
        }
        else{
            angle_electric = axis_position * 3200 / 16384;   
            axis_position_filtered = axis_position;
        }
        #endif
        
    /* `#END` */
}


/*******************************************************************************
* Function Name: tick_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling tick_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use tick_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void tick_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)tick__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: tick_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress tick_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)tick__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: tick_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling tick_Start or tick_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after tick_Start or tick_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void tick_SetPriority(uint8 priority)
{
    *tick_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: tick_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 tick_GetPriority(void)
{
    uint8 priority;


    priority = *tick_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: tick_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_Enable(void)
{
    /* Enable the general interrupt. */
    *tick_INTC_SET_EN = tick__INTC_MASK;
}


/*******************************************************************************
* Function Name: tick_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 tick_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*tick_INTC_SET_EN & (uint32)tick__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: tick_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_Disable(void)
{
    /* Disable the general interrupt. */
    *tick_INTC_CLR_EN = tick__INTC_MASK;
}


/*******************************************************************************
* Function Name: tick_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void tick_SetPending(void)
{
    *tick_INTC_SET_PD = tick__INTC_MASK;
}


/*******************************************************************************
* Function Name: tick_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_ClearPending(void)
{
    *tick_INTC_CLR_PD = tick__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
