#include "app.h"

/*-----------------------------------------------------------------------------------------------*/
int main ()
 {
	 
	AHB1_ENABLE_PERIPHERY(RCC_AHB1ENR_GPIOAEN);/*LED2 PA6,LED3 PA7*/

	GPIO_Structure LED_7 = {.GPIOx = GPIOA, .Pin = PIN7, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};
	GPIO_Structure LED_6 = {.GPIOx = GPIOA, .Pin = PIN6, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};
	GPIO_Init(&LED_7);
	GPIO_Init(&LED_6);

while (1)
  {
		ODR_Xor(&LED_7);
    ODR_Xor(&LED_6);
    delay_ms(200);
  }

return 0;
 }
 /*-----------------------------------------------------------------------------------------------*/