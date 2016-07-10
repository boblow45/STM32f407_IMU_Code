/**
 * @author  Cillian O'Brien
 * @email   cillianobrien01@gmail.com
 * @version v1.0.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   I2C library for STM32F4xx
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Cillian O'Brien, 2016
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
 
#ifndef __10DOF_H_
#define __10DOF_H_ 99

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <math.h>
#include "I2C_improved.h"
#include "stm32f4xx.h"

struct axis
{
	int16_t x;
	int16_t y; 
	int16_t z; 
} acc, gyro, compass ;	 

struct position
{
	float roll;				// Rotation around the x-axis 
	float pitch; 			// Rotation around the y-axis
	float yaw; 				// Rotation around the z-axis
} orientation;
	 
	 
/*
 * @brief  Sets up the Accelerometer
 * @param  *I2Cx: I2C used
 * @note	 Datasheet can be found at following <http://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf>
 * @retval ADXL345 ID value. 
 */
uint8_t ADXL345_init(I2C_TypeDef* I2Cx); 

/*
 * @brief  Sets up the Gyroscope
 * @param  *I2Cx: I2C used
 * @note Datasheet can be found at following <http://www.st.com/web/en/resource/technical/document/datasheet/CD00265057.pdf>
 * @retval L3G4200D ID value. 
 */
uint8_t Gyro_init(I2C_TypeDef* I2Cx); 

/*
 * @brief  Sets up the magnetometer
 * @param  *I2Cx: I2C used
 * @note Datasheet can be found at following <http://terzo.acmesystems.it/download/datasheets/HMC5883L.pdf>
 * @retval HMC5883L ID value.
 */
uint8_t Compass_init(I2C_TypeDef* I2Cx); 

/*
 * @brief  Read back register values from accelerometer
 * @param  *I2Cx: I2C used
 * @param  *acc: pointer to struct to store data from accelerometer. The struct is three elements long.
 * @retval None
 */
void ADXL345_data(I2C_TypeDef* I2Cx, struct axis *acc); 

/*
 * @brief  Read back register values from gyroscope and don't scale
 * @param  *I2Cx: I2C used
 * @param  *gyro: pointer to struct to store data from Gyroscope. The struct is three elements long.
 * @note   The values stored in data array is the raw data returned by the gyroscope in degrees per second
 * @retval None
 */
void Gyro_data(I2C_TypeDef* I2Cx, struct axis *gyro);
	
/*
 * @brief  Read back register values from magnetometer
 * @param  *I2Cx: I2C used
 * @param  *data: pointer to data array to store data from magnetometer. The array is three elements long.
 * @note   The data returned by this function has to be compensated by means of angle
 * @retval None
 */
void Compass_data(I2C_TypeDef* I2Cx, struct axis *compass); //This function will return the Compass data


/*
 * @brief  Used to convert 2 complement to signed magnetude 
 * @param  *MSB: MSB of the number to convert to sgn mag
 * @param  *LSB: LSB of the number to convert to sgn mag
 * @retval Concat_val: the combined value of the MSB and the LSB in signed magnitude format	
 */
static int16_t convert_2_sgn(uint8_t* MSB, uint8_t* LSB);

/*
 * @brief  Calculates unfiltered angles and places them in the "orientation" struct 
 * @param  *I2Cx: I2C used
 * @retval None
 */
void Unfilter_roll_pitch_cal(I2C_TypeDef* I2Cx);



#ifdef __cplusplus
}
#endif

#endif /* __10DOF_H_ */
