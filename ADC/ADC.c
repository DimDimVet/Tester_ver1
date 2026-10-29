#include "ADC.h"

/*var*/
ADC_Structure adc;
uint16_t adcData;
float adcVoltage;

RezultError ADC_Init(ADC_TypeDef* instance)
{
  RezultError tmp_rezult = _OK;

  if (instance == 0)//придумать вместо NULL
    {
      tmp_rezult = _ERROR;
    }
  else
    {
      adc.Instance = instance;
      ADC_SetStructure(&adc);
    }

  ADC_Start_IT(&adc); ///zzz

  return tmp_rezult;
}

RezultError ADC_SetStructure(ADC_Structure* adc)
{
  RezultError tmp_rezult = _OK;

  adc->Init.ClockPrescaler = 0;
  adc->Init.Resolution = 0;
  adc->Init.ScanConvMode = ADC_MODE_SCAN;
  adc->Init.ContinuousConvMode = ADC_ENABLE_CONTINUOUS; ///запуск последовательности измерений
  adc->Init.DiscontinuousConvMode = ADC_CANNAL_COUNT1;
  adc->Init.ExternalTrigConvEdge = 0;
  adc->Init.ExternalTrigConv = ADC_SOFTWARE_START;//0x0F000001
  adc->Init.DataAlign = 0;
  adc->Init.NbrOfConversion = 1;
  adc->Init.DMAContinuousRequests = 0;
  adc->Init.EOCSelection = ADC_DISABLE_END_CONVERSION; ///измеряем все ацп которые используем

  adc->Channel = ADC_CANNAL1;//канал 1
  adc->Rank = 1;
  adc->SamplingTime = ADC_SAMPLETIME_28_CYCLE;//ADC_SMPR1_SMP10_1;//циклы подсчета

  ADC_MspInit(adc);

  ADC_SetReg(adc);


  if (ADC_ConfigChannel(adc) != _OK)
    {
      tmp_rezult = _ERROR;
    }

  return tmp_rezult;

}

RezultError ADC_MspInit(ADC_Structure* adc)
{
  RezultError tmp_rezult = _OK;

  GPIO_Structure GPIO_InitStruct = {.GPIOx = GPIOA, .Pin = PIN1, .Mode = GPIO_MODE_ANALOG, .Pull = GPIO_PUPDR_NOPULL};

  if (adc->Instance == ADC1) /*ADC1 GPIO Configuration PA1 --> ADC1_IN1 */
    {
      Enable_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);

      Enable_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

      GPIO_Init(&GPIO_InitStruct);
    }

  /* ADC прерывания */
  NVIC_SetPriority(ADC_IRQn, 15);
  NVIC_EnableIRQ(ADC_IRQn);

  return tmp_rezult;
}

