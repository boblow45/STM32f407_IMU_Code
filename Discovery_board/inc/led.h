#ifndef LED_H
#define LED_H 23

#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

	#define LED_GREEN					GPIO_PIN_12
	#define LED_ORANGE				GPIO_PIN_13
	#define LED_RED						GPIO_PIN_14
	#define LED_BLUE					GPIO_PIN_15
	#define LED_ALL						LED_GREEN | LED_RED | LED_ORANGE | LED_BLUE
	
	#define LED_PORT			GPIOD
	#define LED_PINS			LED_GREEN | LED_RED | LED_ORANGE | LED_BLUE

	#define BUTTON_PORT			GPIOA
	#define BUTTON_PIN			GPIO_PIN_0
	#define BUTTON_PRESSED		1
	#define BUTTON_PULL			TM_GPIO_PuPd_DOWN




/**
 * @brief  Configures LED pins as outputs
 * @param  None
 * @retval None
 */
void LedInit(void);

/**
 * @brief  Configures Button pin as input
 * @param  None
 * @retval None
 */
void LED_ButtonInit(void);

/**
 * @brief  Turns on LED on board
 * @note   STM32F4x9-Eval board uses inverse logic for leds
 * @param  led: LED you want to turn on
 *            - LED_RED: Red led
 *            - LED_GREEN: Green led
 *            - LED_BLUE: Blue led
 *            - LED_ORANGE: Orange led
 *            - LED_ALL: All leds
 * @retval None
 */

	#define LedOn(led)        TM_GPIO_SetPinHigh(LED_PORT, (led))



/**
 * @brief  Turns off LED on board
 * @note   STM32F4x9-Eval board uses inverse logic for leds
 * @param  led: LED you want to turn off
 *            - LED_RED: Red led
 *            - LED_GREEN: Green led
 *            - LED_BLUE: Blue led
 *            - LED_ORANGE: Orange led
 *            - LED_ALL: All leds
 * @retval None
 */

	#define LedOff(led)       TM_GPIO_SetPinLow(LED_PORT, (led))
	

/**
 * @brief  Toggles LED on board
 * @param  led: LED you want to toggle
 *            - LED_RED: Red led
 *            - LED_GREEN: Green led
 *            - LED_BLUE: Blue led
 *            - LED_ORANGE: Orange led
 *            - LED_ALL: All leds
 * @retval None
 */
#define LedToggle(led)        TM_GPIO_TogglePinValue(LED_PORT, (led))

/**
 * @brief  Checks if led is on
 * @param  led: Led you want to checking
 *            - LED_RED: Red led
 *            - LED_GREEN: Green led
 *            - LED_BLUE: Blue led
 *            - LED_ORANGE: Orange led
 *            - LED_ALL: All leds
 * @retval 1 if led is on or 0 if not
 */

#define LedIsOn(led)          TM_GPIO_GetOutputPinValue(LED_PORT, (led))

/**
 * @brief  Sets led value
 * @param  led: LED you want to set value
 *            - LED_RED: Red led
 *            - LED_GREEN: Green led
 *            - LED_BLUE: Blue led
 *            - LED_ORANGE: Orange led
 *            - LED_ALL: All leds
 * @param  state: Set or clear led 
 *            - 0: led is off
 *            - > 0: led is on
 * @retval None
 */
#define SetLed(led, state)    ((state) ? LedOn(led): LedOff(led))

/**
 * @brief  Checks if user button is pressed
 * @param  None
 * @retval Button status
 *            - 0: Button is not pressed
 *            - > 0: Button is pressed
 */
#define LED_ButtonPressed()       ((TM_GPIO_GetInputPinValue(BUTTON_PORT, BUTTON_PIN) == 0) != BUTTON_PRESSED)

/**
 * @brief  Checks if button was pressed now, but was not already pressed before
 * @param  None
 * @retval Button on pressed value
 *           - 0: In case that button has been already pressed on last call or was not pressed at all yet
 *           - > 0: Button was pressed, but state before was released
 */
uint8_t LED_ButtonOnPressed(void);

/**
 * @brief  Checks if button was released now, but was already pressed before
 * @param  None
 * @retval Button on released value
 *           - 0: In case that button has been already released on last call or was not released at all yet
 *           - > 0: Button was released, but state before was pressed
 */
uint8_t LED_ButtonOnReleased(void);


#endif
