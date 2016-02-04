/*
 * 
 *
 *  Created on: Dec 9, 2015
 *      Author: cobrien
 */
#ifndef __Timer_H_
#define __Timer_H_ 21

#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include <math.h>

/**
 * @brief  Initializes specific timer for Counter capability and sets the required prescaler and Period for the counter
 * @param  *TIM_Data: Pointer to blank structure. Here will init function save all data for specific timer
 * @param  DelayFreq: This value is the inverse of the delay time (1/ts). 
 * @retval None
 */
void InitializeTimer(TIM_TimeBaseInitTypeDef* timerInitStructure, uint32_t DelayFreq);


/**
 * @brief  Used for the collect of data to ensure a set delay between succive data 
 * @param  *TIM_Data: Pointer to initialized structure. Period, Prescaler etc have been set 
 * @param  PastCountVal: The counter value at the start of the code used to calculate delay. 
 * @param  DelayFreq: This value is the inverse of the delay time (1/ts). 
 * @retval None
 */
void EnsureSetRuntime(TIM_TimeBaseInitTypeDef* timerInitStructure, uint32_t PastCountVal, uint32_t DelayFreq);

#ifdef __cplusplus
}
#endif

#endif /* __Timer_H_ */
