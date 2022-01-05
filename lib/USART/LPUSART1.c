#include "LPUSART1.h"    //Файл описания регистров микроконтроллера

void init_LPUSART1(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_LPUART1EN;       //Разрешение тактирования интерфейса LPUSART1
    LPUART1->BRR = 426666;           //Предделитель
    //USART1->CR1 |= USART_CR1_RXNEIE;           //Разрешение прерывания (прием)
    //USART1->CR3 |= USART_CR3_DMAT;             //Разрешение на прием данных от DMA.
    //LPUART1->CR1 |= USART_CR1_OVER8_Msk;         //Одна посылка - 8 бит
    LPUART1->CR1 |= USART_CR1_UE;                //Включение модуля USART2
    LPUART1->CR1 |= USART_CR1_TE;                //Включение передатчика
    LPUART1->CR1 |= USART_CR1_RE;                //Включение приемника

    //LPUART1(GSM)
            //TX
            GPIOB->MODER &=~ GPIO_MODER_MODE10;          //Сброс режима работы вывода A8
            GPIOB->MODER |= GPIO_MODER_MODE10_1;         //Выбор режима для вывода А8 (альтернативная функция)
            GPIOB->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED10;   //Сброс скорости для вывода А8
            GPIOB->AFR[1] |= (4 << 8);                   //Установка альтернативной функции F4
            //GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED10_1 | GPIO_OSPEEDER_OSPEED9_0;    //Выбор максимальной скорости для вывода А8 (Very high speed)
            
            //RX
            GPIOB->MODER &=~ GPIO_MODER_MODE11;          //Сброс режима работы вывода A8
            GPIOB->MODER |= GPIO_MODER_MODE11_1;         //Выбор режима для вывода А8 (альтернативная функция)
            GPIOB->AFR[1] |= (4 << 12);                   //Установка альтернативной функции F4
            GPIOB->PUPDR |= GPIO_PUPDR_PUPD11_0;         //Добавление подтяжки входа к + питания
            GPIOB->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED11;   //Сброс скорости для вывода А8
            //GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED11_1 | GPIO_OSPEEDER_OSPEED10_0;    //Выбор максимальной скорости для вывода А8 (Very high speed)
}

void send_string_LPUART1(char *stringLPUART1_TX)
{
    j = 0;                                           //Сброс счетчика
    while(stringLPUART1_TX[j] != 0)                  //Цикл, пока элемент массива не 0.
    {
        LPUART1->TDR = stringLPUART1_TX[j];          //Загружаем байт в регистр данных UART
        j++;                                         //Увеличиваем счетчик на 1
        while(1)                                     //Бесконечный цикл (ждем флаг завершения отправки)
        {
            if(LPUART1->ISR & USART_ISR_TXE){break;} //Если отправка завершена, выходим из цикла
        }
    }
}

void LPUART1_read_string(void)
{
    for(j = 0; j < 25; j++)
    {
        stringLPUART1_RX[j] = 0;
    }
    j = 0;                                            //Сброс счетчика
    while(1)                                          //Цикл, пока элемент массива не 0.
    {
        while(1)                                      //Бесконечный цикл (ждем флаг завершения отправки)
        {
            if(LPUART1->ISR & USART_ISR_RXNE){break;} //
        }
        stringLPUART1_RX[j] = LPUART1->RDR;
        if(stringLPUART1_RX[j] == 0x0D){break;}
        j++; 
    }
}
/*
50 61 72 6B 53 6F 6E 61 72 2D 45 5A 2D 37 32 0D 
50 4E 3A 4D 42 31 30 30 31 0D 
43 6F 70 79 72 69 67 68 74 20 32 30 31 34 2D 32 30 31 34 0D 
4D 61 78 42 6F 74 69 78 20 49 6E 63 2E 0D 
96 52 6F 48 53 20 31 2E 30 30 33 39 64 20 31 31 31 34 0D 
52 30 32 35 20 50 30 0D 
52 30 32 35 20 50 30 0D 
52 30 32 35 20 50 30 0D 
52 30 32 35 20 50 30 0D
*/