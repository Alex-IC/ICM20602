#include "ICM20602.h"

#define LOW(x) (x & 0xFF) 				//allocates low byte
#define HIGH(x) ((x >> 8) & 0xFF) // allocates high byte


//--------------------------------------------------------------------------------//
int8_t ICM20602_writeRegisters(uint8_t subAddress,uint8_t *Data,uint8_t Size) { 			// Write some registers SPI
	subAddress = subAddress & 0x7F;
	ICM20602_CS_LOW; 
	if(HAL_SPI_Transmit(ICM20602_SPI, &subAddress, 0x01, 0x0010)!=HAL_OK){ICM20602_CS_HIGH;return -1;}
	if(HAL_SPI_Transmit(ICM20602_SPI, Data, Size, 0x0010)!=HAL_OK){ICM20602_CS_HIGH;return -1;}
	ICM20602_CS_HIGH; 
	return 0;
}
//--------------------------------------------------------------------------------//
int8_t ICM20602_readRegisters(uint8_t subAddress,uint8_t *Data,uint8_t Size){ 				// Read some registers SPI
subAddress = (subAddress|0x80);
	
ICM20602_CS_LOW; 
if(HAL_SPI_Transmit(ICM20602_SPI, &subAddress, 0x01, 0x0010)!=HAL_OK){ICM20602_CS_HIGH;return -1;}
if(HAL_SPI_Receive(ICM20602_SPI, Data, Size, 0x0100)!=HAL_OK){ICM20602_CS_HIGH;return -1;}	
ICM20602_CS_HIGH; 
return 0;
}

//--------------------------------------------------------------------------------//
int8_t ICM20602_configAccel(uint8_t range, uint8_t bandwidth) { 											// Accelerometer Full Scale Select & bandwidth
uint8_t tmp; 
      tmp = bandwidth;
     if( ICM20602_writeRegisters(ICM20602_ACCEL_CONFIG_2, &tmp, 1)<0)return -1;//1D
      		HAL_Delay(10);


    tmp = range;
   if( ICM20602_writeRegisters(ICM20602_ACCEL_CONFIG, &tmp, 1)<0)return -1;//1c
   		HAL_Delay(10);

    
  return 0;
}
//--------------------------------------------------------------------------------//
int8_t ICM20602_configGyro(uint8_t range, uint8_t bandwidth) {												// Gyroscope Full Scale Select & bandwidth
uint8_t tmp;
	if (ICM20602_GYRO_DLPF_BYPASS_3281_HZ == bandwidth) {
      tmp=range+0x02;
     if( ICM20602_writeRegisters(ICM20602_GYRO_CONFIG, &tmp, 1)<0)return -1; //1B
		HAL_Delay(10);
    }
	
   else if (ICM20602_GYRO_DLPF_BYPASS_8173_HZ == bandwidth) {
      // bypass dpf and set dps
      tmp=range+0x01;
     if( ICM20602_writeRegisters(ICM20602_GYRO_CONFIG, &tmp, 1)<0)return -1; //1B
		HAL_Delay(10);
    }
    else {
      // configure dpf and set dps
      tmp = bandwidth;
     if( ICM20602_writeRegisters(ICM20602_CONFIG, &tmp, 1)<0)return -1; //1A
      HAL_Delay(10);

      tmp = range;
     if( ICM20602_writeRegisters(ICM20602_GYRO_CONFIG, &tmp, 1)<0)return -1; //1B
      HAL_Delay(10);
    }	
  return 0;
}

