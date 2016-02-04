/*
 * i2c_functions.h
 *
 *  Created on: Dec 9, 2015
 *      Author: cobrien
 */
#ifndef I2C_FUNCTIONS_H_
#define I2C_FUNCTIONS_H_ 98

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

 // The following is a set of basic I2C functions that can be used to create much larger functions
void init_I2C1(void);
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);


// The following is made up from the basic functions shown above
uint8_t I2C_Read_Reg(I2C_TypeDef* I2Cx, uint8_t Device, uint8_t Register);
void I2C_Write_Reg(I2C_TypeDef* I2Cx, uint8_t Device, uint8_t Register, uint8_t data);
void I2C_Read_Multi_Reg(I2C_TypeDef* I2Cx, uint8_t Device, uint8_t Register, uint8_t* buf, uint8_t count);

#ifdef __cplusplus
}
#endif
#endif /* I2C_FUNCTIONS_H_ */
