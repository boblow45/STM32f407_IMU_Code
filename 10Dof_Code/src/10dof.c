/*
 *
 *  Created on: Dec 9, 2015
 *      Author: cobrien
 */

#include "10dof.h"

// Defines
//#define ADXL345       ((uint8_t)0x53)         //Ox53 (followed by read or write) corresponding to OxA7 for read and OxA6 for write
//#define L3G4200D			((uint8_t)0x69) 				//Ox69 (followed by read or write) corresponding to OxD2 for write and OxD3 for read
//#define HMC5883L			((uint8_t)0x1E)					//Ox69 (followed by read or write) corresponding to Ox3C for write and Ox3D for read

#define ADXL345       0x53        
#define L3G4200D			0x69 				
#define HMC5883L			0x1E
#define PI 3.14159265359									// This is used to calculate the rate of change of angle in radians per second


// functions for the ADXL345 accelerometer
// See the following for the associated datasheet <http://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf>
uint8_t ADXL345_init(I2C_TypeDef* I2Cx) // This function sets up the Accelerometer
{
	I2C_Write_Reg(I2Cx, ADXL345, 0x2C, 0x0B);  // Write to Reg 0x2C so as to change the part (0x0B) to sampling of 200 Hz. (0x0A) gives a sampling rate of 100 Hz.
	I2C_Write_Reg(I2Cx, ADXL345, 0x2D, 0x08);  // Write to Reg 0x2D so as to change the part from standby (0x00) to (0x08) measure mode.
	I2C_Write_Reg(I2Cx, ADXL345, 0x38, 0x84);  //Write to Reg 0x38 so as to chage FIFO to 'stream' and sets D2 to 1
	return  I2C_Read_Reg(I2Cx, ADXL345, 0x00);
}

void ADXL345_data(I2C_TypeDef* I2Cx, int16_t* buf)
{
	uint8_t i,j = 0;
	uint8_t raw_data[6];
	I2C_Read_Multi_Reg(I2Cx, ADXL345, 0x32, raw_data, 6);
	for (i = 0 ; i <8; i += 2)
	{
		convert_2_sgn( &raw_data[i+1], &raw_data[i], &buf[j]);
		j++;
	}
}

// Functions for the L3G4200D Gyroscope
// See the following for the associated datasheet http://www.st.com/web/en/resource/technical/document/datasheet/CD00265057.pdf
uint8_t Gyro_init(I2C_TypeDef* I2Cx) // This function sets up the Gyroscope
{
	/*
	  CTRL_REG1. This register enables the x,y and z-axis outputs, it also powers up the device. A value of 0x0F will power up
	  the device, enable all the output bits (x,y,z), set the ODR to 100 Hz and set the BW to 12.5.
	  I decided to change it to 0x4F which will give me an ODR of 200 Hz and a BW of 12.5. Note this BW might slow down the device :(
	 */
	I2C_Write_Reg(I2Cx, L3G4200D, 0x20, 0x4F);
	I2C_Write_Reg(I2Cx, L3G4200D, 0x21, 0x00);  // CTRL_REG2 	This register configures the high-pass filter and it's ODR and cutoff frequency
	I2C_Write_Reg(I2Cx, L3G4200D, 0x22, 0x08);  // CTRL_REG3	This register enables the output of the data I think
	I2C_Write_Reg(I2Cx, L3G4200D, 0x23, 0x10);  // CTRL_REG4 	This register sets the DPS setting. In this case I set it to 500 DPS
	I2C_Write_Reg(I2Cx, L3G4200D, 0x24, 0x00);  // CTRL_REG5 	This register controls the HPF. I am not using it as this data will be filtered later anyway ;)

	return  I2C_Read_Reg(I2Cx, L3G4200D, 0x0F);
}


void Gyro_data(I2C_TypeDef* I2Cx, float* angle_rate)
{
	uint8_t i,j = 0;
	uint8_t raw_data[6];
	int16_t gyro_data[3];
	
	I2C_Read_Multi_Reg(I2Cx, L3G4200D, 0xA8, raw_data, 6);
	for (i = 0 ; i <8; i += 2)
	{
		convert_2_sgn( &raw_data[i+1], &raw_data[i], &gyro_data[j]);
		j++;
	}
	for(i = 0; i < 3; i++)
	{
		angle_rate[i] = gyro_data[i] / PI;
	}

}

// Compass Functions for the  HMC5883L 3-Axis Digital Compass
// See the following website for the datasheet <http://terzo.acmesystems.it/download/datasheets/HMC5883L.pdf>
uint8_t Compass_init(I2C_TypeDef* I2Cx) // This function setups the compass and returns the id register value
{
	I2C_Write_Reg(I2Cx, HMC5883L, 0x00, 0x18);  //Sets the DOR (data output rate) to 75 Hz, the averaging to 1 and measurement  configuration to normal
	I2C_Write_Reg(I2Cx, HMC5883L, 0x01, 0x20);  // Set the range on the sensor to be +- 1.3 Gauss
	I2C_Write_Reg(I2Cx, HMC5883L, 0x02, 0x01);	// Left the device in signal measurement mode for now. Might need to change this in the future
	return  I2C_Read_Reg(I2Cx, HMC5883L, 0x0A); // One of the three device ID registers on the chip. This register will return a value of 0X48
}

void Compass_data(I2C_TypeDef* I2Cx, int16_t* buf) //This function will return the Compass data
{
	uint8_t i,j = 0;
	uint8_t raw_data[6];
	
	I2C_Read_Multi_Reg(I2Cx,  HMC5883L, 0x03, raw_data, 6);
	for (i = 0 ; i <8; i += 2)
	{
		convert_2_sgn( &raw_data[i], &raw_data[i+1], &buf[j]);
		j++;
	}
}


// Usefull functions to make reading data from the IMU easier.
void convert_2_sgn(uint8_t* MSB, uint8_t* LSB, int16_t* data)
{		
		int16_t Concatenate_array;
		Concatenate_array = (( *MSB <<8) & 0xFF00) | ( *LSB & 0xFF);
		if (Concatenate_array >= 0x8000)
		  {
			Concatenate_array ^= 0xFFFF;              //This converts 2 complement to decimal
			Concatenate_array = Concatenate_array + 1;
			Concatenate_array = -Concatenate_array;
		  }
		*data = Concatenate_array;
}

void Unfilter_roll_pitch_cal(I2C_TypeDef* I2Cx, float* roll, float* pitch)
{
	int16_t acc_data[3];
	ADXL345_data(I2Cx, acc_data);

	//*roll = atan2 (acc_data[1], acc_data[2]);
	//The above line of code is commented as it does not give a good angle estimate. Might try and fix it so I only use one type of euler angle We shall see
	*roll = atan2 (acc_data[1],sqrt(acc_data[0]*acc_data[0] +acc_data[2]*acc_data[2]));
	*pitch = atan2( -acc_data[0], sqrt(acc_data[1]*acc_data[1] + acc_data[2]*acc_data[2]));
}
