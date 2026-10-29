#ifndef ADC_H_
#define ADC_H_

/*includes*/
#include "stm32f407xx.h"
#include "stm32f4xx.h"
#include "lib.h"
#include "events.h"
#include "GPIO_STM32F407.h"
#include <stdio.h>
//#include "PWR.h" //temp

/*defaines*/

/*RM0090 Reference manual STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439*/
/*13.13 ADC registers*/
/*13.13.1 Регистр состояния АЦП (ADC_SR) Address offset: 0x00*/
#define ADC_SR_Pos 0x0
#define ADC_FLAG_AWD (ADC_SR_AWD) /*(0x1 << 0)Читаем: Аналоговый флаг сторожевого таймера*/
#define ADC_FLAG_EOC (ADC_SR_EOC) /*(0x1 << 1)Читаем: Окончание преобразования обычного канала*/
#define ADC_FLAG_JEOC (ADC_SR_JEOC) /*(0x1 << 2)Читаем: Этот бит устанавливается аппаратно в конце преобразования всех инжектированных каналов в группе*/
#define ADC_FLAG_JSTRT (ADC_SR_JSTRT) /*(0x1 << 3)Читаем: Этот бит устанавливается аппаратно при запуске преобразования инжектированной группы*/
#define ADC_FLAG_STRT (ADC_SR_STRT) /*(0x1 << 4)Читаем: Этот бит устанавливается аппаратно при запуске обычного преобразования каналов*/
#define ADC_FLAG_OVR (ADC_SR_OVR) /*(0x1 << 5)Читаем:  Переполнение*/

