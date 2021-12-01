#include "RCC.h"

void HSE_RCC_init(void)
{
    RCC->CR |= RCC_CR_HSEON;                                    //Запуск внешнего генератора

    while(!((RCC->CR & RCC_CR_HSERDY) == RCC_CR_HSERDY)){;}     //Ожидание запуска внешнего генератора

    RCC->CFGR &=~ RCC_CFGR_PLLMUL;                              //Сброс коэффициента умножения ФАПЧ
    //RCC->CFGR |= RCC_CFGR_PLLMUL_1 | RCC_CFGR_PLLMUL_0;         //Установка коэффициента умножения ФАПЧ равным 8
    RCC->CFGR |= RCC_CFGR_PLLMUL_1;         //Установка коэффициента умножения ФАПЧ равным 6                       

    RCC->CFGR |= RCC_CFGR_PLLDIV_0;                             //Установка предделителя ФАПФ (значение делителя = 1)

    RCC->CFGR |= RCC_CFGR_PLLSRC;                               //Настройка входного сигнала для ФАПЧ (сигнал с HSE)
    
    RCC->CR |= RCC_CR_PLLON;                                    //Запуск ФАПЧ

    while(!((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY)){;}     //Ожидание запуска ФАПЧ

    FLASH->ACR |= FLASH_ACR_LATENCY;                            //Установка двух циклов ожидания памяти

    RCC->CFGR &=~ RCC_CFGR_HPRE;                                //Установка коэффиента предделения для шины AHB равным 1
    RCC->CFGR &=~ RCC_CFGR_PPRE1;                               //Установка коэффиента предделения для шины AHB1 равным 1
    RCC->CFGR &=~ RCC_CFGR_PPRE2;                               //Установка коэффиента предделения для шины AHB2 равным 1
    RCC->CFGR |= RCC_CFGR_SW_1 | RCC_CFGR_SW_0;                 //Переключение на тактирование от ФАПЧ

    while(!(((RCC->CFGR & RCC_CFGR_SWS_1) == RCC_CFGR_SWS_1) && ((RCC->CFGR & RCC_CFGR_SWS_0) == RCC_CFGR_SWS_0))){;} //Ожидание переключения на ФАПЧ
}

void MCO_OUT(void)
{
    RCC->CFGR &=~ RCC_CFGR_MCO_PRE;         //Установка предделителя MCO (значение делителя = 1)
    RCC->CFGR |= RCC_CFGR_MCOSEL_SYSCLK;    //Выбор сигнала тактирования, который будет выведен на выход MCO

    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;      //Включение тактирования порта А

    GPIOA->MODER &=~ GPIO_MODER_MODE8;          //Сброс режима работы вывода A8
    GPIOA->MODER |= GPIO_MODER_MODE8_1;         //Выбор режима для вывода А8 (альтернативная функция)
    GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED8;   //Сброс максимальной скорости для вывода А8
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED8_1 | GPIO_OSPEEDER_OSPEED8_0;    //Выбор максимальной скорости для вывода А8 (Very high speed)
}

void HSE_16MHz(void)
{
    RCC->CR |= RCC_CR_CSSHSEON;     //Запуск системы безопасности HSE
    RCC->CR |= RCC_CR_HSEON;        //Запуск генератора HSI
    while(!((RCC->CR & RCC_CR_HSERDY) == RCC_CR_HSERDY)){;}   //Проверка запуска HSE
    RCC->CFGR |= RCC_CFGR_SW_HSE;
}