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
#include "pid.h"
#include "stdio.h"
#include "CyLib.h"

int8 PIDConfig(int32 kp, int32 ki, int32 kd, int32 i_limit_pos, int32 i_limit_neg, int32 fs, pid_ts* PID){
    PID->kp = kp;
    PID->ki = ki;
    PID->kd = kd;
    PID->i_limit_pos = i_limit_pos;
    PID->i_limit_neg = i_limit_neg;
    PID->fs = fs;
    PID->setpoint = 0;
    PID->input = 0;
    PID->error_d = 0;
    PID->error_i = 0;
    PID->error_last = 0;
    PID->error_p = 0;
    PID->feedforward = 0;
    PID->output = 0;
    return 0;
}

int8 PIDReset(pid_ts* PID){
    PID->setpoint = 0;
    PID->input = 0;
    PID->error_i = 0;
    PID->output = 0;
    PID->feedforward = 0;
    
    return 0;
}

int32 PID(int32 input, int32 setpoint, int32 feedforward, pid_ts* PID){
    //PID function in integer math
    int32 error;
    int32 error_i;
    
    PID->input = input;
    PID->setpoint = setpoint;
    PID->feedforward = feedforward;
    
    error = PID->setpoint - PID->input;
    
    PID->error_p = (error * PID->kp) / 1000;
    error_i = (error * PID->ki) / 1000;
    
    //anti-windup logic
    if(PID->error_i + error_i < PID->i_limit_neg){
        PID->error_i = PID->i_limit_neg;   
    }
    else if(PID->error_i + error_i > PID->i_limit_pos){
        PID->error_i = PID->i_limit_pos;   
    }
    else{
        PID->error_i = PID->error_i + error_i;   
    }
    PID->error_d = (error - PID->error_last) * PID->kd / 1000;
    PID->error_last = error;
    
    PID->output = PID->feedforward + PID->error_p + PID->error_i + PID->error_d;
    
    return PID->output;
}

uint32 PIDStatus(pid_ts* PID){
    return PID->status;
}

/* [] END OF FILE */
