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
#include "I2C_improved.h"

/* Private variables */
static uint32_t I2C_Timeout;
static uint32_t I2C_INT_Clocks[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

/* Private functions */
void I2C1_INT_InitPins(I2C_Pinout_t pinout);
void I2C2_INT_InitPins(I2C_Pinout_t pinout);
void I2C3_INT_InitPins(I2C_Pinout_t pinout);
void I2C_GPIO_INIT(GPIO_TypeDef* GPIOx, uint16_t SDA, uint16_t SCL);

///////////////////////////////////////////////////////////////////////////////////////////////////////
////																			/* I2C Init function */																		///
///////////////////////////////////////////////////////////////////////////////////////////////////////

void Init_I2C(I2C_TypeDef* I2Cx, I2C_Pinout_t pinout, uint32_t clockSpeed) 
	{
	I2C_InitTypeDef I2C_InitStruct;
	
	if (I2Cx == I2C1) 
		{
			/* Enable pins */
		I2C1_INT_InitPins(pinout);
		
		/* Check clock, set the lowest clock your devices support on the same I2C but */
		if (clockSpeed < I2C_INT_Clocks[0]) 
			{I2C_INT_Clocks[0] = clockSpeed;}
		
		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = I2C_INT_Clocks[0];
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C1_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = I2C1_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = I2C1_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = I2C1_ACK;
		I2C_InitStruct.I2C_DutyCycle = I2C1_DUTY_CYCLE;
		}
	else if (I2Cx == I2C2) 
		{
		/* Enable clock */
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
		
		/* Enable pins */
		I2C2_INT_InitPins(pinout);
		
		/* Check clock, set the lowest clock your devices support on the same I2C but */
		if (clockSpeed < I2C_INT_Clocks[1]) 
			{I2C_INT_Clocks[1] = clockSpeed;}
		
		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = I2C_INT_Clocks[1];
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C2_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = I2C2_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = I2C2_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = I2C2_ACK;
		I2C_InitStruct.I2C_DutyCycle = I2C2_DUTY_CYCLE;
	} 
	else if (I2Cx == I2C3) 
		{
		/* Enable clock */
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
		
		/* Enable pins */
		I2C3_INT_InitPins(pinout);
		
		/* Check clock, set the lowest clock your devices support on the same I2C but */
		if (clockSpeed < I2C_INT_Clocks[2]) 
			{I2C_INT_Clocks[2] = clockSpeed;}
		
		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = I2C_INT_Clocks[2];
		I2C_InitStruct.I2C_Mode = I2C3_MODE;
		I2C_InitStruct.I2C_DutyCycle = I2C3_DUTY_CYCLE;
		I2C_InitStruct.I2C_OwnAddress1 = I2C3_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = I2C3_ACK;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C3_ACKNOWLEDGED_ADDRESS;
	}
	
	I2C_Cmd(I2C1, DISABLE);							// Disable I2C first 
	I2C_Init(I2Cx, &I2C_InitStruct); 		// Initialize I2C
	I2C_Cmd(I2C1, ENABLE); 							//Enable I2C 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
////																			/* I2C Read functions */																	///
///////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) {
	uint8_t received_data;
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	I2C_Stop(I2Cx);
	I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_DISABLE);
	received_data = I2C_ReadNack(I2Cx);
	return received_data;
}

void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	I2C_WriteData(I2Cx, reg);
	//TM_I2C_Stop(I2Cx);
	I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	for (i = 0; i < count; i++) {
		if (i == (count - 1)) {
			/* Last byte */
			data[i] = I2C_ReadNack(I2Cx);
		} else {
			data[i] = I2C_ReadAck(I2Cx);
		}
	}
}

uint8_t I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address) {
	uint8_t data;
	I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	/* Also stop condition happens */
	data = I2C_ReadNack(I2Cx);
	return data;
}

void I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	for (i = 0; i < count; i++) {
		if (i == (count - 1)) {
			/* Last byte */
			data[i] = I2C_ReadNack(I2Cx);
		} else {
			data[i] = I2C_ReadAck(I2Cx);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
////																			/* I2C Write functions */																	///
///////////////////////////////////////////////////////////////////////////////////////////////////////

void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data) {

	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	I2C_WriteData(I2Cx, data);
	I2C_Stop(I2Cx);
}

void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	for (i = 0; i < count; i++) {
		I2C_WriteData(I2Cx, data[i]);
	}
	I2C_Stop(I2Cx);
}

void I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data) {
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, data);
	I2C_Stop(I2Cx);
}

void I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	for (i = 0; i < count; i++) {
		I2C_WriteData(I2Cx, data[i]);
	}
	I2C_Stop(I2Cx);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
