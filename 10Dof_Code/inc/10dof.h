/*
 * 
 *
 *  Created on: Dec 9, 2015
 *      Author: cobrien
 */
#ifndef __10DOF_H_
#define __10DOF_H_ 99

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <math.h>
#include "I2C.h"
#include "stm32f4xx.h"

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
 * @param  *data: pointer to data array to store data from accelerometer. The array is three elements long.
 * @retval None
 */
void ADXL345_data(I2C_TypeDef* I2Cx, int16_t* data); 

/*
 * @brief  Read back register values from gyroscope and scale the values accordingly
 * @param  *I2Cx: I2C used
 * @param  *data: pointer to data array to store data from gyroscope. The array is three elements long.
 * @note   The values stored in data are floats so that the data returned by the the device is in radains per second
 * @retval None
 */
void Gyro_data(I2C_TypeDef* I2Cx, float* data); //This function will return the Gyroscope data

/*
 * @brief  Read back register values from gyroscope and don't scale
 * @param  *I2Cx: I2C used
 * @param  *data: pointer to data array to store data from gyroscope. The array is three elements long.
 * @note   The values stored in data array is the raw data returned by the gyroscope in degrees per second
 * @retval None
 */
void Gyro_data_raw(I2C_TypeDef* I2Cx, int16_t* gyro_data);
	
/*
 * @brief  Read back register values from magnetometer
 * @param  *I2Cx: I2C used
 * @param  *data: pointer to data array to store data from magnetometer. The array is three elements long.
 * @note   The data returned by this function has to be compensated by means of angle
 * @retval None
 */
void Compass_data(I2C_TypeDef* I2Cx, int16_t* data); //This function will return the Compass data




/*
 * @brief  Used to convert 2 complement to signed magnetude 
 * @param  *MSB: MSB of the number to convert to sgn mag
 * @param  *LSB: LSB of the number to convert to sgn mag
 * @retval *data: sgn mag result of the MSB concatenated with LSB
 */
void convert_2_sgn(uint8_t* MSB, uint8_t* LSB, int16_t* data);

/*
 * @brief  Returns unfiltered angles 
 * @param  *I2Cx: I2C used
 * @param  *roll: angle of the accelerometer with respect to the x-axis
 * @param  *pitch: angle of the accelerometer with respect to the y-axis
 * @retval None
 */
void Unfilter_roll_pitch_cal(I2C_TypeDef* I2Cx, float* roll, float* pitch);



#ifdef __cplusplus
}
#endif

#endif /* __10DOF_H_ */