//--------------------------------------------------------------------------------//
uint8_t ICM20602_WHO_I_AM(void){ 																											// Read ID ICM20602 (0x12)
	uint8_t e;
	if(ICM20602_readRegisters(ICM20602_WHO_AM_I, &e, 1)<0)return 0;
  return e;
}
//--------------------------------------------------------------------------------//
int8_t ICM20602_Init(void){																														// Init ICM20602
  uint8_t tmp = 0;
	
		tmp=0x00;
		if(ICM20602_writeRegisters(ICM20602_PWR_MGMT_1, &tmp, 1)<0)return -1;    										// CLK_SEL=0: internal 8MHz, TEMP_DIS=0, SLEEP=0 
		tmp=0x07;
		HAL_Delay(10);
   if( ICM20602_writeRegisters(ICM20602_SMPLRT_DIV, &tmp, 1)<0)return -1;  											// Gyro output sample rate = Gyro Output Rate/(1+SMPLRT_DIV)
		  HAL_Delay(10); 
		if(ICM20602_configAccel(ICM20602_ACCEL_RANGE_4G, ICM20602_ACCEL_DLPF_99_HZ)<0)return -1;// Config accelerometer 
	  HAL_Delay(10); 
		if(ICM20602_configGyro(ICM20602_GYRO_RANGE_2000_DPS,ICM20602_GYRO_DLPF_92_HZ)<0)return -1; // Config gyroscope
		HAL_Delay(10);

  return 0;
}
//--------------------------------------------------------------------------------//
int8_t ICM20602_readSensors(ICM20602_transfer *dat){																						// Read Accelerometer&Gyroscope
  uint8_t buf[14] = {0};
//REG_ACCEL_XOUT_H
  if(ICM20602_readRegisters(ICM20602_ACCEL_XOUT_H, buf, 14)<0)return -1;

	dat->acs_xH = buf[0];
	dat->acs_xL = buf[1];
	dat->acs_yH = buf[2];
	dat->acs_yL = buf[3];
	dat->acs_zH = buf[4];
	dat->acs_zL = buf[5];


	dat->gyro_xH = buf[8];
	dat->gyro_xL = buf[9];
	dat->gyro_yH = buf[10];
	dat->gyro_yL = buf[11];
	dat->gyro_zH = buf[12];
	dat->gyro_zL = buf[13];
	

  return 0;
}

//--------------------------------------------------------------------------------//
int8_t ICM20602_readOffset(ICM20602_ofset *accel_ofset,ICM20602_ofset *gyro_ofset){                     // Read Offset Accelerometer&Gyroscope
uint8_t buf_a[6];
uint8_t buf_g[6];
	  if(ICM20602_readRegisters(ICM20602_XA_OFFSET_H, buf_a, 6)<0)return -1;
		if(ICM20602_readRegisters(ICM20602_XG_OFFS_USRH, buf_g, 6)<0)return -1;
	
		accel_ofset->x_ofset=buf_a[0]<<8|buf_a[1];
		accel_ofset->y_ofset=buf_a[2]<<8|buf_a[3];
		accel_ofset->z_ofset=buf_a[4]<<8|buf_a[5];
	
		gyro_ofset->x_ofset=buf_g[0]<<8|buf_g[1];
		gyro_ofset->y_ofset=buf_g[2]<<8|buf_g[3];
		gyro_ofset->z_ofset=buf_g[4]<<8|buf_g[5];
	return 0;
}
//--------------------------------------------------------------------------------//
int8_t ICM20602_writeOffset(ICM20602_ofset *accel_ofset,ICM20602_ofset *gyro_ofset){                    // Write Offset Accelerometer&Gyroscope
uint8_t buf_a[6];
uint8_t buf_g[6];
		
	buf_a[0]=HIGH(accel_ofset->x_ofset);
	buf_a[1]=LOW(accel_ofset->x_ofset);
	buf_a[2]=HIGH(accel_ofset->y_ofset);
	buf_a[3]=LOW(accel_ofset->y_ofset);
	buf_a[4]=HIGH(accel_ofset->z_ofset);
	buf_a[5]=LOW(accel_ofset->z_ofset);
	
	buf_g[0]=HIGH(gyro_ofset->x_ofset);
	buf_g[1]=LOW(gyro_ofset->x_ofset);
	buf_g[2]=HIGH(gyro_ofset->y_ofset);
	buf_g[3]=LOW(gyro_ofset->y_ofset);
	buf_g[4]=HIGH(gyro_ofset->z_ofset);
	buf_g[5]=LOW(gyro_ofset->z_ofset);

	
		  if(ICM20602_writeRegisters(ICM20602_XA_OFFSET_H, buf_a, 6)<0)return -1;
			HAL_Delay(10);
			if(ICM20602_writeRegisters(ICM20602_XG_OFFS_USRH, buf_g, 6)<0)return -1;
      HAL_Delay(10);
	return 0;

}
//--------------------------------------------------------------------------------//