/*13.13.2 Регистр управления 1 АЦП  (ADC_CR1) Address offset: 0x04*/
#define ADC_CR1_Pos 0x04
#define ADC_CANNAL0 (0)/*(0x00000 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL1 (ADC_CR1_AWDCH_0)/*(0x00001 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL2 (ADC_CR1_AWDCH_1)/*(0x00010 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL3 (ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)/*(0x00011 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL4 (ADC_CR1_AWDCH_2)/*(0x00100 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL5 (ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_0)/*(0x00101 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL6 (ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1)/*(0x00110 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL7 (ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)/*(0x00111 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL8 (ADC_CR1_AWDCH_3)/*(0x01000 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL9 (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_0)/*(0x01001 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL10 (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_1)/*(0x01010 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL11 (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)/*(0x01011 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL12 (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2)/*(0x01100 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL13 (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_0)/*(0x01101 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL14 (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1)/*(0x01110 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_CANNAL15 (ADC_CR1_AWDCH_3 | ADC_CR1_AWDCH_2 | ADC_CR1_AWDCH_1 | ADC_CR1_AWDCH_0)/*(0x01111 << 0)Выбора канала аналогового сторожевого таймера*/
#define ADC_IT_EOCIE (ADC_CR1_EOCIE) /*(0x1 << 5)Разрешение прерывания для EOC*/
#define ADC_IT_AWDIE (ADC_CR1_AWDIE) /*(0x1 << 6)Разрешение прерывания аналогового сторожевого таймера*/
#define ADC_IT_JEOCIE (ADC_CR1_JEOCIE) /*(0x1 << 7)Разрешение прерывания для инжектированных каналов*/
#define ADC_MODE_SCAN (0) /*(0 << 8)В режиме сканирования преобразуются входные сигналы, выбранные через регистры ADC_SQRx или ADC_JSQRx.*/
#define ADC_MODE_SCAN_INJECT (ADC_CR1_SCAN) /*(0x1 << 8)В режиме сканирования преобразуются входные сигналы, выбранные через регистры ADC_SQRx или ADC_JSQRx.*/
#define ADC_ENABLE_AWDSGL (ADC_CR1_AWDSGL) /*(0x1 << 9)Включение сторожевого таймера на одном канале в режиме сканирования*/
#define ADC_INJECT_GRUP_JAUTO (ADC_CR1_JAUTO)/*(0x1 << 10)Автоматическое преобразование инжектированной группы*/
#define ADC_MODE_DISCEN (ADC_CR1_DISCEN) /*(0x1 << 11)Прерывистый режим на обычных каналах*/
#define ADC_MODE_JDISCEN (ADC_CR1_JDISCEN) /*(0x1 << 12)Прерывистый режим на инжектированных каналах*/
#define ADC_CANNAL_COUNT1 (0)/*(0x000 << 13)Количество каналов в прерывистом режиме*/
#define ADC_CANNAL_COUNT2 (ADC_CR1_DISCNUM_0)/*(0x001 << 13)Количество каналов в прерывистом режиме*/
#define ADC_CANNAL_COUNT3 (ADC_CR1_DISCNUM_1)/*(0x010 << 13)Количество каналов в прерывистом режиме*/
#define ADC_CANNAL_COUNT4 (ADC_CR1_DISCNUM_1 | ADC_CR1_DISCNUM_0)/*(0x011 << 13)Количество каналов в прерывистом режиме*/
#define ADC_CANNAL_COUNT5 (ADC_CR1_DISCNUM_2)/*(0x100 << 13)Количество каналов в прерывистом режиме*/
#define ADC_CANNAL_COUNT6 (ADC_CR1_DISCNUM_2 | ADC_CR1_DISCNUM_0)/*(0x101 << 13)Количество каналов в прерывистом режиме*/
#define ADC_CANNAL_COUNT7 (ADC_CR1_DISCNUM_2 | ADC_CR1_DISCNUM_1)/*(0x110 << 13)Количество каналов в прерывистом режиме*/
#define ADC_CANNAL_COUNT8 (ADC_CR1_DISCNUM_2 | ADC_CR1_DISCNUM_1 | ADC_CR1_DISCNUM_0)/*(0x111 << 13)Количество каналов в прерывистом режиме*/
#define ADC_ENABLE_JAWDEN  (ADC_CR1_JAWDEN)/*(0x1 << 22)Включение аналогового сторожевого таймера на инжектированных каналах*/
#define ADC_ENABLE_AWDEN  (ADC_CR1_AWDEN)/*(0x1 << 23)Включение аналогового сторожевого таймера на обычных каналах*/
#define ADC_12bit (0)/*(0x00 << 24)Разрешения преобразования, 15 циклов ADCCLK*/
#define ADC_10bit (ADC_CR1_RES_0)/*(0x01 << 24)Разрешения преобразования, 13 циклов ADCCLK*/
#define ADC_8bit (ADC_CR1_RES_1)/*(0x10 << 24)Разрешения преобразования, 11 циклов ADCCLK*/
#define ADC_6bit (ADC_CR1_RES_1 | ADC_CR1_RES_0)/*(0x11 << 24)Разрешения преобразования, 9 циклов ADCCLK*/
#define ADC_IT_OVR (ADC_CR1_OVRIE) /*(0x1 << 26)Разрешение прерывания переполнения*/

