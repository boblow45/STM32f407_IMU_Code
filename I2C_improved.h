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
#ifndef I2C_H
#define I2C_H 161

/*
 *	\par Pinout
@verbatim
       |Pinout   1     |Pinout  2      |Pinout 3
I2CX   |SCL      SDA   |SCL     SDA    |SCL   SDA
_______|_______________|_______________|_________
I2C1   |PB6      PB7   |PB8     PB9    |PB6   PB9
I2C2   |PB10     PB11  |PF1     PF0    |PH4   PH5
I2C3   |PA8      PC9   |PH7     PH8    |-     -
@endverbatim

# The following 
@verbatim
//By default library support only 7bit long address
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
  I2C_InitStruct->I2C_ClockSpeed = 10000;																		// 100kHz
  I2C_InitStruct->I2C_Mode = I2C_Mode_I2C;																	// I2C mode
  I2C_InitStruct->I2C_DutyCycle = I2C_DutyCycle_2;													// 50% duty cycle --> standard
  I2C_InitStruct->I2C_OwnAddress1 = 0x00;																		// Set own address to 0x00, not relevant in master mode
  I2C_InitStruct->I2C_Ack = I2C_Ack_Disable;																// disable acknowledge when reading (can be changed later on)
  I2C_InitStruct->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	 	// set address length to 7 bit addresses
}
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.6.1
  - June 26, 2016
  - Added the ability to change the pins that the I2C is put out on. 
 */ 

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "attributes.h"
#include "defines.h"


/**
 * @defgroup I2C_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Timeout for I2C
 */
#ifndef I2C_TIMEOUT
#define I2C_TIMEOUT					20000
#endif

/* I2C1 settings, change them in defines.h project file */
#ifndef I2C1_ACKNOWLEDGED_ADDRESS
#define I2C1_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
#endif
#ifndef I2C1_MODE
#define I2C1_MODE					I2C_Mode_I2C
#endif
#ifndef I2C1_OWN_ADDRESS
#define I2C1_OWN_ADDRESS				0x00
#endif
#ifndef I2C1_ACK
#define I2C1_ACK						I2C_Ack_Disable
#endif
#ifndef I2C1_DUTY_CYCLE
#define I2C1_DUTY_CYCLE				I2C_DutyCycle_2
#endif

/* I2C2 settings, change them in defines.h project file */
#ifndef I2C2_ACKNOWLEDGED_ADDRESS
#define I2C2_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
#endif
#ifndef I2C2_MODE
#define I2C2_MODE					I2C_Mode_I2C
#endif
#ifndef I2C2_OWN_ADDRESS
#define I2C2_OWN_ADDRESS				0x00
#endif
#ifndef I2C2_ACK
#define I2C2_ACK						I2C_Ack_Disable
#endif
#ifndef I2C2_DUTY_CYCLE
#define I2C2_DUTY_CYCLE				I2C_DutyCycle_2
#endif

/* I2C3 settings, change them in defines.h project file */
#ifndef I2C3_ACKNOWLEDGED_ADDRESS
#define I2C3_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
#endif
#ifndef I2C3_MODE
#define I2C3_MODE					I2C_Mode_I2C
#endif
#ifndef I2C3_OWN_ADDRESS
#define I2C3_OWN_ADDRESS				0x00
#endif
#ifndef I2C3_ACK
#define I2C3_ACK						I2C_Ack_Disable
#endif
#ifndef I2C3_DUTY_CYCLE
#define I2C3_DUTY_CYCLE				I2C_DutyCycle_2
#endif

#define I2C_CLOCK_STANDARD			    100000  /*!< I2C Standard speed */
#define I2C_CLOCK_FAST_MODE			    400000  /*!< I2C Fast mode speed */
#define I2C_CLOCK_FAST_MODE_PLUS		1000000 /*!< I2C Fast mode plus speed */
#define I2C_CLOCK_HIGH_SPEED			  3400000 /*!< I2C High speed */

 /**
 * @}
 */
 
/**
 * @defgroup I2C_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  I2C Pinout enumeration
 */
typedef enum {
	I2C_Pinout_1,      /*!< Use Pinout1 from Pinout table for I2Cx */
	I2C_Pinout_2,      /*!< Use Pinout2 from Pinout table for I2Cx */
	I2C_Pinout_3,      /*!< Use Pinout3 from Pinout table for I2Cx */
} I2C_Pinout_t;

/**
 * @}
 */

/**
 * @defgroup I2C_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes I2C
 * @param  *I2Cx: I2C used
 * @param  pinout: Pins used. This parameter can have a value of @ref I2C_Pinout_t enumeration
 * @param  clockSpeed: Clock speed for SCL in Hertz
 * @retval None
 */
void Init_I2C(I2C_TypeDef* I2Cx, I2C_Pinout_t pinout, uint32_t clockSpeed);

/**
 * @brief  Reads single byte from slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to read from
 * @retval Data from slave
 */
uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg);

/**
 * @brief  Reads multi bytes from slave
 * @param  *I2Cx: I2C used
 * @param  uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  uint8_t reg: register to read from
 * @param  uint8_t *data: pointer to data array to store data from slave
 * @param  uint8_t count: how many bytes will be read
 * @retval None
 */
void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
 * @brief  Reads byte from slave without specify register address (This is used in the case where the device has a predefined register bank that can be read from without specifing a register to start from)
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Data from slave
 */
uint8_t I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address);

/**
 * @brief  Reads multi bytes from slave without setting register from where to start read
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to store data from slave
 * @param  count: how many bytes will be read
 * @retval None
 */
void I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

/**
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
 * @brief  Writes byte to slave without specify register address
 *
 *         Useful if you have I2C device to read like that:
 *            - I2C START
 *            - SEND DEVICE ADDRESS
 *            - SEND DATA BYTE
 *            - I2C STOP
 *
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  data: data byte which will be send to device
 * @retval None
 */
void I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data);

/**
 * @brief  Writes multi bytes to slave without setting register from where to start write
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to write data to slave
 * @param  count: how many bytes you want to write
 * @retval None
 */
void I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

/**
 * @brief  Checks if device is connected to I2C bus
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Device status:
 *            - 0: Device is not connected
 *            - > 0: Device is connected
 */
uint8_t I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address);

/**
 * @brief  I2C Start condition
 * @param  *I2Cx: I2C used
 * @param  address: slave address
 * @param  direction: master to slave or slave to master
 * @param  ack: ack enabled or disabled
 * @retval Start condition status
 * @note   For private use
 */
int16_t I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);

/**
 * @brief  Stop condition on I2C
 * @param  *I2Cx: I2C used
 * @retval Stop condition status
 * @note   For private use
 */
uint8_t I2C_Stop(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte without ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t I2C_ReadNack(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte with ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t I2C_ReadAck(I2C_TypeDef* I2Cx);

/**
 * @brief  Writes to slave
 * @param  *I2Cx: I2C used
 * @param  data: data to be sent
 * @retval None
 * @note   For private use
 */
void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

#endif

