#include <Arduino.h>
#include <Wire.h>
#pragma once

enum ICM20602_registers
{
    XG_OFFS_TC_H	    =   0x04,
 	XG_OFFS_TC_L	    =   0x05,
 	YG_OFFS_TC_H	    =   0x07,
 	YG_OFFS_TC_L	    =   0x08,
 	ZG_OFFS_TC_H	    =   0x0A,
 	ZG_OFFS_TC_L	    =   0x0B,
 	SELF_TEST_X_ACCEL	=   0x0D,
 	SELF_TEST_Y_ACCEL	=   0x0E,
 	SELF_TEST_Z_ACCEL	=   0x0F,
 	XG_OFFS_USRH	    =   0x13,
 	XG_OFFS_USRL	    =   0x14,
 	YG_OFFS_USRH	    =   0x15,
 	YG_OFFS_USRL	    =   0x16,
 	ZG_OFFS_USRH	    =   0x17,
 	ZG_OFFS_USRL	    =   0x18,
 	SMPLRT_DIV	        =   0x19,
 	CONFIG	            =   0x1A,   
 	GYRO_CONFIG	        =   0x1B,
 	ACCEL_CONFIG	    =   0x1C,
 	ACCEL_CONFIG2	    =   0x1D,
 	LP_MODE_CFG	        =   0x1E,
 	ACCEL_WOM_X_THR	    =   0x20,
 	ACCEL_WOM_Y_THR	    =   0x21,
 	ACCEL_WOM_Z_THR	    =   0x22,
 	FIFO_EN	            =   0x23,
 	FSYNC_INT	        =   0x36,
 	INT_PIN_CFG	        =   0x37,
 	INT_ENABLE	        =   0x38,
 	FIFO_WM_INT_STATUS	=   0x39,
 	INT_STATUS	        =   0x3A,
 	ACCEL_XOUT_H	    =   0x3B,
 	ACCEL_XOUT_L	    =   0x3C,
 	ACCEL_YOUT_H	    =   0x3D,
 	ACCEL_YOUT_L	    =   0x3E,
 	ACCEL_ZOUT_H	    =   0x3F,
 	ACCEL_ZOUT_L	    =   0x40,
 	TEMP_OUT_H	        =   0x41,
 	TEMP_OUT_L	        =   0x42,
 	GYRO_XOUT_H	        =   0x43,
 	GYRO_XOUT_L         =   0x44,
 	GYRO_YOUT_H	        =   0x45,
 	GYRO_YOUT_L	        =   0x46,
 	GYRO_ZOUT_H	        =   0x47,
 	GYRO_ZOUT_L	        =   0x48,
 	SELF_TEST_X_GYRO	=   0x50,
 	SELF_TEST_Y_GYRO	=   0x51,
 	SELF_TEST_Z_GYRO	=   0x52,
 	FIFO_WM_TH1	        =   0x60,
 	FIFO_WM_TH2	        =   0x61,
 	SIGNAL_PATH_RESET	=   0x68,
 	ACCEL_INTEL_CTRL	=   0x69,
 	USER_CTRL	        =   0x6A,
 	PWR_MGMT_1	        =   0x41,
 	PWR_MGMT_2	        =   0x6C,
 	I2C_IF	            =   0x70,
 	FIFO_COUNTH	        =   0x72,
 	FIFO_COUNTL	        =   0x73,
 	FIFO_R_W	        =   0x74,
 	WHO_AM_I	        =	0x12, 
 	XA_OFFSET_H			=	0x77,
 	XA_OFFSET_L			=	0x78,
 	YA_OFFSET_H		    =	0x7A,
 	YA_OFFSET_L			=	0x7B,
 	ZA_OFFSET_H			=	0x7D,
 	ZA_OFFSET_L			=	0x7E,
};

class ICM20602
{
private:
    uint8_t Addr;
    void write8(byte reg, byte value) ;
    uint8_t read8(byte reg);
    
public:
    ICM20602(bool Addr); //True = 69, False = 68

};



