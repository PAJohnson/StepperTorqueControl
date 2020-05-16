/* ========================================
 * Helper functions for LIS3DH I2C accelerometer
 * ========================================
*/

#ifndef LIS3DH_H
#define LIS3DH_H
    
#include "cytypes.h"
#include "I2C_1.h"

#define LIS3DH_ADDRESS    (0x18)

struct Accel{
    int16 accel_x;
    int16 accel_y;
    int16 accel_z;
    int16 x_offset;
    int16 y_offset;
    int16 z_offset;
};

void writeRegister8(uint8 reg, uint8 val);
uint8 readRegister8(uint8 reg);

void accel_update(struct Accel * acc_struct);

/* example usage
    struct Accel accel
    writeRegister8(0x20,0x07);  //enable all axes
    writeRegister8(0x20, readRegister8(0x20) | (0b0111 << 4)); //400Hz rate
    writeRegister8(0x23, readRegister8(0x23) | (0b00100000));   //+/-8g range
    accel_update(&accel);
*/
#endif

/* [] END OF FILE */
