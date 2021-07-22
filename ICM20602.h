#include "main.h"
#include "stm32f4xx_hal.h"

//---------------------Connecting----------//
extern SPI_HandleTypeDef hspi1;
#define ICM20602_SPI 			(&hspi1)
#define ICM20602_CS_LOW  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define ICM20602_CS_HIGH 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
//---------------------Connecting----------//

/***** Defines *****/

#define ICM20602_XG_OFFS_TC_H 0x04
#define ICM20602_XG_OFFS_TC_L 0x05
#define ICM20602_YG_OFFS_TC_H 0x07
#define ICM20602_YG_OFFS_TC_L 0x08
#define ICM20602_ZG_OFFS_TC_H 0x0A
#define ICM20602_ZG_OFFS_TC_L 0x0B
#define ICM20602_SELF_TEST_X_ACCEL 0x0D
#define ICM20602_SELF_TEST_Y_ACCEL 0x0E
#define ICM20602_SELF_TEST_Z_ACCEL 0x0F
#define ICM20602_XG_OFFS_USRH 0x13
#define ICM20602_XG_OFFS_USRL 0x14
#define ICM20602_YG_OFFS_USRH 0x15
#define ICM20602_YG_OFFS_USRL 0x16
#define ICM20602_ZG_OFFS_USRH 0x17
#define ICM20602_ZG_OFFS_USRL 0x18
#define ICM20602_SMPLRT_DIV 0x19
#define ICM20602_CONFIG 0x1A
#define ICM20602_GYRO_CONFIG 0x1B
#define ICM20602_ACCEL_CONFIG 0x1C
#define ICM20602_ACCEL_CONFIG_2 0x1D
#define ICM20602_LP_MODE_CFG 0x1E
#define ICM20602_ACCEL_WOM_X_THR 0x20
#define ICM20602_ACCEL_WOM_Y_THR 0x21
#define ICM20602_ACCEL_WOM_Z_THR 0x22
#define ICM20602_FIFO_EN 0x23
#define ICM20602_FSYNC_INT 0x36
#define ICM20602_INT_PIN_CFG 0x37
#define ICM20602_INT_ENABLE 0x38
#define ICM20602_FIFO_WM_INT_STATUS 0x39
#define ICM20602_INT_STATUS 0x3A
#define ICM20602_ACCEL_XOUT_H 0x3B
#define ICM20602_ACCEL_XOUT_L 0x3C
#define ICM20602_ACCEL_YOUT_H 0x3D
#define ICM20602_ACCEL_YOUT_L 0x3E
#define ICM20602_ACCEL_ZOUT_H 0x3F
#define ICM20602_ACCEL_ZOUT_L 0x40
#define ICM20602_TEMP_OUT_H 0x41
#define ICM20602_TEMP_OUT_L 0x42
#define ICM20602_GYRO_XOUT_H 0x43
#define ICM20602_GYRO_XOUT_L 0x44
#define ICM20602_GYRO_YOUT_H 0x45
#define ICM20602_GYRO_YOUT_L 0x46
#define ICM20602_GYRO_ZOUT_H 0x47
#define ICM20602_GYRO_ZOUT_L 0x48
#define ICM20602_SELF_TEST_X_GYRO 0x50
#define ICM20602_SELF_TEST_Y_GYRO 0x51
#define ICM20602_SELF_TEST_Z_GYRO 0x52
#define ICM20602_FIFO_WM_TH1 0x60
#define ICM20602_FIFO_WM_TH2 0x61
#define ICM20602_SIGNAL_PATH_RESET 0x68
#define ICM20602_ACCEL_INTEL_CTRL 0x69
#define ICM20602_USER_CTRL 0x6A
#define ICM20602_PWR_MGMT_1 0x6B
#define ICM20602_PWR_MGMT_2 0x6C
#define ICM20602_I2C_IF 0x70
#define ICM20602_FIFO_COUNTH 0x72
#define ICM20602_FIFO_COUNTL 0x73
#define ICM20602_FIFO_R_W 0x74
#define ICM20602_WHO_AM_I 0x75
#define ICM20602_XA_OFFSET_H 0x77
#define ICM20602_XA_OFFSET_L 0x78
#define ICM20602_YA_OFFSET_H 0x7A
#define ICM20602_YA_OFFSET_L 0x7B
#define ICM20602_ZA_OFFSET_H 0x7D
#define ICM20602_ZA_OFFSET_L 0x7E

