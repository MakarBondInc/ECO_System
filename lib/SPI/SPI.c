#include "SPI.h"

void init_SPI1(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;     //Разрешение тактирования интерфейса SPI
    SPI1->CR1 |= SPI_CR1_MSTR;              //Мастер
    SPI1->CR1 &=~ SPI_CR1_BR;               //
    SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;              //Fsys / 8 = 2 МГц
    //SPI2->CR2 |= SPI_CR2_ERRIE;
    //SPI2->CR2 |= SPI_CR2_RXNEIE;            //Разрешение прерывания по приему данных
    //SPI2->CR1 |= SPI_CR1_LSBFIRST;           //Последовательность бит LSB

    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    SPI1->CR1 |= SPI_CR1_SPE;               //Включение модуля SPI

    //PA5 -- SCK  (AF0)
        GPIOA->MODER &=~ GPIO_MODER_MODE5;          //Сброс режима работы вывода A5
        GPIOA->MODER |= GPIO_MODER_MODE5_1;         //Выбор режима для вывода А5 (альтернативная функция)
        GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED5;   //Сброс скорости для вывода А5
        GPIOA->AFR[0] |= (0 << 20);                 //Установка альтернативной функции (AF0)
    //PA6 -- MISO  (AF0)
        GPIOA->MODER &=~ GPIO_MODER_MODE6;          //Сброс режима работы вывода A5
        GPIOA->MODER |= GPIO_MODER_MODE6_1;         //Выбор режима для вывода А5 (альтернативная функция)
        GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED6;   //Сброс скорости для вывода А5
        GPIOA->AFR[0] |= (0 << 24);                 //Установка альтернативной функции (AF0)
    //PA7 -- MOSI  (AF0)
        GPIOA->MODER &=~ GPIO_MODER_MODE7;          //Сброс режима работы вывода A5
        GPIOA->MODER |= GPIO_MODER_MODE7_1;         //Выбор режима для вывода А5 (альтернативная функция)
        GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED7;   //Сброс скорости для вывода А5
        GPIOA->AFR[0] |= (0 << 28);                 //Установка альтернативной функции (AF0)
}

void send_byte_SPI1(uint8_t data)
{
    SPI1->DR = data;
    while(1)                                    //Бесконечный цикл (ждем флаг завершения отправки)
    {
        if(SPI1->SR & SPI_SR_TXE){break;}       //Если отправка завершена, выходим из цикла
    }
}