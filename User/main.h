/* Include core modules */
#include "stm32f4xx.h"
#include <math.h>


/* Include my libraries here */
#include "defines.h"
#include "10dof.h"
#include "led.h"
#include "Timer.h"
#include <stdio.h>
#include <string.h>

// Includes an acrruate delay function

#include "tm_stm32f4_delay.h"
 

#ifdef SWOPRINT
// Stuff for debug. These line of code enable printing to the console
#include "tm_stm32f4_swo.h"
#endif