#define ICM20602_WHO_AM_I_CONST 0X12

#define ICM20602_ACCEL_DLPF_218_1_HZ   			0x00  
#define ICM20602_ACCEL_DLPF_99_HZ   				0x02  
#define ICM20602_ACCEL_DLPF_44_8_HZ   			0x03  
#define ICM20602_ACCEL_DLPF_21_2_HZ  			 	0x04  
#define ICM20602_ACCEL_DLPF_10_2_HZ   			0x05  
#define ICM20602_ACCEL_DLPF_5_1_HZ 				  0x06  
#define ICM20602_ACCEL_DLPF_420_HZ   				0x07  
#define ICM20602_ACCEL_DLPF_BYPASS_1046_HZ 	0x08 

#define ICM20602_ACCEL_RANGE_2G   0x00 
#define ICM20602_ACCEL_RANGE_4G   0x08
#define ICM20602_ACCEL_RANGE_8G   0x10
#define ICM20602_ACCEL_RANGE_16G  0x18 

#define ICM20602_GYRO_DLPF_250_HZ   				0x00  
#define ICM20602_GYRO_DLPF_176_HZ  					0x01  
#define ICM20602_GYRO_DLPF_92_HZ   					0x02  
#define ICM20602_GYRO_DLPF_41_HZ  		 			0x03  
#define ICM20602_GYRO_DLPF_20_HZ   					0x04  
#define ICM20602_GYRO_DLPF_10_HZ   					0x05  
#define ICM20602_GYRO_DLPF_5_HZ   					0x06 
#define ICM20602_GYRO_DLPF_3281_HZ   				0x07 
#define ICM20602_GYRO_DLPF_BYPASS_3281_HZ   0x08		
#define ICM20602_GYRO_DLPF_BYPASS_8173_HZ   0x00		

#define ICM20602_GYRO_RANGE_250_DPS   0x00 
#define ICM20602_GYRO_RANGE_500_DPS   0x08 
#define ICM20602_GYRO_RANGE_1000_DPS  0x10
#define ICM20602_GYRO_RANGE_2000_DPS  0x18

/***** Defines *****/
//---------------------Variables----------//

typedef struct{  // main structure
volatile uint8_t acs_xH;
volatile uint8_t acs_xL;
volatile uint8_t acs_yH;
volatile uint8_t acs_yL;
volatile uint8_t acs_zH;
volatile uint8_t acs_zL;	
volatile uint8_t gyro_xH;
volatile uint8_t gyro_xL;
volatile uint8_t gyro_yH;
volatile uint8_t gyro_yL;
volatile uint8_t gyro_zH;
volatile uint8_t gyro_zL;
volatile uint8_t null[6];	
}ICM20602_transfer;

typedef struct { // structure for read/write offset
volatile int16_t x_ofset;
volatile int16_t y_ofset;
volatile int16_t z_ofset;	
}ICM20602_ofset;

//---------------------Variables----------//

//---------------------Functions----------//
int8_t ICM20602_readSensors(ICM20602_transfer *dat); 													// Read accelerometer&gyroscope
int8_t ICM20602_Init(void); 																					// Init function
uint8_t ICM20602_WHO_I_AM(void); 																			// Read ID ICM20602 (return 0x12)
int8_t ICM20602_readOffset(ICM20602_ofset *accel_ofset,ICM20602_ofset *gyro_ofset); 		// Read offset accelerometer&gyroscope
int8_t ICM20602_writeOffset(ICM20602_ofset *accel_ofset,ICM20602_ofset *gyro_ofset);		// Write offset accelerometer&gyroscope
//---------------------Functions----------//
