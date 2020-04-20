/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef PID_H
#define PID_H
    
#include <stdio.h>
#include <CyLib.h>
    
typedef struct pid_ts{
    int32 input;
    int32 setpoint;
    int32 output;
    int32 feedforward;
    int32 kp;
    int32 ki;
    int32 kd;
    int32 i_limit_pos;
    int32 i_limit_neg;
    int64 error_p;
    int64 error_i;
    int64 error_d;
    int32 fs;
    int64 error_last;
    uint32 status;
} pid_ts;
    
int8 PIDConfig(int32 kp, int32 ki, int32 kd, int32 i_limit_pos, int32 i_limit_neg, int32 fs, pid_ts* PID);
int8 PIDReset(pid_ts* PID);
int32 PID(int32 input, int32 setpoint, int32 feedforward, pid_ts* PID);
uint32 PIDStatus(pid_ts* PID);
    
#endif

/* [] END OF FILE */