RezultError ADC_SetReg(ADC_Structure* adc)
{
  RezultError tmp_rezult = _OK;

  ADC_Common_TypeDef *tmpADC_Common;

  /* Указатель на общий регистр управления, к которому принадлежит adc */
  tmpADC_Common = ADC123_COMMON;

  /* Установите предделитель тактовой частоты АЦП */
  tmpADC_Common->CCR &= ~(ADC_CCR_ADCPRE);
  tmpADC_Common->CCR |=  adc->Init.ClockPrescaler;

  /* Установить режим сканирования АЦП */
  adc->Instance->CR1 &= ~(ADC_MODE_SCAN);
  adc->Instance->CR1 |=  adc->Init.ScanConvMode;;

  /* Установить разрешение АЦП */
  adc->Instance->CR1 &= ~(ADC_CR1_RES);
  adc->Instance->CR1 |=  adc->Init.Resolution;

  /* Установить выравнивание данных АЦП */
  adc->Instance->CR2 &= ~(ADC_CR2_ALIGN);
  adc->Instance->CR2 |= adc->Init.DataAlign;

  /* Включить внешний триггер, если выбор триггера отличается от запуска программного обеспечения */
  if (adc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
    {
      /* Выберите внешний триггер для начала преобразования */
      adc->Instance->CR2 &= ~(ADC_CR2_EXTSEL);
      adc->Instance->CR2 |= adc->Init.ExternalTrigConv;

      /* Выберите полярность внешнего триггера */
      adc->Instance->CR2 &= ~(ADC_CR2_EXTEN);
      adc->Instance->CR2 |= adc->Init.ExternalTrigConvEdge;
    }
  else
    {
      /* Сброс внешнего триггера */
      adc->Instance->CR2 &= ~(ADC_CR2_EXTSEL);
      adc->Instance->CR2 &= ~(ADC_CR2_EXTEN);
    }

  /* Включить или отключить режим непрерывного преобразования АЦП */
  adc->Instance->CR2 &= ~(ADC_CR2_CONT);
  adc->Instance->CR2 |= adc->Init.ContinuousConvMode;

  if (adc->Init.DiscontinuousConvMode != DISABLE)
    {
      /* Включить выбранный регулярный прерывистый режим АЦП */
      adc->Instance->CR1 |= (uint32_t)ADC_CR1_DISCEN;

      /* Установите количество каналов для преобразования в прерывистом режиме. */
      adc->Instance->CR1 &= ~(ADC_CR1_DISCNUM);
      adc->Instance->CR1 |=  adc->Init.NbrOfDiscConversion;
    }
  else
    {
      /* Отключить выбранный режим постоянного прерывистого преобразования АЦП */
      adc->Instance->CR1 &= ~(ADC_CR1_DISCEN);
    }

  /* Установить номер преобразования АЦП */
  adc->Instance->SQR1 &= ~(ADC_SQR1_L);
  adc->Instance->SQR1 |=  ADC_SQR1(adc->Init.NbrOfConversion);

  /* Включить или отключить непрерывный запрос ADC DMA */
  adc->Instance->CR2 &= ~(ADC_CR2_DDS);
  adc->Instance->CR2 |= ADC_ENABLE_DMAContReq(adc->Init.DMAContinuousRequests);

  /* Включить или отключить выбор конца преобразования АЦП */
  adc->Instance->CR2 &= ~(ADC_CR2_EOCS);
  adc->Instance->CR2 |= adc->Init.EOCSelection;//ADC_CR2_EOCSelection(adc->Init.EOCSelection);

  return tmp_rezult;
}

RezultError ADC_ConfigChannel(ADC_Structure* adc)
{
  RezultError tmp_rezult = _OK;

  ADC_Common_TypeDef *tmpADC_Common;

  /* если выбран ADC_Channel_10 ... ADC_Channel_18 */
  if (adc->Channel > ADC_CANNAL9)
    {
      /* Очистить старое время выборки */
      adc->Instance->SMPR1 &= ~ADC_SMPR1(ADC_SMPR1_SMP10, adc->Channel);

      /* Установите новое время выборки */
      adc->Instance->SMPR1 |= ADC_SMPR1(adc->SamplingTime, adc->Channel);
    }
  else /* ADC_Channel включает в себя ADC_Channel_[0..9]*/
    {
      /* Очистить старое время выборки */
      adc->Instance->SMPR2 &= ~ADC_SMPR2(ADC_SMPR2_SMP0, adc->Channel);

      /* Установите новое время выборки */
      adc->Instance->SMPR2 |= ADC_SMPR2(adc->SamplingTime, adc->Channel);
    }

  /* Для рангов с 1 по 6*/
  if (adc->Rank < 7U)
    {
      /* Очистить старые биты SQx для выбранного ранга */
      adc->Instance->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, adc->Rank);

      /* Установите биты SQx для выбранного ранга */
      adc->Instance->SQR3 |= ADC_SQR3_RK(adc->Channel, adc->Rank);
    }
  /* Для рангов с 7 по 12 */
  else if (adc->Rank < 13U)
    {
      /* Очистить старые биты SQx для выбранного ранга */
      adc->Instance->SQR2 &= ~ADC_SQR2_RK(ADC_SQR2_SQ7, adc->Rank);

      /* Установите биты SQx для выбранного ранга */
      adc->Instance->SQR2 |= ADC_SQR2_RK(adc->Channel, adc->Rank);
    }
  /* Для рангов с 13 по 16 */
  else
    {
      /* Очистить старые биты SQx для выбранного ранга */
      adc->Instance->SQR1 &= ~ADC_SQR1_RK(ADC_SQR1_SQ13, adc->Rank);

      /* Установите биты SQx для выбранного ранга */
      adc->Instance->SQR1 |= ADC_SQR1_RK(adc->Channel, adc->Rank);
    }

  /* Указатель на общий регистр управления, к которому принадлежит adc   */
  tmpADC_Common = ADC123_COMMON;

  return tmp_rezult;
}