/*13.13.3 Регистр управления 2 АЦП  (ADC_CR2) Address offset: 0x08*/
#define ADC_CR2_Pos 0x08
#define ADC_ENABLE (ADC_CR2_ADON)  /*(0x1 << 0)АЦП ВКЛ/ВЫКЛ*/
#define ADC_ENABLE_CONTINUOUS (ADC_CR2_CONT)  /*(0x1 << 1)Непрерывное преобразование*/
#define ADC_ENABLE_DMA (ADC_CR2_DMA)  /*(0x1 << 8)режим прямого доступа к памяти (для режима одного АЦП)*/
#define ADC_ENABLE_DMAContReq(DMAContReq_MODE) ((DMAContReq_MODE) << 9U)/*Включить непрерывный запрос ADC DMA.*/
#define ADC_END_CONVERSION (ADC_CR2_EOCS)  /*(0x1 << 10)Выбор конца преобразования, Бит EOC устанавливается в конце каждого обычного преобразования*/
#define ADC_DISABLE_END_CONVERSION (0)  /*Отключить Выбор конца преобразования*/
#define ADC_DATA_ALIGNMENT (ADC_CR2_ALIGN)  /*(0x1 << 11)выравнивание данных, 0: Выравнивание по правому краю , 1: Выравнивание по левому краю*/
#define ADC_EVENT_INJECT_TIMER1_CC4 (0) /*0000: Событие таймера 1 CC4 */
#define ADC_EVENT_INJECT_TIMER1_TRGO (ADC_CR2_JEXTSEL_0) /*0001: Событие таймера 1 TRGO*/
#define ADC_EVENT_INJECT_TIMER2_CC1 (ADC_CR2_JEXTSEL_1) /*0010: Событие таймера 2 CC1*/
#define ADC_EVENT_INJECT_TIMER2_TRGO (ADC_CR2_JEXTSEL_1|ADC_CR2_JEXTSEL_0) /*0011: Событие таймера 2 TRGO*/
#define ADC_EVENT_INJECT_TIMER3_CC2 (ADC_CR2_JEXTSEL_2) /*0100: Событие таймера 3 CC2*/
#define ADC_EVENT_INJECT_TIMER3_CC4 (ADC_CR2_JEXTSEL_2|ADC_CR2_JEXTSEL_0) /*0101: Событие таймера 3 CC4*/
#define ADC_EVENT_INJECT_TIMER4_CC1 (ADC_CR2_JEXTSEL_2|ADC_CR2_JEXTSEL_1) /*0110: Событие таймера 4 CC1*/
#define ADC_EVENT_INJECT_TIMER4_CC2 (ADC_CR2_JEXTSEL_2|ADC_CR2_JEXTSEL_1|ADC_CR2_JEXTSEL_0) /*0111: Событие таймера 4 CC2*/
#define ADC_EVENT_INJECT_TIMER4_CC3 (ADC_CR2_JEXTSEL_3) /*1000: Событие таймера 4 CC3*/
#define ADC_EVENT_INJECT_TIMER4_TRGO (ADC_CR2_JEXTSEL_3|ADC_CR2_JEXTSEL_0) /*1001: Событие таймера 4 TRGO*/
#define ADC_EVENT_INJECT_TIMER5_CC4 (ADC_CR2_JEXTSEL_3|ADC_CR2_JEXTSEL_1) /*1010: Событие таймера 5 CC4*/
#define ADC_EVENT_INJECT_TIMER5_TRGO (ADC_CR2_JEXTSEL_3|ADC_CR2_JEXTSEL_1|ADC_CR2_JEXTSEL_0) /*1011: Событие таймера 5 TRGO*/
#define ADC_EVENT_INJECT_TIMER8_CC2 (ADC_CR2_JEXTSEL_3|ADC_CR2_JEXTSEL_2) /*1100: Событие таймера 8 CC2*/
#define ADC_EVENT_INJECT_TIMER8_CC3 (ADC_CR2_JEXTSEL_3|ADC_CR2_JEXTSEL_2|ADC_CR2_JEXTSEL_0) /*1101: Событие таймера 8 CC3*/
#define ADC_EVENT_INJECT_TIMER8_CC4 (ADC_CR2_JEXTSEL_3|ADC_CR2_JEXTSEL_2|ADC_CR2_JEXTSEL_1) /*1110: Событие таймера 8 CC4*/
#define ADC_EVENT_INJECT_EXTI (ADC_CR2_JEXTSEL_3|ADC_CR2_JEXTSEL_2|ADC_CR2_JEXTSEL_1|ADC_CR2_JEXTSEL_0) /*1111: Линия EXTI15*/
#define ADC_NO_TRIGER_INJECT (0) /*0000: Обнаружение триггера отключено*/
#define ADC_TRIGER_INJECT_RISING (ADC_CR2_JEXTEN_0) /*0001: Обнаружение триггера по переднему фронту*/
#define ADC_TRIGER_INJECT_FALLING (ADC_CR2_JEXTEN_1) /*0010: Обнаружение триггера по заднему фронту*/
#define ADC_TRIGER_INJECT_ENABLE (ADC_CR2_JEXTEN_1|ADC_CR2_JEXTEN_0) /*0011: Обнаружение триггера как по переднему, так и по заднему фронту*/
#define ADC_ENABLE_INJECT_CANNAL (ADC_CR2_JSWSTART) /*Запуск преобразования инжектированных каналов*/
#define ADC_EVENT_TIMER1_CC1 (0) /*0000: Событие таймера 1 CC1 */
#define ADC_EVENT_TIMER1_CC2 (ADC_CR2_EXTSEL_0) /*0001: Событие таймера 1 CC2*/
#define ADC_EVENT_TIMER1_CC3 (ADC_CR2_EXTSEL_1) /*0010: Событие таймера 1 CC3*/
#define ADC_EVENT_TIMER2_CC2 (ADC_CR2_EXTSEL_1|ADC_CR2_EXTSEL_0) /*0011: Событие таймера 2 CC2*/
#define ADC_EVENT_TIMER2_CC3 (ADC_CR2_EXTSEL_2) /*0100: Событие таймера 2 CC3*/
#define ADC_EVENT_TIMER2_CC4 (ADC_CR2_EXTSEL_2|ADC_CR2_EXTSEL_0) /*0101: Событие таймера 2 CC4*/
#define ADC_EVENT_TIMER2_TRGO (ADC_CR2_EXTSEL_2|ADC_CR2_EXTSEL_1) /*0110: Событие таймера 2 TRGO*/
#define ADC_EVENT_TIMER3_CC1 (ADC_CR2_EXTSEL_2|ADC_CR2_EXTSEL_1|ADC_CR2_EXTSEL_0) /*0111: Событие таймера 3 CC1*/
#define ADC_EVENT_TIMER3_TRGO (ADC_CR2_EXTSEL_3) /*1000: Событие таймера 3 TRGO*/
#define ADC_EVENT_TIMER4_CC4 (ADC_CR2_EXTSEL_3|ADC_CR2_EXTSEL_0) /*1001: Событие таймера 4 CC4*/
#define ADC_EVENT_TIMER5_CC1 (ADC_CR2_EXTSEL_3|ADC_CR2_EXTSEL_1) /*1010: Событие таймера 5 CC1*/
#define ADC_EVENT_TIMER5_CC2 (ADC_CR2_EXTSEL_3|ADC_CR2_EXTSEL_1|ADC_CR2_EXTSEL_0) /*1011: Событие таймера 5 CC2*/
#define ADC_EVENT_TIMER5_CC3 (ADC_CR2_EXTSEL_3|ADC_CR2_EXTSEL_2) /*1100: Событие таймера 5 CC3*/
#define ADC_EVENT_TIMER8_CC1 (ADC_CR2_EXTSEL_3|ADC_CR2_EXTSEL_2|ADC_CR2_EXTSEL_0) /*1101: Событие таймера 8 CC1*/
#define ADC_EVENT_TIMER8_TRGO (ADC_CR2_EXTSEL_3|ADC_CR2_EXTSEL_2|ADC_CR2_EXTSEL_1) /*1110: Событие таймера 8 TRGO*/
#define ADC_EVENT_EXTI (ADC_CR2_EXTSEL_3|ADC_CR2_EXTSEL_2|ADC_CR2_EXTSEL_1|ADC_CR2_EXTSEL_0) /*1111: Линия EXTI15*/
#define ADC_SOFTWARE_START (ADC_EVENT_EXTI + 1U) /*если не используем внешнее событие*/
#define ADC_NO_TRIGER (0) /*0000: Обнаружение триггера отключено*/
#define ADC_TRIGER_RISING (ADC_CR2_EXTEN_0) /*0001: Обнаружение триггера по переднему фронту*/
#define ADC_TRIGER_FALLING (ADC_CR2_EXTEN_1) /*0010: Обнаружение триггера по заднему фронту*/
#define ADC_TRIGER_ENABLE (ADC_CR2_EXTEN_1|ADC_CR2_EXTEN_0) /*0011: Обнаружение триггера как по переднему, так и по заднему фронту*/
#define ADC_ENABLE_CANNAL (AADC_CR2_SWSTART) /*Запуск преобразования обычного каналов*/