////																			/* Private functions */																		///
///////////////////////////////////////////////////////////////////////////////////////////////////////
int16_t I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack) {
	/* Generate I2C start pulse */
	I2Cx->CR1 |= I2C_CR1_START;
	
	/* Wait till I2C is busy */
	I2C_Timeout = I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_SB)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Enable ack if we select it */
	if (ack) {
		I2Cx->CR1 |= I2C_CR1_ACK;
	}

	/* Send write/read bit */
	if (direction == I2C_TRANSMITTER_MODE) {
		/* Send address with zero last bit */
		I2Cx->DR = address & ~I2C_OAR1_ADD0;
		
		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!(I2Cx->SR1 & I2C_SR1_ADDR)) {
			if (--I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	if (direction == I2C_RECEIVER_MODE) {
		/* Send address with 1 last bit */
		I2Cx->DR = address | I2C_OAR1_ADD0;
		
		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
			if (--I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	
	/* Read status register to clear ADDR flag */
	I2Cx->SR2;
	
	/* Return 0, everything ok */
	return 0;
}

void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data) {
	/* Wait till I2C is not busy anymore */
	I2C_Timeout = I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_TXE) && I2C_Timeout) {
		I2C_Timeout--;
	}
	
	/* Send I2C data */
	I2Cx->DR = data;
}

uint8_t I2C_ReadAck(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Enable ACK */
	I2Cx->CR1 |= I2C_CR1_ACK;
	
	/* Wait till not received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t I2C_ReadNack(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Disable ACK */
	I2Cx->CR1 &= ~I2C_CR1_ACK;
	
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Wait till received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t I2C_Stop(I2C_TypeDef* I2Cx) {
	/* Wait till transmitter not empty */
	I2C_Timeout = I2C_TIMEOUT;
	while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF)))) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Return 0, everything ok */
	return 0;
}

uint8_t I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address) {
	uint8_t connected = 0;
	/* Try to start, function will return 0 in case device will send ACK */
	if (!I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE)) {
		connected = 1;
	}
	
	/* STOP I2C */
	I2C_Stop(I2Cx);
	
	/* Return status */
	return connected;
}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	////																	/* I2C GPIO Setup functions */																///
	///////////////////////////////////////////////////////////////////////////////////////////////////////

void I2C1_INT_InitPins(I2C_Pinout_t pinout) 
	{
	/* Init pins */
		// As all of the  options are on GPIOB bank there is no need to put the following enable in the if statements 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);   // enable APB1 peripheral clock for I2C1
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // enable clock for SCL and SDA pins
		
	if (pinout == I2C_Pinout_1) 
		{I2C_GPIO_INIT(GPIOB, GPIO_Pin_6, GPIO_Pin_7);}
	if (pinout == I2C_Pinout_2) 
		{I2C_GPIO_INIT(GPIOB, GPIO_Pin_8, GPIO_Pin_9);}
	if (pinout == I2C_Pinout_3) 
		{I2C_GPIO_INIT(GPIOB, GPIO_Pin_6, GPIO_Pin_9);}
	}

void I2C2_INT_InitPins(I2C_Pinout_t pinout) 
	{
	/* Init pins */
	if (pinout == I2C_Pinout_1) 
			{	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);   // enable APB1 peripheral clock for I2C2
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // enable clock for SCL and SDA pins
			I2C_GPIO_INIT(GPIOB, GPIO_Pin_10, GPIO_Pin_11);
			}
	if (pinout == I2C_Pinout_2) 
		{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);   // enable APB1 peripheral clock for I2C2
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // enable clock for SCL and SDA pins
		I2C_GPIO_INIT(GPIOF, GPIO_Pin_1, GPIO_Pin_0);
		}
	if (pinout == I2C_Pinout_3) 
		{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);   // enable APB1 peripheral clock for I2C2
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);  // enable clock for SCL and SDA pins
		I2C_GPIO_INIT(GPIOH, GPIO_Pin_4, GPIO_Pin_5);
		}
	}

void I2C3_INT_InitPins(I2C_Pinout_t pinout) 
	{
	/* Init pins */
	if (pinout == I2C_Pinout_1) 
		{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8; 					// Need to use PA8 as SCL
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;				// set pins to alternate function
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// set GPIO speed
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;				// enable pull up resistors
		GPIO_Init(GPIOA, &GPIO_InitStruct);							// init GPIOA

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; 					// Need to use PC9 as SDA
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;				// set pins to alternate function
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// set GPIO speed
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;				// enable pull up resistors
		GPIO_Init(GPIOC, &GPIO_InitStruct);							// init GPIOC
		}
	if (pinout == I2C_Pinout_2) 
		{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);   // enable APB1 peripheral clock for I2C2
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);  // enable clock for SCL and SDA pins
		I2C_GPIO_INIT(GPIOF, GPIO_Pin_7, GPIO_Pin_8);	  }
	}

void I2C_GPIO_INIT(GPIO_TypeDef* GPIOx, uint16_t SCL, uint16_t SDA)
	{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = SCL| SDA; 						// we are going to use SCL as clock pin and SDA as data pin
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;				// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;				// enable pull up resistors
	GPIO_Init(GPIOx, &GPIO_InitStruct);							// init GPIOx
	}
	
	//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
	