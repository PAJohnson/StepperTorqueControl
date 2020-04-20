/*******************************************************************************
* File Name: main.c
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>
#include "string.h"
#include "stdio.h"
#include "pid.h"

//with direction held high,
//angle increases and shaft rotates clockwise viewed from top

#define SPI_CMD_READ 0x4000 /*!< flag indicating read attempt when using SPI interface */
#define SPI_REG_AGC 0x3ffd /*!< agc register when using SPI */
#define SPI_REG_MAG 0x3ffe /*!< magnitude register when using SPI */
#define SPI_REG_DATA 0x3fff /*!< data register when using SPI */
#define SPI_REG_CLRERR 0x1 /*!< clear error register when using SPI */

#define LIS3DH_ADDRESS    (0x18)
#define FULL_STEP (0u)
#define HALF_STEP (1u)
#define QUARTER_STEP (2u)
#define EIGHTH_STEP (3u)
#define SIXTEENTH_STEP (7u)

uint8 ACC_Read_buffer[10];
uint8 ACC_Write_buffer[8];


volatile uint16 read_angle_command = SPI_CMD_READ | SPI_REG_DATA;

volatile int32 angle;
volatile int32 angle_electric;
volatile int32 angle_diff;
volatile int32 torque_command = 0;

int32 angle_old;
struct accel{
    int16 accel_x;
    int16 accel_y;
    int16 accel_z;
    int16 x_offset;
    int16 y_offset;
    int16 z_offset;
    int32 x_sum;
    int32 y_sum;
    int32 z_sum;
    int16 x_buffer[128];
    int16 y_buffer[128];
    int16 z_buffer[128];
    uint16 x_index;
    uint16 y_index;
    uint16 z_index;
};


volatile uint32 angle_sum;
char transmit_buff[80];
volatile uint8 controller_tick;
volatile int32 axis_position_filtered = 0;
volatile int32 axis_position_filtered_2 = 0;
int16 position;
int32 position_ref = 0;
int32 position_ref_initial = 0;
int32 position_ref_old = 0;
volatile int32 axis_position;
volatile int32 lead_angle = 0;

uint8 spiCalcEvenParity(uint16 value);
void writeRegister8(uint8 reg, uint8 val);
uint8 readRegister8(uint8 reg);
int32 axis_update(int32 axis, int32 angle);
void accel_update(struct accel * acc_struct);
uint32 angle_update(void);
void accel_offset_update(struct accel * acc_struct);

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

uint8 switch_val = 0;
uint8 switch_old = 0;
uint8 step_count = 0;

int32 fullstepMap[200];
int32 fullstepMapSorted[200];
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

int32 min;
int32 min_index;
float split_position;
int32 closest_int;


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  Main function performs following functions:
*   1. Starts Character LCD and print project info
*   2. Starts SPI Master component
*   3. Configures the DMA transfer for RX and TX directions
*   4. Displays the results on Character LCD
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
    uint8 i = 0;
    uint16 j = 0;
    struct accel accel_value;
    pid_ts position_pid;
    CyDelay(200u);
    SPIM_Start();
    UART_1_Start();
    //I2C_1_Start();
    Timer_1_Start();
    ShiftReg_1_Start();
    Microstep_Write(SIXTEENTH_STEP);

    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    StepDir_Start();
    StepDir_Stop();
    StepDir_WritePeriod(2000000000);
    StepDir_Enable();

    ACC_Write_buffer[0] = 0x0F;
    
    read_angle_command |= spiCalcEvenParity(read_angle_command) << 15;
    angle = angle_update();

    PIDConfig(10,0,0,10000,-10000,100,&position_pid);
    
    //SPIM_WriteTxData(read_angle_command);
    //accel initializaton
    //writeRegister8(0x20,0x07);  //enable all axes
    //writeRegister8(0x20, readRegister8(0x20) | (0b0111 << 4)); //400Hz rate
    //writeRegister8(0x23, readRegister8(0x23) | (0b00100000));   //+/-8g range
    
    //accel_update(&accel_value);
    CyGlobalIntEnable;
    tick_Start();
    //angle_electric = axis_update(axis_position,angle_update()) * 3200/16384;
    if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
            position_ref_initial = ADC_DelSig_1_GetResult16();
            position_ref_initial = ((position_ref_initial * 2) - 16384);   
            position_ref_old = position_ref_initial;
    }

    for(;;)
    {
        if(controller_tick == 1){
            Microstep_Write(SIXTEENTH_STEP);
            angle = angle_update();

            controller_tick = 0;
           
            torque_command = PID(axis_position_filtered_2,position_ref,0,&position_pid);
            
            torque_command = 0;
            
            sprintf(transmit_buff,"%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",position_pid.input,position_pid.setpoint,position_pid.output,(int32)position_pid.error_p,(int32)position_pid.error_i,(int32)position_pid.error_d,angle_diff,lead_angle);
            UART_1_PutString(transmit_buff);
            
        }
        //accel_update(&accel_value);
        //angle = angle_update();
        //axis_position = axis_update(axis_position,angle);
        
        
        
        if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
            position_ref = ADC_DelSig_1_GetResult16();
            position_ref = ((position_ref * 2) - 16384);   
        }
        position_ref = (99 * position_ref_old + 1 * position_ref) / 100;
        position_ref_old = position_ref;
        
        if(position_ref > (16384*2-100)){
            position_ref = 16384*2 - 100;
        }
        else if(position_ref < (-16384*2+100)){
            position_ref = -16384*2+100;
        }
        else{
            
        }
