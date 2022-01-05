#include "USART2.h"    //Файл описания регистров микроконтроллера

void init_USART2(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;       //Разрешение тактирования интерфейса USART2
    USART2->BRR = BRR_speed_USART2;                    //72МГц/9600 = 7500 = 0x1D4C
    //USART2->CR1 |= USART_CR1_RXNEIE;            //Разрешение прерывания (прием)
    //USART2->CR3 |= USART_CR3_DMAT;              //Разрешение на прием данных от DMA.
    //USART2->CR1 |= USART_CR1_OVER8_Msk;
    USART2->CR1 |= USART_CR1_UE;                //Включение модуля USART2
    USART2->CR1 |= USART_CR1_TE;                //Включение передатчика
    USART2->CR1 |= USART_CR1_RE;                //Включение приемника
    
    //UART2(PC)
            //PA2 -- TX  (AF4)
            GPIOA->MODER &=~ GPIO_MODER_MODE2;          //Сброс режима работы вывода A8
            GPIOA->MODER |= GPIO_MODER_MODE2_1;         //Выбор режима для вывода А8 (альтернативная функция)
            GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED2;   //Сброс скорости для вывода А8
            GPIOA->AFR[0] |= (4 << 8);                  //Установка альтернативной функции AF4
            //GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED2_1 | GPIO_OSPEEDER_OSPEED2_0;    //Выбор максимальной скорости для вывода А8 (Very high speed)
            
            //PA3 -- RX  (AF4)
            GPIOA->MODER &=~ GPIO_MODER_MODE3;          //Сброс режима работы вывода A8
            GPIOA->MODER |= GPIO_MODER_MODE3_1;         //Выбор режима для вывода А8 (альтернативная функция)
            GPIOA->AFR[0] |= (4 << 12);                 //Установка альтернативной функции AF4
            GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;         //Добавление подтяжки входа к + питания
            GPIOA->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED3;   //Сброс скорости для вывода А8
            //GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED3_1 | GPIO_OSPEEDER_OSPEED3_0;    //Выбор максимальной скорости для вывода А8 (Very high speed)
}

void UART2_send_string(char *stringUART2_TX)
{
    i = 0;                                        //Сброс счетчика
    while(stringUART2_TX[i] != 0)                 //Цикл, пока элемент массива не 0.
    {
        USART2->TDR = stringUART2_TX[i];          //Загружаем байт в регистр данных UART
        i++;                                      //Увеличиваем счетчик на 1
        while(1)                                    //Бесконечный цикл (ждем флаг завершения отправки)
        {
            if(USART2->ISR & USART_ISR_TXE){break;} //Если отправка завершена, выходим из цикла
        }
    }
}

void UART2_send_byte(char data)
{
    {
        USART2->TDR = data;          //Загружаем байт в регистр данных UART
        while(1)                                    //Бесконечный цикл (ждем флаг завершения отправки)
        {
            if(USART2->ISR & USART_ISR_TXE){break;} //Если отправка завершена, выходим из цикла
        }
    }
}