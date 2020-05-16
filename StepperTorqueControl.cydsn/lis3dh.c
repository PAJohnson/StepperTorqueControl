/* ========================================
 * Helper functions for LIS3DH I2C accelerometer
 * ========================================
*/

#include "lis3dh.h"

void writeRegister8(uint8 reg, uint8 val){
    //this function writes "val" to register "reg"
    
    I2C_1_MasterSendStart(LIS3DH_ADDRESS,I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(reg);
    I2C_1_MasterWriteByte(val);
    I2C_1_MasterSendStop();
}

uint8 readRegister8(uint8 reg){
    //Do an I2C transaction, writing the desired register address and 
    //another transaction to read the data at that address
    uint8 read;
    I2C_1_MasterSendStart(LIS3DH_ADDRESS,I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(reg);
    I2C_1_MasterSendStop();
    
    I2C_1_MasterSendStart(LIS3DH_ADDRESS,I2C_1_READ_XFER_MODE);
    read = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    I2C_1_MasterSendStop();
    return read;
}

void accel_update(struct Accel * acc_struct){
    //read all three axes of the accelerometer
    
    acc_struct->accel_x = readRegister8(0x28);
    acc_struct->accel_x |= (readRegister8(0x29) << 8);
    acc_struct->accel_y = readRegister8(0x2A);
    acc_struct->accel_y |= (readRegister8(0x2B) << 8);
    acc_struct->accel_z = readRegister8(0x2C);
    acc_struct->accel_z |= (readRegister8(0x2D) << 8);  
    return;
}

/* [] END OF FILE */