/*13.13.4 Регистр 1 времени выборки АЦП (ADC_SMPR1) Address offset: 0x0C*/
#define ADC_SMPR1_Pos 0x0C
#define ADC_SMPR1(SAMPLETIME, CANNALx) ((SAMPLETIME) << (3U * (CANNALx - 10U)))/*: Выбор времени выборки канала 10-18*/

/*13.13.5 Регистр 2 времени выборки АЦП (ADC_SMPR2) Address offset: 0x10*/
#define ADC_SMPR2_Pos 0x10
#define ADC_SMPR2(SAMPLETIME, CANNALx) ((SAMPLETIME) << (3U * (CANNALx)))/*: Выбор времени выборки канала 0-9*/

#define ADC_SAMPLETIME_3_CYCLE_ (0x000) 
#define ADC_SAMPLETIME_15_CYCLE (0x001)
#define ADC_SAMPLETIME_28_CYCLE (0x010)
#define ADC_SAMPLETIME_56_CYCLE (0x011)
#define ADC_SAMPLETIME_84_CYCLE (0x100)
#define ADC_SAMPLETIME_112_CYCLE (0x101)
#define ADC_SAMPLETIME_144_CYCLE (0x110)
#define ADC_SAMPLETIME_480_CYCLE (0x111)

/*13.13.9 Регистр 1 последовательности (регулярной АЦП)(ADC_SQR1) Address offset: 0x2C*/
#define ADC_SQR1_Pos 0x2C
#define ADC_SQR1_RK(CANNALx, RANKNB) (((uint32_t)((uint16_t)(CANNALx))) << (5 * ((RANKNB) - 13)))/* Установите ранг обычного канала в диапазоне от 13 до 16.*/
#define ADC_SQR1(NbrOfConversion) ((NbrOfConversion - 1U) << 20U) /*Min_Data = 1 до Max_Data = 8, Задаёт количество дискретных преобразований, на которые будет разделена основная последовательность*/

