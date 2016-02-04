#include "Timer.h"

void InitializeTimer(TIM_TimeBaseInitTypeDef* timerInitStructure, uint32_t DelayFreq)
{	
		/* Enable clock for TIM2 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		// Set the Prescaler to zero before calculation
		timerInitStructure->TIM_Prescaler = 0;
		// Used to see if the the counter divides in evenly. It is an easy way to check if the counter is correctly set  

			while(1)
			{
				if( ((2*84000000/DelayFreq)%(timerInitStructure->TIM_Prescaler + 1)-1) ==0)
					break;	
				else
					// Increment the Prescaler and continue while loop
					timerInitStructure->TIM_Prescaler += 1;				
				// check to see if the code is runing out of range. Hopefully this shouldn't happen
				if (timerInitStructure->TIM_Prescaler >= 65535 && timerInitStructure->TIM_Prescaler >= 65535)
					{	
						#ifdef SWOPRINT
						TM_SWO_Printf("Presaler and period for timer are going out of range :(");
						#endif
						break;
					}
			}		
		timerInitStructure->TIM_Period = (2*84000000)/((timerInitStructure->TIM_Prescaler + 1)*DelayFreq)-1;
		/* Count up */
    timerInitStructure->TIM_CounterMode = TIM_CounterMode_Up;	
		// Set the reset of the required entries 
    timerInitStructure->TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure->TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
}

void EnsureSetRuntime(TIM_TimeBaseInitTypeDef* timerInitStructure, uint32_t PastCountVal, uint32_t DelayFreq)
{	
	// Flag used set if counter rolls over
	uint8_t RapFlag;
	// Calculates the number of ticks of counter rquired to get the required delay on code 
	uint32_t count = (84000000)/((timerInitStructure->TIM_Prescaler + 1)*DelayFreq)-1;
	// varible used as check to see if required number of ticks have been achived 
	uint32_t check;
	// number that has to be added onto the counter in the case of a rap around
	uint32_t rap_addtion = timerInitStructure->TIM_Period - PastCountVal;
	while(check < count)
	{	
		// Check to see if there has been a rap around.
		RapFlag = PastCountVal > TIM_GetCounter(TIM2);
		
		// If a Rap around has occred ticks before rap have to be added on
		if (RapFlag != 0)
			check = rap_addtion + TIM_GetCounter(TIM2);
		else
			// If there is no rap number of ticks is calculated using this formula. You just have to take the current number of ticks 
			// from the number of ticks accumulated on counter at start of code
			check = TIM_GetCounter(TIM2) - PastCountVal;
	}
}
