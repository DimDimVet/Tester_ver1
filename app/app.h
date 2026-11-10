#ifndef APP_H_
#define APP_H_

/*-----------------------------------------------------------------------------------------------*/

#include <string.h>
#include "GPIO_STM32F407.h"
#include "RCC_STM32F407.h"
#include "USB_FS_STM32F407.h"
#include "ADC.h"
#include "events.h"

/*-----------------------------------------------------------------------------------------------*/
/*Служебные деФАЙНЫ*/
/*Размер буфера*/
#define SIZE_BUFF_USB 36
/*Конец строчного пакета*/
#define STOP_FLAG_USB '#'
/*значение пакета*/
#define BTN_ON_RELE_FLAG_USB "BtnOnRELE#\n"
#define BTN_OFF_RELE_FLAG_USB "BtnOffRELE#\n"


void Work_USB();
void Handler_ADC_Event(uint16_t adcData, float adcVoltage);
void Decoder_usb(char *data, uint16_t len);
uint16_t Comparator_Arr(char* arr1, char* arr2, uint16_t len);
//void Work_MSO();
//void Work_USART();
/*-----------------------------------------------------------------------------------------------*/

#endif