/*13.13.10 Регистр 2 последовательности (регулярной АЦП)(ADC_SQR2) Address offset: 0x30*/
#define ADC_SQR2_Pos 0x30
#define ADC_SQR2_RK(CANNALx, RANKNB) (((uint32_t)((uint16_t)(CANNALx))) << (5 * ((RANKNB) - 7)))/* Установите ранг обычного канала в диапазоне от 7 до 12.*/

/*13.13.11 Регистр 3 последовательности (регулярной АЦП)(ADC_SQR3) Address offset: 0x34*/
#define ADC_SQR3_Pos 0x30
#define ADC_SQR3_RK(CANNALx, RANKNB) (((uint32_t)((uint16_t)(CANNALx))) << (5 * ((RANKNB) - 1)))/* Установите ранг обычного канала в диапазоне от 7 до 12.*/

/*13.13.14 Регистр данных АЦП(ADC_DR) Address offset: 0x4C*/
#define ADC_DR_Pos 0x4C
#define ADC_GET_DATA(INSTANCE) ((INSTANCE->DR))

#define ADC_STAB_DELAY_US (3) /* Задержка времени стабилизации АЦП*/

/*structures*/
typedef struct
{
  uint32_t ClockPrescaler;               /*Выберите предделитель тактовой частоты АЦП. Тактовая частота общая для
                                            всех АЦП.
                                            Этот параметр может иметь значение @ref ADC_ClockPrescaler */
  uint32_t Resolution;                   /*Настраивает разрешение АЦП.
                                            Этот параметр может иметь значение @ref ADC_Resolution. */
  uint32_t DataAlign;                    /*Задаёт выравнивание данных АЦП по правому краю (старший значащий бит в 11-м бите регистра и младший значащий бит в 0-м бите регистра) (настройка по умолчанию)
                                            или по левому краю (для обычной группы: старший значащий бит в 15-м бите регистра и младший значащий бит в 4-м бите регистра, для инжектированной группы (старший значащий бит сохраняется как знаковое значение из-за потенциально отрицательного значения после применения смещения): старший значащий бит в 14-м бите регистра и младший значащий бит в 3-м бите регистра).
                                            Этот параметр может быть значением @ref ADC_Data_align */
  uint32_t ScanConvMode;                 /*Настраивает секвенсор обычных и инжектированных групп.
                                            Этот параметр можно связать с параметром «DiscontinuousConvMode», чтобы основная последовательность была разделена на последовательные части.
                                            Если отключено: преобразование выполняется в одиночном режиме (преобразуется один канал, определенный в ранге 1).
                                            Параметры «NbrOfConversion» и «InjectedNbrOfConversion» отбрасываются (эквивалентно значению 1).
                                            Если включено: преобразования выполняются в последовательном режиме (несколько рангов, определенных параметрами «NbrOfConversion»/«InjectedNbrOfConversion» и рангом каждого канала).
                                            Направление сканирования — восходящее: от ранга 1 к рангу «n».
                                            Этот параметр можно установить в значения «ВКЛЮЧЕНО» или «ОТКЛЮЧЕНО». */
  uint32_t EOCSelection;                 /*Указывает, какой флаг EOC (End Of Conversion) используется для преобразования с помощью опроса и прерывания: конец преобразования каждого ранга или полная последовательность.
                                            Этот параметр может быть значением @ref ADC_EOCSelection.
                                            Примечание: Для инжектируемой группы сигнал окончания преобразования (flag&IT) активируется только в конце последовательности.
                                            Следовательно, если конец преобразования установлен на конец каждого преобразования, инжектируемую группу не следует использовать с прерыванием (HAL_ADCEx_InjectedStart_IT)
                                            или опросом (HAL_ADCEx_InjectedStart и HAL_ADCEx_InjectedPollForConversion). Опрос по-прежнему возможен, поскольку драйвер будет использовать предполагаемое время окончания инжектируемого преобразования. Примечание: Если предполагается использование функции переполнения, используйте АЦП в режиме «прерывание» (функция HAL_ADC_Start_IT()) с параметром EOCSelection, установленным на конец каждого преобразования, или в режиме «передача по DMA» (функция HAL_ADC_Start_DMA()).
                                            Если предполагается обход функции переполнения, используйте АЦП в режиме «опрос» или «прерывание», при этом параметр EOCSelection должен быть установлен на конец последовательности. */
  FunctionalState ContinuousConvMode;    /*Указывает, выполняется ли преобразование в одиночном режиме (одно преобразование) или непрерывно для обычной группы,
                                            после срабатывания выбранного триггера (запуск программы или внешний триггер).
                                            Этот параметр может быть установлен на ВКЛЮЧЕНО или ОТКЛЮЧЕНО. */
  uint32_t NbrOfConversion;              /*Указывает количество рангов, которые будут преобразованы в рамках обычного группового секвенсора.
                                            Чтобы использовать обычный групповой секвенсор и преобразовать несколько рангов, необходимо включить параметр «ScanConvMode».
                                            Этот параметр должен быть числом в диапазоне от Min_Data = 1 до Max_Data = 16. */
  FunctionalState DiscontinuousConvMode; /*Определяет, выполняется ли последовательность преобразований обычной группы в режиме Complete-sequence/Discontinuous-sequence (основная последовательность, разделенная на последовательные части).
                                            Прерывистый режим используется только при включенном секвенсоре (параметр ScanConvMode). Если секвенсор отключен, этот параметр отменяется.
                                            Прерывистый режим может быть включен только при отключенном непрерывном режиме. Если включен непрерывный режим, этот параметр отменяется.
                                            Этот параметр может быть установлен в значение ENABLE (ВКЛЮЧЕНО) или DISABLE (ОТКЛЮЧЕНО). */
  uint32_t NbrOfDiscConversion;          /* Задаёт количество дискретных преобразований, на которые будет разделена основная последовательность обычной группы (параметр NbrOfConversion).
                                            Если параметр DiscontinuousConvMode отключён, этот параметр отбрасывается.
                                            Значение этого параметра должно быть числом в диапазоне от Min_Data = 1 до Max_Data = 8. */
  uint32_t ExternalTrigConv;             /* Выбирает внешнее событие, используемое для запуска преобразования обычной группы.
                                            Если установлено значение ADC_SOFTWARE_START, внешние триггеры отключены.
                                            Если установлено значение внешнего источника триггера, триггер по умолчанию срабатывает по переднему фронту события.
                                            Этот параметр может иметь значение @ref ADC_External_trigger_Source_Regular */
  uint32_t ExternalTrigConvEdge;         /*Выбирает внешний фронт запуска обычной группы.
                                            Если задано значение ADC_SOFTWARE_START, этот параметр отбрасывается.
                                            Этот параметр может иметь значение @ref ADC_External_trigger_edge_Regular */
  FunctionalState DMAContinuousRequests; /*Указывает, выполняются ли запросы DMA в режиме однократного выполнения (передача DMA останавливается при достижении заданного количества преобразований)
                                          или в непрерывном режиме (передача DMA не ограничена, независимо от количества преобразований).
                                          Примечание: В непрерывном режиме DMA должен быть настроен на циклический режим. В противном случае при достижении указателя максимального значения буфера DMA будет срабатывать переполнение.
                                          Примечание: Этот параметр необходимо изменять, когда преобразования не выполняются ни в обычной, ни в инжектированной группе (АЦП отключен или АЦП включен без непрерывного режима или внешнего триггера, который мог бы запустить преобразование).
                                          Этот параметр может быть установлен в значение «ВКЛЮЧЕНО» (ENABLE) или «ОТКЛЮЧЕНО» (DISABLE) */
} ADC_Init_TypeDef;

