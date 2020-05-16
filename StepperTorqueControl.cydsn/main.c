/*******************************************************************************
* File Name: main.c
* Patrick Johnson 2020
* Stepper motor controller
* Implementation of this paper: https://www.mdpi.com/2079-9292/7/10/242/pdf
*******************************************************************************/

#include <project.h>
#include "string.h"
#include "stdio.h"
#include "macros.h"
#include "pid.h"
#include "lis3dh.h"

//#define DEBUG_SERIAL

volatile uint16 read_angle_command = SPI_CMD_READ | SPI_REG_DATA;

volatile int32 angle;
volatile int32 angle_electric;
volatile int32 angle_diff;
volatile int32 torque_command = 0;
volatile uint8 controller_tick;
volatile int32 axis_position_filtered = 0;
volatile int32 axis_position_filtered_2 = 0;
volatile int32 axis_position;
volatile int32 lead_angle = 0;

char transmit_buff[80];
int32 position_ref = 0;
int32 position_ref_initial = 0;
int32 position_ref_old = 0;

uint8 spiCalcEvenParity(uint16 value);

//step pulses are sent using a shift register to avoid bitbanging
//this array converts a number of pulses to a bitstream
// step_array[4] = 0b10101010 = 0xAA
volatile uint32 step_array[17] = {   0x00000000,
                            0x00000002,
                            0x0000000A,
                            0x0000002A,
                            0x000000AA,
                            0x000002AA,
                            0x00000AAA,
                            0x00002AAA,
                            0x0000AAAA,
                            0x0002AAAA,
                            0x000AAAAA,
                            0x002AAAAA,
                            0x00AAAAAA,
                            0x02AAAAAA,
                            0x0AAAAAAA,
                            0x2AAAAAAA,
                            0xAAAAAAAA  };

//offset map generated manually
//This corrects for encoder error vs motor full step positions.
volatile int32 offsetMap[512] = {0,
0,
0,
-3,
-3,
-3,
-2,
-2,
-6,
-6,
-6,
-3,
-3,
-5,
-5,
-5,
-5,
-5,
-8,
-8,
-8,
-6,
-6,
-6,
-8,
-8,
-7,
-7,
-7,
-10,
-10,
-8,
-8,
-8,
-11,
-11,
-10,
-10,
-10,
-13,
-13,
-11,
-11,
-11,
-12,
-12,
-12,
-12,
-12,
-15,
-15,
-15,
-12,
-12,
-14,
-14,
-14,
-12,
-12,
-15,
-15,
-15,
-13,
-13,
-14,
-14,
-14,
-13,
-13,
-13,
-15,
-15,
-13,
-13,
-13,
-14,
-14,
-13,
-13,
-13,
-15,
-15,
-12,
-12,
-12,
-12,
-12,
-12,
-11,
-11,
-13,
-13,
-13,
-10,
-10,
-11,
-11,
-11,
-10,
-10,
-12,
-12,
-12,
-9,
-9,
-9,
-9,
-9,
-8,
-8,
-8,
-9,
-9,
-7,
-7,
-7,
-7,
-7,
-5,
-5,
-5,
-7,
-7,
-4,
-4,
-4,
-4,
-4,
-4,
-4,
-4,
-5,
-5,
-5,
-3,
-3,
-2,
-2,
-2,
-1,
-1,
-3,
-3,
-3,
0,
0,
0,
0,
0,
1,
1,
1,
0,
0,
2,
2,
2,
2,
2,
4,
4,
4,
2,
2,
5,
5,
5,
4,
4,
6,
6,
6,
5,
5,
5,
8,
8,
7,
7,
7,
9,
9,
8,
8,
8,
11,
11,
10,
10,
10,
11,
11,
10,
10,
10,
12,
12,
12,
11,
11,
13,
13,
13,
11,
11,
13,
13,
13,
12,
12,
13,
13,
13,
11,
11,
11,
14,
14,
12,
12,
12,
14,
14,
11,
11,
11,
12,
12,
11,
11,
11,
12,
12,
9,
9,
9,
11,
11,
11,
9,
9,
10,
10,
10,
7,
7,
8,
8,
8,
7,
7,
6,
6,
6,
4,
4,
6,
6,
6,
3,
3,
3,
2,
2,
0,
0,
0,
1,
1,
0,
0,
0,
0,
0,
-3,
-3,
-3,
-2,
-2,
-2,
-4,
-4,
-4,
-4,
-4,
-7,
-7,
-6,
-6,
-6,
-9,
-9,
-9,
-9,
-9,
-12,
-12,
-10,
-10,
-10,
-12,
-12,
-12,
-11,
-11,
-14,
-14,
-14,
-13,
-13,
-14,
-14,
-14,
-14,
-14,
-17,
-17,
-17,
-15,
-15,
-17,
-17,
-17,
-16,
-16,
-16,
-19,
-19,
-17,
-17,
-17,
-18,
-18,
-17,
-17,
-17,
-20,
-20,
-17,
-17,
-17,
-19,
-19,
-19,
-17,
-17,
-20,
-20,
-20,
-17,
-17,
-18,
-18,
-18,
-18,
-18,
-20,
-20,
-20,
-18,
-18,
-19,
-19,
-19,
-17,
-17,
-17,
-20,
-20,
-17,
-17,
-17,
-17,
-17,
-16,
-16,
-16,
-18,
-18,
-16,
-16,
-16,
-16,
-16,
-15,
-15,
-15,
-17,
-17,
-17,
-15,
-15,
-15,
-15,
-15,
-14,
-14,
-15,
-15,
-15,
-12,
-12,
-13,
-13,
-13,
-11,
-11,
-11,
-12,
-12,
-10,
-10,
-10,
-10,
-10,
-8,
-8,
-8,
-11,
-11,
-7,
-7,
-7,
-8,
-8,
-6,
-6,
-6,
-7,
-7,
-7,
-4,
-4,
-5,
-5,
-5,
-3,
-3,
-4,
-4,
-4,
-1,
-1,
-2,
-2,
-2,
0,
0,
-2,
-2,
-2,
0,
0,
0,
0,
0,
1,
1,
1,
0,
0,
2,
2,
2,
1,
1,
3,
3,
3,
1,
1,
1,
3,
3,
2,
2,
2,
3,
3,
2,
2,
2,
3,
3,
2,
2,
2,
3,
3,
0,
0,
0,
3,
3,
3,
1,
1,
2,
2,
2,
0,
0,
1,
1,
1,
0,
0,
0,
0,
0,
-1,
-1 };