//        if(position_ref > 100){
//            position_ref = 40000;  
//        }
//        else if(position_ref < -100){
//            position_ref = 0;
//        }


        
        //full step calibration map
        switch_val = sw1_read_Read();
        if(switch_val == 0 && switch_old == 1){
//            Direction_Write(1);
//    	    Microstep_Write(FULL_STEP);
//            Step_reg_Write(0);
//            Step_reg_Write(2);
//            CyDelay(2);
//            Step_reg_Write(0);
//            CyDelay(100);
//            Direction_Write(0);
//    	    Microstep_Write(FULL_STEP);
//            Step_reg_Write(0);
//            Step_reg_Write(2);
//            CyDelay(2);
//            Step_reg_Write(0);
//            CyDelay(100);
//            for(j = 0; j < 200; j++){
//    			//increment one full step
//    			//wait for 500ms
//    			//take 10 readings, average, output to uart
//    			//increment step count
//                Direction_Write(1);
//    			Microstep_Write(SIXTEENTH_STEP);
//                for(i = 0; i < 16; i++){
//                    Step_reg_Write(0);
//                    CyDelay(2);
//                    Step_reg_Write(2);
//                    CyDelay(2);
//                }
//    			CyDelay(100);
//
//                //flush old angle
//                CyDelay(10);
//                angle_sum = angle_update();
//                CyDelay(10);
//                angle_sum = angle_update();
//                CyDelay(10);
//    			angle_sum = 0;
//    			for(i = 0; i < 100; i++){
//                    angle = angle_update();
//    				angle_sum = angle_sum + angle;
//                    //sprintf(transmit_buff,"i = %d, angle_sum = %ld \n",i,angle);
//                    //UART_1_PutString(transmit_buff);
//    				CyDelay(1);
//    			}
//    			angle = angle_sum / 100;
//                fullstepMap[j] = angle;
//                //sprintf(transmit_buff,"%d,%ld\n",step_count,angle);
//                //UART_1_PutString(transmit_buff);
//                step_count++;
//            }
//            //linearization map
//            //rotate fullstepMap array to be from low to high
//            min = fullstepMap[0];
//            for(j = 0; j < 200; j++){
//                if(fullstepMap[j] < min){
//                    min = fullstepMap[j];
//                    min_index = j;
//                }
//            }
//            for(j = 0; j < 200; j++){
//                fullstepMapSorted[j] = fullstepMap[(j+min_index)%200];
//                fullstepMapSorted[j] = (min + 1.8 * j * 16384.0 / 360.0) - fullstepMapSorted[j];
//                sprintf(transmit_buff,"%ld,\n",fullstepMapSorted[j]);
//                UART_1_PutString(transmit_buff);
//            }
//            for(j = 0; j < 512; j++){
//                //dont bother with trying to interpolate some strange way
//                //upsample by 256, downsample by 100
//                //128 50
//                //64 25
//                //could do this by scaling points
//                //what does the upsample look like?
//                //for(j = 0; j < 12800; j++){
//                //    upsample[j] = og[(j*25)/64] + ((og[(j*25)/64 + 1] - og[(j*25)/64]) * ((j*25)%64)) / 64;
//                //}
//                //offsetMap[j] = upsample[j*25]
//                //if j < 507
//                offsetMap[j] = fullstepMapSorted[j*25/64];
//                sprintf(transmit_buff,"%d,%ld\n",j,offsetMap[j]);
//                UART_1_PutString(transmit_buff);
//                //else offsetMap[j] = offsetMap[506]
//            }
//            //sprintf(transmit_buff,"int32 offsetMap[512] = {");
//            //UART_1_PutString(transmit_buff);
//            //for(j = 0; j < 511; j++){
//            //    sprintf(transmit_buff,"%ld,\n",offsetMap[j]);   
//            //    UART_1_PutString(transmit_buff);
//            //}
//            //sprintf(transmit_buff,"%ld };",offsetMap[511]);
//            //UART_1_PutString(transmit_buff);
        }
        
        switch_old = switch_val;
