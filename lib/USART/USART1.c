#include "USART1.h"    //Файл описания регистров микроконтроллера

void init_USART1(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;       //Разрешение тактирования интерфейса USART1
    USART1->BRR = BRR_speed_USART1;             //Предделитель       
    //USART1->CR1 |= USART_CR1_RXNEIE;          //Разрешение прерывания (прием)
    //USART1->CR3 |= USART_CR3_DMAT;            //Разрешение на прием данных от DMA.
    USART1->CR1 |= USART_CR1_OVER8_Msk;         //Одна посылка - 8 бит
    USART1->CR1 |= USART_CR1_UE;                //Включение модуля USART2
    USART1->CR1 |= USART_CR1_TE;                //Включение передатчика
    USART1->CR1 |= USART_CR1_RE;                //Включение приемника

    //UART1(GSM)
            //TX
            GPIOA->MODER &=~ GPIO_MODER_MODE9;          //Сброс режима работы вывода A8
            GPIOA->MODER |= GPIO_MODER_MODE9_1;         //Выбор режима для вывода А8 (альтернативная функция)
            GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED9;   //Сброс скорости для вывода А8
            GPIOA->AFR[1] |= (4 << 4);                  //Установка альтернативной функции F4
            //GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED9_1 | GPIO_OSPEEDER_OSPEED9_0;    //Выбор максимальной скорости для вывода А8 (Very high speed)
            
            //RX
            GPIOA->MODER &=~ GPIO_MODER_MODE10;          //Сброс режима работы вывода A8
            GPIOA->MODER |= GPIO_MODER_MODE10_1;         //Выбор режима для вывода А8 (альтернативная функция)
            GPIOA->AFR[1] |= (4 << 8);                 //Установка альтернативной функции F4
            GPIOA->PUPDR |= GPIO_PUPDR_PUPD10_0;         //Добавление подтяжки входа к + питания
            GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED10;   //Сброс скорости для вывода А8
            //GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED10_1 | GPIO_OSPEEDER_OSPEED10_0;    //Выбор максимальной скорости для вывода А8 (Very high speed)
}

void send_string_UART1(char *stringUART1_TX)
{
    j = 0;                                        //Сброс счетчика
    while(stringUART1_TX[j] != 0)                 //Цикл, пока элемент массива не 0.
    {
        USART1->TDR = stringUART1_TX[j];          //Загружаем байт в регистр данных UART
        j++;                                      //Увеличиваем счетчик на 1
        while(1)                                    //Бесконечный цикл (ждем флаг завершения отправки)
        {
            if(USART1->ISR & USART_ISR_TXE){break;} //Если отправка завершена, выходим из цикла
        }
    }
}