RezultError ADC_Start_IT(ADC_Structure* adc)
{
  RezultError tmp_rezult = _OK;

  uint32_t counter = 0U;
  ADC_Common_TypeDef *tmpADC_Common;

  /* Включить периферийное устройство */
  Enable_ADC();

  counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));

  while (counter != 0U)
    {
      counter--;
    }

  /* Указатель на общий регистр управления, к которому принадлежит adc    */
  tmpADC_Common = ADC123_COMMON;

  /* Очистить флаг преобразования обычной группы и флаг переполнения */
  Disable_BIT(adc->Instance->SR, (ADC_FLAG_EOC | ADC_FLAG_OVR));

  /* Включить прерывание конверсии для обычной группы */
  Enable_BIT(adc->Instance->CR1, (ADC_IT_EOCIE | ADC_IT_OVR));

  /* Проверьте, включен ли многомодовый режим */
  if (Disable_BIT(tmpADC_Common->CCR, ADC_CCR_MULTI))
    {
      if ((adc->Instance == ADC1) || ((adc->Instance == ADC2) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_0)) \
      || ((adc->Instance == ADC3) && ((ADC->CCR & ADC_CCR_MULTI_Msk) < ADC_CCR_MULTI_4)))
        {
          /* если внешний триггер отсутствует, включите программное преобразование обычных каналов */
          if ((adc->Instance->CR2 & ADC_CR2_EXTEN) == RESET)
            {
              /* Включить выбранное программное преобразование АЦП для обычной группы */
              Enable_BIT(adc->Instance->CR2, ADC_CR2_SWSTART);
            }
        }
    }
  else
    {
      /* если экземпляр дескриптора соответствует АЦП1 и нет внешнего триггера, включите программное преобразование обычных каналов */
      if ((adc->Instance == ADC1) && ((adc->Instance->CR2 & ADC_CR2_EXTEN) == RESET))
        {
          /* Включить выбранное программное преобразование АЦП для обычной группы */
          Enable_BIT(adc->Instance->CR2, ADC_CR2_SWSTART);
        }
    }

  return tmp_rezult;
}

void Enable_ADC()
{
  Enable_BIT(adc.Instance->CR2, ADC_CR2_ADON);
  Enable_BIT(adc.Instance->CR2, ADC_CR2_SWSTART);
}

void Disable_ADC()
{
  Disable_BIT(adc.Instance->CR2, ADC_CR2_ADON);
  Disable_BIT(adc.Instance->CR2, ADC_CR2_SWSTART);
}

void ADC_IRQHandler(void)
{
  adcData = ADC_GET_DATA(adc.Instance);

	if (Read_BIT(adc.Instance->SR,ADC_SR_EOC))
    {
				Disable_BIT(adc.Instance->SR,ADC_SR_EOC);
				
        adcVoltage = adcData * 3.0 / 4095;
				Start_Event_ADC(adcData,adcVoltage);
				Disable_ADC();
    }
}