//        sprintf(transmit_buff,"%ld\n",angle_update());
//        UART_1_PutString(transmit_buff);
//        ShiftReg_1_Stop();
//        ShiftReg_1_WriteRegValue(0x00AA);
//        ShiftReg_1_Enable();

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

void writeRegister8(uint8 reg, uint8 val){
    I2C_1_MasterSendStart(LIS3DH_ADDRESS,I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(reg);
    I2C_1_MasterWriteByte(val);    //enable all axes, normal mode
    I2C_1_MasterSendStop();
}

uint8 readRegister8(uint8 reg){
    uint8 read;
    I2C_1_MasterSendStart(LIS3DH_ADDRESS,I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(reg);
    I2C_1_MasterSendStop();
    
    I2C_1_MasterSendStart(LIS3DH_ADDRESS,I2C_1_READ_XFER_MODE);
    read = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    I2C_1_MasterSendStop();
    return read;
}

int32 axis_update(int32 axis, int32 angle){
    static int32 angle_old;
    int32 axis_position;
    if(angle - angle_old > 10000){
        //angle decreasing, crossed over 0
        axis_position = axis + (angle - (angle_old + 16384));
    }
    else if(angle - angle_old < -10000){
        //angle increasing, crossed over 0
        axis_position = axis + ((angle - angle_old) + 16384) % 16384;
    }
    else{
        axis_position = axis + angle - angle_old;
    }
    angle_old = angle;
    angle_old = angle;
    return axis;
}

void accel_update(struct accel * acc_struct){
    //acc_struct->accel_x = readRegister8(0x28);
    //acc_struct->accel_x |= (readRegister8(0x29) << 8);
    acc_struct->accel_y = readRegister8(0x2A);
    acc_struct->accel_y |= (readRegister8(0x2B) << 8);
    //acc_struct->accel_z = readRegister8(0x2C);
    //acc_struct->accel_z |= (readRegister8(0x2D) << 8);  
    return;
}

uint32 angle_update(void){
    //int32 angle;
    //SPIM_WriteTxData(read_angle_command);
    //if(SPIM_GetRxBufferSize() != 0){
    //    angle = SPIM_ReadRxData() & 0x3FFF; //((SPIM_ReadRxData() & 0x3FFF) * 3600)/16384;
    //}
    return angle;
}

void accel_offset_update(struct accel * acc_struct){
    
//    acc_struct->x_sum -= acc_struct->x_buffer[acc_struct->x_index];
//    acc_struct->x_buffer[acc_struct->x_index] = acc_struct->accel_x;
//    acc_struct->x_sum += acc_struct->x_buffer[acc_struct->x_index];
//    acc_struct->x_index = (acc_struct->x_index + 1) % 128;
//    acc_struct->x_offset = acc_struct->x_sum / 128;
//    
//    acc_struct->y_sum -= acc_struct->y_buffer[acc_struct->y_index];
//    acc_struct->y_buffer[acc_struct->y_index] = acc_struct->accel_y;
//    acc_struct->y_sum += acc_struct->y_buffer[acc_struct->y_index];
//    acc_struct->y_index = (acc_struct->y_index + 1) % 128;
//    acc_struct->y_offset = acc_struct->y_sum / 128;
//    
//    acc_struct->z_sum -= acc_struct->z_buffer[acc_struct->z_index];
//    acc_struct->z_buffer[acc_struct->z_index] = acc_struct->accel_z;
//    acc_struct->z_sum += acc_struct->z_buffer[acc_struct->z_index];
//    acc_struct->z_index = (acc_struct->z_index + 1) % 128;
//    acc_struct->z_offset = acc_struct->z_sum / 128;
    acc_struct->y_offset = (acc_struct->y_offset * 120 + acc_struct->accel_y * 8)/128;
}

/* [] END OF FILE */
