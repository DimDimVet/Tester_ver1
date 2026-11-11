#include "app.h"

/*-----------------------------------------------------------------------------------------------*/
/*Настройки кода входящего пакета USB*/
uint8_t count_size_buf;
char btn_ON_RELE[SIZE_BUFF_USB] = BTN_ON_RELE_FLAG_USB;
char btn_OFF_RELE[SIZE_BUFF_USB] = BTN_OFF_RELE_FLAG_USB;

/*-----------------------------------------------------------------------------------------------*/

/*Настройки портов*/
	GPIO_Structure LED_7 = {.GPIOx = GPIOA, .Pin = PIN7, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407ve
	GPIO_Structure LED_6 = {.GPIOx = GPIOA, .Pin = PIN6, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407ve
	
	GPIO_Structure RELE_ON = {.GPIOx = GPIOA, .Pin = PIN2, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//PA2
	GPIO_Structure RELE_OFF = {.GPIOx = GPIOA, .Pin = PIN3, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//PA3

/*-----------------------------------------------------------------------------------------------*/
/*var для разного*/
uint8_t tst = 0;
//uint8_t tstRele = 0;
float rez_temp = 3;
uint8_t data11[100];

int main ()
 {
	 
	AHB1_ENABLE_PERIPHERY(RCC_AHB1ENR_GPIOAEN);/*LED2 PA6,LED3 PA7*/
	AHB1_ENABLE_PERIPHERY(RCC_AHB1ENR_GPIOFEN);/*LED0 PF9,LED1 PF10*/


	GPIO_Structure LED_0 = {.GPIOx = GPIOF, .Pin = PIN9, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407zgt6
	GPIO_Structure LED_1 = {.GPIOx = GPIOF, .Pin = PIN10, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407zgt6
	
	
	GPIO_Init(&LED_7);
	GPIO_Init(&LED_6);
	GPIO_Init(&LED_0);
	GPIO_Init(&LED_1);
	
	GPIO_Init(&RELE_ON);
	GPIO_Init(&RELE_OFF);
	
	Connect_Event_ADC(Handler_ADC_Event);
	
	Work_USB();
	ADC_Init(ADC1);
	
	ODR_1 (&RELE_ON);
	ODR_0 (&RELE_OFF);

while (1)
  {
//		ODR_Xor(&LED_7);
//    ODR_Xor(&LED_6);
		ODR_Xor(&LED_0);
    ODR_Xor(&LED_1);
		

//		if (tstRele == 1)
//		{
//		ODR_1(&RELE_ON);
//		delay_ms(10);	
//		ODR_0(&RELE_ON);
//		ODR_0(&RELE_OFF);
//			tstRele = 0;
//		}
//		else if (tstRele == 0)
//		{
//			ODR_0(&RELE_ON);
//			ODR_1(&RELE_OFF);
//			delay_ms(10);
//			ODR_0(&RELE_OFF);
//			tstRele = 1;
//		}
//		
//		if(tst == 1)
//		{
//			USB_send_data(data11, 15);
//		}
		
		
		delay_ms(1);
  }

return 0;
 }
 /*-----------------------------------------------------------------------------------------------*/
 /*Функция включения USB*/
void Work_USB()
{
  /*--Настройка RCC под USB 48Мгц от 8МГц кварца--*/
  RCC_Oscillator_t osc;
  osc.HSE_State = RCC_HSE_ON;
  osc.HSE_Frequence = 8000000;
  osc.HSI_State = RCC_HSI_ON;
  osc.HSI_CalibrationValue = 30;
  osc.LSI_State = RCC_LSI_ON;

  RCC_Oscillator_Init(&osc);

  RCC_PLL_t pll;
  pll.PLL_State = RCC_PLL_ON;
  pll.PLL_Sourse = RCC_PLLSOURCE_HSE;//HSI 1-1,HSE 1-3
  pll.PLL_M = 4;
  pll.PLL_N = 168;
  pll.PLL_P = 2;
  pll.PLL_Q = 7;

  RCC_PLL_Init(&pll);

  RCC_Sysclk_t sysclk;
  sysclk.SYSCLK_Sourse = RCC_SYSCLK_SOURCE_PLL;
  sysclk.AHB_Divider = RCC_AHB_DIV1;

  RCC_Sysclk_Init(&sysclk, &osc, &pll);

  /*--Инициализация USB--*/
  AHB2_ENABLE_PERIPHERY(RCC_AHB2ENR_OTGFSEN);//включим OTGFS

  __disable_irq ();

  USB_Init_GPIO();

  USB_Init_Reg();

  __enable_irq ();
}

/*-----------------------------------------------------------------------------------------------*/
/*Обработчик USB по прерыванию*/
uint32_t USB_Handler_Recieve_data(uint16_t length)
{
  /*Принять*/
  uint8_t *data = EndPoint[1].rxBuffer_ptr;
  char in_usb_data[SIZE_BUFF_USB];

  for (uint32_t i = 0; i < length; i++)
    {
      in_usb_data[i] = data[i];
    }

	Decoder_usb(in_usb_data,length);
		
  /*Передать*/
  //USB_send_data(in_usb_data, length); //для тестов

  return length;
}

/*-----------------------------------------------------------------------------------------------*/
void Handler_ADC_Event(uint16_t adcData, float adcVoltage)
{

  char buff_str_temp[50];
  char buff_str_PWR[50];
  char buff_str_Step[50];

  sprintf(buff_str_temp, "Volt=%.2fV", adcVoltage);

//  sprintf(buff_str_Step, "Step temp=%d  ", pwr_str.step_temp);
//  sprintf(buff_str_PWR, "Fill PWR=%.2d", pwr_str.fill_Factor);
//sprintf(buff_str_temp, "Curr.Temp=%.1fC  ", rez_temp);
	
	
  

  for (uint32_t i = 0; i < 15; i++)
    {
      data11[i] = buff_str_temp[i];
    }
  //USB_send_data(data11, 15);
  //SSD1306_Clear(&ssd);

//  current_screen.str0 = buff_str_temp;
//  current_screen.x0 = 10;

////  current_screen.str1 = buff_str_temp;
////  current_screen.x1 = 10;

//  current_screen.str2 = buff_str_pwr_on;
//  current_screen.x2 = 10;

////  current_screen.str3 = rezultReadUsart;
////  current_screen.x3 = 10;

//  current_screen.str4 = buff_str_Step;
//  current_screen.x4 = 10;

//  current_screen.str5 = buff_str_PWR;
//  current_screen.x5 = 10;

//  Work_Screen(&ssd, &current_screen);

//  Handler_ADC_PWR(&pwr_str, adcData, &rez_temp);
}

/*-----------------------------------------------------------------------------------------------*/
void Decoder_usb(char *data, uint16_t len)
{
	char in_usb_data[SIZE_BUFF_USB];

  for (uint32_t i = 0; i < len; i++)
    {
      in_usb_data[i] = data[i];
    }

		if (Comparator_Arr(btn_ON_RELE, in_usb_data, len))
    {/*вынести в метод*/
      ODR_1(&RELE_ON);
			delay_ms(10);	
			ODR_0(&RELE_ON);
			ODR_0(&RELE_OFF);
			USB_send_data(data11, 15);
			ODR_Xor(&LED_7);
    }
		
		if (Comparator_Arr(btn_OFF_RELE, in_usb_data, len))
    {/*вынести в метод*/
			ODR_0(&RELE_ON);
			ODR_1(&RELE_OFF);
			delay_ms(10);
			ODR_0(&RELE_OFF);
			USB_send_data(data11, 15);
			ODR_Xor(&LED_6);
    }


}

uint16_t Comparator_Arr(char* arr1, char* arr2, uint16_t len)
{
  for (uint16_t i = 0; i < len; i++)
    {

      if (arr1[i] != arr2[i])
        {
          return 0;
        }
    }

  return 1;
}