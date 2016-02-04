#include "led.h"

/* Button pressed status for onpressed/onreleased events */
static volatile uint8_t INT_LED_ButtonPressed = 0;

void LedInit(void) {
	/* Set pins as output */
	TM_GPIO_Init(LED_PORT, LED_ALL, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
	
	/* Turn leds off */
	LedOff(LED_ALL);
}

void LED_ButtonInit(void) {
	/* Set pin as input */
	TM_GPIO_Init(BUTTON_PORT, BUTTON_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, BUTTON_PULL, TM_GPIO_Speed_Low);
}

uint8_t LED_ButtonOnPressed(void) {
	/* If button is now pressed, but was not already pressed */
	if (LED_ButtonPressed()) {
		if (!INT_LED_ButtonPressed) {
			/* Set flag */
			INT_LED_ButtonPressed = 1;
			
			/* Return button onpressed */
			return 1;
		}
	} else {
		/* Clear flag */
		INT_LED_ButtonPressed = 0;
	}
	
	/* Button is not pressed or it was already pressed before */
	return 0;
}

uint8_t LED_ButtonOnReleased(void) {
	/* If button is now released, but was not already released */
	if (!LED_ButtonPressed()) {
		if (INT_LED_ButtonPressed) {
			/* Set flag */
			INT_LED_ButtonPressed = 0;
			
			/* Return button onreleased */
			return 1;
		}
	} else {
		/* Set flag */
		INT_LED_ButtonPressed = 1;
	}
	
	/* Button is not released or it was already released before */
	return 0;
}

