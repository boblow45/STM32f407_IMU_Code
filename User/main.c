#include "main.h"

const float  PI = 3.14159265358979;


#ifdef SDCARD
/* Fatfs object */
FATFS FatFs;
/* File object */
FIL fil;
/*
The Following line is a flag to tell the programme if the SD card is mounted. If it is set to zero
the programme stops recording data. This value can be changed by means of a external interupt
*/
uint8_t Sdmounted = 1;

/* Used as a bufer when converting accelerometer data to string*/
char buffer[50];
#endif

int main(void) 
	{
		//The following line of code creates a struct that is requred for the timer used in the stm32f4
		TIM_TimeBaseInitTypeDef ConstantDelayStruct;
		
		uint8_t Acc_id, gyro_id, Compass_id;
		float angles[3];
		
		uint32_t DelayFreq = 50; // This delay is set from the wanted/requied delay time of 20e-3 seconds
		uint32_t PastCountVal;
		
		SystemInit();
		init_I2C1(); 	//Initialize I2C, SCL: PB6 and SDA: PB7 with 100kHt serial clock 
		Acc_id = ADXL345_init(I2C1), gyro_id = Gyro_init(I2C1), Compass_id = Compass_init(I2C1);
		
		LedInit();
		
		if(Acc_id == 0xE5)LedOn(LED_RED);
		if(gyro_id == 0xD3)LedOn(LED_GREEN);
		if(Compass_id == 0x48)LedOn(LED_ORANGE);
		
		InitializeTimer(&ConstantDelayStruct, DelayFreq);
		
		//used to enable print to console. The following line of code enables the used of print to console 
		#ifdef SWOPRINT
		TM_SWO_Init();		// Initialize SWO output
		#endif
		
		#ifdef SDCARD
			// Add an exterinal interupt on pin A0 so as to close the file and unmount the SD card
			TM_EXTI_Attach(GPIOA, GPIO_Pin_0, TM_EXTI_Trigger_Rising);	
			/* Mount drive */
			if (f_mount(&FatFs, "0:", 1) == FR_OK) 
				{
					/* Mounted OK, turn on RED LED */
					LedOn(LED_BLUE);
					f_open(&fil, "0:1stfile.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
				}
		#endif
				
		while (1) 
			{	
				PastCountVal = TIM_GetCounter(TIM2);
				Unfilter_roll_pitch_cal(I2C1, &angles[0], &angles[1]);
				angles[0] = angles[0]*180/PI;
				angles[1] = angles[1]*180/PI;
				
				#ifdef SWOPRINT
				TM_SWO_Printf("Roll has a value of: %f\n", angles[0]);
				TM_SWO_Printf("Pitch has a value of: %f\n\n", angles[1]);
				#endif
				
				#ifdef SDCARD
				if(Sdmounted)
					{
						sprintf(buffer, "Roll: %3.5f, Pitch: %3.5f \n", angles[0], angles[1]);
						f_puts(buffer, &fil);
					}
				#endif
				EnsureSetRuntime(&ConstantDelayStruct,PastCountVal, DelayFreq);
			}
}

#ifdef SDCARD
void TM_EXTI_Handler(uint16_t GPIO_Pin) 
	{
		/* Handle external line 0 interrupts */
		if (GPIO_Pin == GPIO_Pin_0) 
			{
				f_close(&fil);
				f_mount(0, "0:", 1);
				LedOff(LED_BLUE);
				Sdmounted = 0;
			}
	}
#endif
