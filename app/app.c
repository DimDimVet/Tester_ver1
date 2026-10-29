#include "app.h"

/*-----------------------------------------------------------------------------------------------*/
/*var для разного*/
uint8_t tst = 0;
float rez_temp = 3;
uint8_t data1[100];

int main ()
 {
	 
	 uint8_t data2[100] = "A1D2E3";
	 
	AHB1_ENABLE_PERIPHERY(RCC_AHB1ENR_GPIOAEN);/*LED2 PA6,LED3 PA7*/
	AHB1_ENABLE_PERIPHERY(RCC_AHB1ENR_GPIOFEN);/*LED0 PF9,LED1 PF10*/

	GPIO_Structure LED_7 = {.GPIOx = GPIOA, .Pin = PIN7, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407ve
	GPIO_Structure LED_6 = {.GPIOx = GPIOA, .Pin = PIN6, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407ve
	GPIO_Structure LED_0 = {.GPIOx = GPIOF, .Pin = PIN9, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407zgt6
	GPIO_Structure LED_1 = {.GPIOx = GPIOF, .Pin = PIN10, .Mode = GPIO_MODE_OUTPUT, .Speed = GPIO_SPEED_LOW, .Pull = GPIO_PUPDR_PULLUP};//stm32f407zgt6
	
	GPIO_Init(&LED_7);
	GPIO_Init(&LED_6);
	GPIO_Init(&LED_0);
	GPIO_Init(&LED_1);
	
	Connect_Event_ADC(Handler_ADC_Event);
	
	
	Work_USB();
	//ADC_Init(ADC1);
	uint8_t data3[100] = "AER";
	

while (1)
  {
		ODR_Xor(&LED_7);
    ODR_Xor(&LED_6);
		ODR_Xor(&LED_0);
    ODR_Xor(&LED_1);
		Enable_ADC();
		//USB_send_data(data3, 15);

		
		delay_ms(200);
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
/*Обработчик USB по прерыванию*/
uint32_t USB_Handler_Recieve_data(uint16_t length)
{
  /*Принять*/
  uint8_t *data = EndPoint[1].rxBuffer_ptr;
  uint8_t data1[100];

  for (uint32_t i = 0; i < length; i++)
    {
      data1[i] = data[i];
    }

  /*Передать*/
  USB_send_data(data1, length);
  tst = 1;
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
      data1[i] = buff_str_temp[i];
    }
  USB_send_data(data1, 15);
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