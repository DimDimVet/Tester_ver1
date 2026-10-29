#ifndef APP_H_
#define APP_H_

/*-----------------------------------------------------------------------------------------------*/

#include <string.h>
#include "GPIO_STM32F407.h"
#include "RCC_STM32F407.h"
#include "USB_FS_STM32F407.h"
#include "ADC.h"
#include "events.h"

void Work_USB();
void Handler_ADC_Event(uint16_t adcData, float adcVoltage);
//void Work_MSO();
//void Work_USART();
/*-----------------------------------------------------------------------------------------------*/

#endif