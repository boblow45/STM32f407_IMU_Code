
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

	/*used to enable print to console. if not defined the print to console is removed*/
	//#define SWOPRINT 12

	/*used to enable the sd card code in the main loop for data acquisition*/
	#define SDCARD 13








	#ifdef SDCARD
	/*   _________________
		/ 1 2 3 4 5 6  7  8 |NR   |SDIO INTERFACE													|SPI INTERFACE
	 /								   	|NAME		STM32F4XX		DESCRIPTION								|NAME	STM32F4XX	DESCRIPTION 
	/ 9								   	|4-BIT	1-BIT							| 
	| 								  	|													| 
	|										1	|CD/DAT3	PC11	-			Connector data line 3		|CS		PB5			Chip select for SPI 
	|										2	|CMD			PD2		PD2		Command/Response line		|MOSI	PA7			Data input for SPI 
	|										3	|VSS1			GND		GND		GND											|VSS1	GND			GND 
	|	 SD CARD Pinout	  4	|VDD			3.3V	3.3V	3.3V Power supply				|VDD	3.3V		3.3V Power supply 
	|										5	|CLK			PC12	PC12	Clock										|SCK	PA5			Clock for SPI 
	|									  6	|VSS2			GND		GND		GND											|VSS2	GND			GND 
	|									  7	|DAT0			PC8		PC8		Connector data line 0		|MISO	PA6			Data output for SPI 
	|									  8	|DAT1			PC9		-			Connector data line 1		|-		-			- 
	|__________________ 9	|DAT2			PC10	-			Connector data line 2		|-		-			- 
	*/

	/* Use SPI communication with SDCard */
	#define	FATFS_USE_SDIO				0

	//Set your SPI, for corresponding pins look at TM SPI library
	#define FATFS_SPI               SPI1
	#define FATFS_SPI_PINSPACK      TM_SPI_PinsPack_1
	
	//Set your CS pin for SPI			
	#define FATFS_CS_PORT           GPIOB
	#define FATFS_CS_PIN            GPIO_PIN_5

	#include "tm_stm32f4_fatfs.h" 
	#include "fatfs_sd.h"
	#include "ffconf.h"
	#include "integer.h"
	//the following is being inclued so the sd card can be unmounted after enough data has been collected
	#include "tm_stm32f4_exti.h"
	#endif

#endif
