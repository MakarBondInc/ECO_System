#include "GPIO.h"
// Используемые интерфейсы 
//  SPI1
//      PA5 -- SCK  (AF0)
//      PA6 -- MISO  (AF0)
//      PA7 -- MOSI  (AF0)
//
//  USART2
//      PA2 -- TX  (AF4)
//      PA3 -- RX  (AF4)
//  LPUSART1
//      PB10 -- TX  (AF4)
//      PB11 -- RX  (AF4)

// Используевые порты ввода-вывода
    // PA0 -- IRQ_nFR
// PA1 -- INT1_AXL
    // PA10 -- RI_M66
    // PB2 -- CE_nFR
// PB9 -- INT2_AXL
    // PB12 -- PWRKEY_M66
    // PB13 -- CSN_nRF
// PB14 -- CS_AXL

 
void GPIO_Init()
{
    //Запуск тактирования портов
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;      //Запуск тактирования порта А
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;      //Запуск тактирования порта B
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;      //Запуск тактирования порта C

    //PA0 -- IRQ_nFR 
        GPIOA->MODER &=~ GPIO_MODER_MODE0;
        GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;         //Добавление подтяжки входа к + питания
        EXTI->IMR |= EXTI_IMR_IM0;                  //Разрешение внешнего прерывания на линии
        EXTI->FTSR |= EXTI_FTSR_FT0;                //Прерывание по спаду сигнала

    //PB13 -- CSN_nRF
        GPIOB->MODER &=~ GPIO_MODER_MODE13;
        GPIOB->MODER |= GPIO_MODER_MODE13_0;
        GPIOB->OTYPER &=~ GPIO_OTYPER_OT_13;

    //PB2 -- CE_nFR
        GPIOB->MODER &=~ GPIO_MODER_MODE2;
        GPIOB->MODER |= GPIO_MODER_MODE2_0;
        GPIOB->OTYPER &=~ GPIO_OTYPER_OT_2;

    //PB14 -- CS_AXL
        GPIOB->MODER &=~ GPIO_MODER_MODE14;
        GPIOB->MODER |= GPIO_MODER_MODE14_0;
        GPIOB->OTYPER &=~ GPIO_OTYPER_OT_14;

    //PB15 -- En 5V
        GPIOB->MODER &=~ GPIO_MODER_MODE15;
        GPIOB->MODER |= GPIO_MODER_MODE15_0;
        GPIOB->OTYPER &=~ GPIO_OTYPER_OT_15;

    //PA11 -- Led_1
        GPIOA->MODER &=~ GPIO_MODER_MODE11;
        GPIOA->MODER |= GPIO_MODER_MODE11_0;
        GPIOA->OTYPER &=~ GPIO_OTYPER_OT_11;

    //PA12 -- Led_2
        GPIOA->MODER &=~ GPIO_MODER_MODE12;
        GPIOA->MODER |= GPIO_MODER_MODE12_0;
        GPIOA->OTYPER &=~ GPIO_OTYPER_OT_12;

    //PB5 -- Led_3
        GPIOB->MODER &=~ GPIO_MODER_MODE5;
        GPIOB->MODER |= GPIO_MODER_MODE5_0;
        GPIOB->OTYPER &=~ GPIO_OTYPER_OT_5;

    //PB6 -- US_RX
        GPIOB->MODER &=~ GPIO_MODER_MODE6;
        GPIOB->MODER |= GPIO_MODER_MODE6_0;
        GPIOB->OTYPER &=~ GPIO_OTYPER_OT_6;

    //PB3 -- US_GND
        GPIOB->MODER &=~ GPIO_MODER_MODE3;
        GPIOB->MODER |= GPIO_MODER_MODE3_0;
        GPIOB->OTYPER &=~ GPIO_OTYPER_OT_3;
}