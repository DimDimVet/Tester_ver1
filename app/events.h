#ifndef EVENTS_H_
#define EVENTS_H_

/*includes*/
#include "stm32f407xx.h"
#include "stm32f4xx.h"
#include "lib.h"
#include "GPIO_STM32F407.h"
#include "RCC_STM32F407.h"
#include "ADC.h"

/*defaines*/

/*structures*/

/*var*/

/*event var*/
//typedef void (*event_buttons_panel_t)(int vol);
typedef void (*event_ADC_t)(uint16_t adcData, float adcVoltage);
typedef void (*event_write_to_USART_t)(uint16_t vol, char* flag);
typedef void (*event_on_off_PWR_t)(uint8_t flag);


/*event func*/
//void Connect_Event_Buttons_panel(event_buttons_panel_t func);
//void Start_Event_Buttons_panel(int vol);

void Connect_Event_ADC(event_ADC_t func);
void Start_Event_ADC(uint16_t adcData, float adcVoltage);

void Connect_Event_Write_To_USART(event_write_to_USART_t func);
void Start_Event_Write_To_USART(uint16_t vol, char* flag);

void Connect_Event_On_Off_PWR(event_on_off_PWR_t func);
void Start_Event_On_Off_PWR(uint8_t flag);

/*func*/


#endif