typedef struct
{
  ADC_TypeDef *Instance; /*Зарегистрировать базовый адрес */

  ADC_Init_TypeDef Init; /*Требуемые параметры АЦП */

  uint32_t NbrOfCurrentConversionRank; /*Номер АЦП текущего ранга преобразования*/

  //DMA_HandleTypeDef *DMA_Handle; /*Обработчик указателя DMA */

  uint32_t Channel;      /*Указывает канал для настройки в обычной группе АЦП.
                            Этот параметр может иметь значение @ref ADC_channels */
  uint32_t Rank;         /*Указывает ранг в секвенсоре обычных групп.
                            Этот параметр должен быть числом от Min_Data = 1 до Max_Data = 16. */
  uint32_t SamplingTime; /*Значение времени выборки*/

} ADC_Structure;

/*var*/

/*func*/
RezultError ADC_Init(ADC_TypeDef* instance);
RezultError ADC_SetStructure(ADC_Structure* adc);
RezultError ADC_MspInit(ADC_Structure* adc);
RezultError ADC_SetReg(ADC_Structure* adc);
RezultError ADC_ConfigChannel(ADC_Structure* adc);
RezultError ADC_Start_IT(ADC_Structure* adc);
//void WorkADC(char* buff_str_temp);
void Enable_ADC(void);
void Disable_ADC(void);


//void TempChar(char* tmp);
#endif