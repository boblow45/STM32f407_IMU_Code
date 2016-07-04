#include "main.h"

const float  PI = 3.14159265358979;





int main(void) 
	{
		SystemInit();
		Init_I2C(I2C1, I2C_Pinout_1, I2C_CLOCK_FAST_MODE);
				
		while (1) 
			{	
				I2C_Read(I2C1, (0x53), 0x00);
			}
	}