/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* 
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    pid_ts position_pid;
    CyDelay(200u);
    
    //Start components
    SPIM_Start();
    UART_1_Start();
    Timer_1_Start();
    ShiftReg_1_Start();
    Microstep_Write(SIXTEENTH_STEP);

    CyDelay(10);
    
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    
    //StepDir not currently used
    //Hardware up and down counter to be used as external step/dir interface
    StepDir_Start();
    StepDir_Stop();
    StepDir_WritePeriod(2000000000);
    StepDir_Enable();
    
    //initialize angle command - see datasheet for AS5048
    read_angle_command |= spiCalcEvenParity(read_angle_command) << 15;

    PIDConfig(10,0,0,10000,-10000,100,&position_pid);
    
    CyGlobalIntEnable;
    tick_Start();
    
    if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
            position_ref_initial = ADC_DelSig_1_GetResult16();
            position_ref_initial = ((position_ref_initial * 2) - 16384);   
            position_ref_old = position_ref_initial;
    }

    for(;;)
    {
        if(controller_tick == 1){
            Microstep_Write(SIXTEENTH_STEP);

            controller_tick = 0;
           
            //PID loop on position. Outputs a torque command.
            torque_command = PID(axis_position_filtered_2,position_ref,0,&position_pid);

            #ifdef DEBUG_SERIAL
            sprintf(transmit_buff,"%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",position_pid.input,position_pid.setpoint,position_pid.output,(int32)position_pid.error_p,(int32)position_pid.error_i,(int32)position_pid.error_d,angle_diff,lead_angle);
            UART_1_PutString(transmit_buff);
            #endif
            
        }
        
        if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
            position_ref = ADC_DelSig_1_GetResult16();
            position_ref = ((position_ref * 2) - 16384); //allow commanding positive and negative offset from initial position
        }
        position_ref = (99 * position_ref_old + 1 * position_ref) / 100; //filtering, ADC is noisy!
        position_ref_old = position_ref;
        
        //DelSig ADC will give values that are out of range for 16 bits. Saturate at limits.
        if(position_ref > (16384*2-100)){
            position_ref = 16384*2 - 100;
        }
        else if(position_ref < (-16384*2+100)){
            position_ref = -16384*2+100;
        }
        else{
            ;   
        }
    }
}

uint8 spiCalcEvenParity(uint16 value)
{
    uint8 cnt = 0;
    uint8 i;
    for (i = 0; i < 16; i++)
    {
        if (value & 0x1)
        {
        cnt++;
        }
    value >>= 1;
    }
    return cnt & 0x1;
}

/* [] END OF FILE */
