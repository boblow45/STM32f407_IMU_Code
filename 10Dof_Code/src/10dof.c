/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Cillian O'Brien, 2016
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "10dof.h"

#define ADXL345       0x53    // 7 bit address for the Accelerometer    
#define L3G4200D			0x69 		// 7 bit address for the Gyroscope
#define HMC5883L			0x1E		// 7 bit address for the Magnetometer/Compass
#define PI 3.14159265359			



///////////////////////////////////////////////////////////////////////////////////////////////////////
////																			/* ADXL345(Acc) functions */															///
///////////////////////////////////////////////////////////////////////////////////////////////////////

// functions for the  accelerometer
// See the following for the associated datasheet <http://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf>
uint8_t ADXL345_init(I2C_TypeDef* I2Cx) // This function sets up the Accelerometer
{
	uint8_t deviceID;
	
	I2C_Write(I2Cx, ADXL345, 0x2C, 0x0B);  // Write to Reg 0x2C so as to change the part (0x0B) to sampling of 200 Hz. (0x0A) gives a sampling rate of 100 Hz.
	I2C_Write(I2Cx, ADXL345, 0x2D, 0x08);  // Write to Reg 0x2D so as to change the part from standby (0x00) to (0x08) measure mode.
	I2C_Write(I2Cx, ADXL345, 0x38, 0x84);  // Write to Reg 0x38 so as to chage FIFO to 'stream' and sets D2 to 1
	deviceID = I2C_Read(I2Cx, ADXL345, 0x00);
	
	return deviceID; 
}

// Read data from the acclereomter, convert it to sign mag  an put it inside of struct axis  
void ADXL345_data(I2C_TypeDef* I2Cx, struct axis *acc)
{
	uint8_t data[6];
	
	I2C_ReadMulti(I2Cx, ADXL345, 0x32, data, sizeof(data)/sizeof(uint8_t));
	acc->x = convert_2_sgn(&data[1], &data[0]);
	acc->y = convert_2_sgn(&data[3], &data[2]);
	acc->z = convert_2_sgn(&data[5], &data[4]);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
////																			/* L3G4200D (Gyro) functions */														///
///////////////////////////////////////////////////////////////////////////////////////////////////////

// See the following for the associated datasheet http://www.st.com/web/en/resource/technical/document/datasheet/CD00265057.pdf
uint8_t Gyro_init(I2C_TypeDef* I2Cx) // This function sets up the Gyroscope
{
	/*
	  CTRL_REG1. This register enables the x,y and z-axis outputs, it also powers up the device. A value of 0x0F will power up
	  the device, enable all the output bits (x,y,z), set the ODR to 100 Hz and set the BW to 12.5.
	  I decided to change it to 0x4F which will give me an ODR of 200 Hz and a BW of 12.5. Note this BW might slow down the device :(
	 */
	uint8_t deviceID;
	
	I2C_Write(I2Cx, L3G4200D, 0x20, 0x4F);
	I2C_Write(I2Cx, L3G4200D, 0x21, 0x00);  // CTRL_REG2 	This register configures the high-pass filter and it's ODR and cutoff frequency
	I2C_Write(I2Cx, L3G4200D, 0x22, 0x08);  // CTRL_REG3	This register enables the output of the data I think
	I2C_Write(I2Cx, L3G4200D, 0x23, 0x10);  // CTRL_REG4 	This register sets the DPS setting. In this case I set it to 500 DPS
	I2C_Write(I2Cx, L3G4200D, 0x24, 0x00);  // CTRL_REG5 	This register controls the HPF. I am not using it as this data will be filtered later anyway ;)

	deviceID = I2C_Read(I2Cx, L3G4200D, 0x0F);
	return deviceID;
}

// Read data from the gyroscope, convert it to sign mag  an put it inside of struct axis 
void Gyro_data(I2C_TypeDef* I2Cx, struct axis *gyro)
{
	uint8_t data[6];
	
	I2C_ReadMulti(I2Cx, L3G4200D, 0xA8, data, sizeof(data)/sizeof(uint8_t));
	gyro->x = convert_2_sgn(&data[1], &data[0]);
	gyro->y = convert_2_sgn(&data[3], &data[2]);
	gyro->z = convert_2_sgn(&data[5], &data[4]);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
////																	/* HMC5883L ( Compass) functions */														///
///////////////////////////////////////////////////////////////////////////////////////////////////////

// See the following website for the datasheet <http://terzo.acmesystems.it/download/datasheets/HMC5883L.pdf>
uint8_t Compass_init(I2C_TypeDef* I2Cx) // This function setups the compass and returns the id register value
{	
	uint8_t deviceID;
	
	I2C_Write(I2Cx, HMC5883L, 0x00, 0x18);  //Sets the DOR (data output rate) to 75 Hz, the averaging to 1 and measurement  configuration to normal
	I2C_Write(I2Cx, HMC5883L, 0x01, 0x20);  // Set the range on the sensor to be +- 1.3 Gauss
	I2C_Write(I2Cx, HMC5883L, 0x02, 0x01);	// Left the device in signal measurement mode for now. Might need to change this in the future
	deviceID = I2C_Read(I2Cx, HMC5883L, 0x0A); // One of the three device ID registers on the chip. This register will return a value of 0X48

	return deviceID; 
}

void Compass_data(I2C_TypeDef* I2Cx, struct axis *compass) //This function will return the Compass data
{
	uint8_t data[6];
	
	I2C_ReadMulti(I2Cx,  HMC5883L, 0x03, data, sizeof(data)/sizeof(uint8_t));
	compass->x = convert_2_sgn(&data[0], &data[1]);
	compass->y = convert_2_sgn(&data[2], &data[3]);
	compass->z = convert_2_sgn(&data[4], &data[5]);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
////																			/* Private functions */																		///
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

static int16_t convert_2_sgn(uint8_t* MSB, uint8_t* LSB)
{		
		int16_t Concat_val;
		Concat_val = ((*MSB <<8) & 0xFF00) | (*LSB & 0xFF);
		if (Concat_val >= (1<<15))
		  {
			Concat_val ^= 0xFFFF;              //This converts 2 complement to decimal
			Concat_val = Concat_val + 1;
			Concat_val = -Concat_val;
		  }
		return Concat_val;
}


void Unfilter_roll_pitch_cal(I2C_TypeDef* I2Cx)
{

	ADXL345_data(I2Cx, &acc);

	//orientation->roll = atan2 (acc.y, acc.z);
	//The above line of code is commented as it does not give a good angle estimate. Might try and fix it so I only use one type of euler angle We shall see
	
	orientation.roll = atan2(acc.y,sqrt(acc.x*acc.x + acc.z*acc.z));
	orientation.pitch = atan2(-acc.x, sqrt(acc.y*acc.y + acc.z*acc.z));
}
